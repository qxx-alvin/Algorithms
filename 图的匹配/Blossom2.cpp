// ����ժ�ԣ�https://www.cnblogs.com/zhsl/p/3271754.html

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int N = 250;
int n;
int head;   // queue head
int tail;   // queue tail
int Start;
int Finish;
int link[N];     //��ʾ�ĸ���ƥ�����ĸ���
int Father[N];   //�����������·��Father��������������̫������
int Base[N];     //�õ������Ķ仨
int Q[N];
bool mark[N];   // һ������������
bool map[N][N];
bool InBlossom[N];
bool in_Queue[N];  // whether in queue

// ���˫����·
void addBiLink(int x, int y)
{
	map[x][y] = map[y][x] = 1;
}

// ���ƥ����·
void addMatchLink(int x, int y)
{
	link[x] = y;
	link[y] = x;
}

void CreateGraph(){
	int x, y;
	//scanf("%d", &n);
	//while (scanf("%d%d", &x, &y) != EOF)  // �ڵ��±��1��ʼ
	//	map[x][y] = map[y][x] = 1;
	n = 10;
	addBiLink(1, 2);
	addBiLink(2, 3);
	addBiLink(3, 4);
	addBiLink(5, 6);
	addBiLink(6, 7);
	addBiLink(7, 8);
	addBiLink(4, 8);
	addBiLink(1, 9);
	addBiLink(9, 5);
	addBiLink(7, 10);

	addMatchLink(1, 2);
	addMatchLink(3, 4);
	addMatchLink(5, 6);
	addMatchLink(7, 8);

}


void BlossomContract(int x, int y){
	fill(mark, mark + n + 1, false);
	fill(InBlossom, InBlossom + n + 1, false);
#define pre Father[link[i]]
	int lca, i;
	for (i = x; i; i = pre)    // ��x����ǰ����
	{ 
		i = Base[i]; 
		mark[i] = true; 
	}
	for (i = y; i; i = pre) {  // ���ڻ���
		i = Base[i]; 
		if (mark[i])   // ��ǰһ��mark�ˣ�����һ�������ˣ������ҵ��˻���
		{
			lca = i; 
			break; 
		} 
	}  //Ѱ��lca֮�á���һ��Ҫע��i=Base[i]
	for (i = x; Base[i] != lca; i = pre){      // �����ڽڵ���ڻ���
		if (Base[pre] != lca) 
			Father[pre] = link[i]; //����BFS���еĸ�����ƥ��ߵĵ㣬Father����� ������
		InBlossom[Base[i]] = true;
		InBlossom[Base[link[i]]] = true;
	}
	for (i = y; Base[i] != lca; i = pre){
		if (Base[pre] != lca) 
			Father[pre] = link[i]; //ͬ�� ������
		InBlossom[Base[i]] = true;
		InBlossom[Base[link[i]]] = true;
	}
#undef pre
	if (Base[x] != lca) 
		Father[x] = y;     //ע�ⲻ�ܴ�lca����滷�Ĺؼ��������� ������
	if (Base[y] != lca) 
		Father[y] = x;   // ������
	for (i = 1; i <= n; i++)
		if (InBlossom[Base[i]]){
			Base[i] = lca;      // ���ڻ���
			if (!in_Queue[i]){     
				Q[++tail] = i;
				in_Queue[i] = true;     //Ҫע�������������BFS���и����ı��Ƿ�ƥ��ߵĵ㣬������û����ӵ�
			}
		}
}

// Modify matching with augmenting path
void Change(){
	int x, y, z;
	z = Finish;
	while (z){     // Stop when z equals Start
		y = Father[z];    // y is a S-vertex
		x = link[y];      // x is a T-vertex
		link[y] = z;
		link[z] = y;
		z = x;
	}
}

void FindAugmentPath(){
	fill(Father, Father + n + 1, 0);
	fill(in_Queue, in_Queue + n + 1, false);
	for (int i = 1; i <= n; i++) Base[i] = i;  // �Գ�һ�仨
	head = 0; tail = 1;   // reset queue
	Q[1] = Start; // enqueue
	in_Queue[Start] = 1; 
	while (head != tail){
		int x = Q[++head];  // dequeue
		for (int y = 1; y <= n; y++)    // BFS
			if (map[x][y] && Base[x] != Base[y] && link[x] != y)   // ����ͬһ�仨����δƥ��
				if (Start == y || link[y] && Father[link[y]])    // �������Father��ʾ�õ��Ƿ񱻷��ʡ�Start==y means a loop from the beginning; the latter condition means a loop
					BlossomContract(x, y);
				else if (!Father[y]){  // if (Father[y]), this means y has been visited, skip
					Father[y] = x;   // used for backtrace
					if (link[y]){   // matched
						Q[++tail] = link[y]; // enqueue S-vertex
						in_Queue[link[y]] = true; // label it 
					}
					else{   // found augmenting path
						Finish = y;    
						Change();    // Start is matched after this step
						return;   
					}
				}
	}
}

void Edmonds(){
	//memset(link, 0, sizeof(link));
	for (Start = 1; Start <= n; Start++)
		if (link[Start] == 0)
			FindAugmentPath();
}

void output(){
	fill(mark, mark + n + 1, false);
	int cnt = 0;
	for (int i = 1; i <= n; i++)
		if (link[i]) cnt++;
	printf("%d\n", cnt);
	for (int i = 1; i <= n; i++)
		if (!mark[i] && link[i]){
			mark[i] = true;
			mark[link[i]] = true;
			printf("%d %d\n", i, link[i]);
		}
}

int main(){
	//    freopen("input.txt","r",stdin);
	CreateGraph();
	Edmonds();
	output();
	system("pause");
}