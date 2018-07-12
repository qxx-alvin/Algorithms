#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <queue>

using namespace std;

#define N 16

int linkCapacity[N][N];				// ��·����
int nodeFlow[N];						// �����ڵ������������ǰ·����Сʣ������
int preNode[N];							// ��ǰ·���Ͻڵ��ǰ���ڵ�

// �Ҵ�src��dst������·��������Сʣ��
int BFS(int src, int dst)
{
	queue<int> Q;
	for (int i = 0; i < N; i++)
	{
		preNode[i] = -1;
	}
	preNode[src] = 0;
	nodeFlow[src] = INT_MAX;
	Q.push(src);
	while (!Q.empty())
	{
		int curNode = Q.front();
		Q.pop();
		if (curNode == dst)
			break;
		for (int i = 1; i <= dst; i++)
		{
			if (i != src && linkCapacity[curNode][i] > 0 && preNode[i] == -1)
			{
				preNode[i] = curNode;
				nodeFlow[i] = min(linkCapacity[curNode][i], nodeFlow[curNode]);
				Q.push(i);
			}
		}
	}
	if (preNode[dst] != -1)					
		return nodeFlow[dst];
	else												// ������Ŀ�Ľڵ㣬���Ҳ�������·
		return -1;
}

int MaxFlow(int src, int dst)
{
	int aug;
	int sumFlow = 0;
	while ((aug = BFS(src, dst)) != -1)
	{
		int cur = dst;
		while (cur != src)
		{
			int pre = preNode[cur];
			linkCapacity[pre][cur] -= aug;
			linkCapacity[cur][pre] += aug;
			cur = pre;
		}
		sumFlow += aug;
	}
	return sumFlow;
}

int main()
{
	int cases;
	int nodes, links;
	int u, v, w;
	scanf("%d", &cases);
	for (int i = 0; i < cases; ++i)
	{
		scanf("%d%d", &nodes, &links);
		memset(linkCapacity, 0, sizeof(linkCapacity));
		memset(nodeFlow, 0, sizeof(nodeFlow));
		for (int j = 0; j < links; j++)
		{
			scanf("%d%d%d", &u, &v, &w);
			linkCapacity[u][v] = w;
		}
		printf("case %d: %d\n", i, MaxFlow(1, nodes));
	}
	return 0;
}