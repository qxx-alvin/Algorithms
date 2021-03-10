// 代码摘自：https://www.cnblogs.com/zhsl/p/3271754.html

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
int link[N];     //表示哪个点匹配了哪个点
int Father[N];   //这个就是增广路的Father……但是用起来太精髓了
int Base[N];     //该点属于哪朵花
int Q[N];
bool mark[N];   // 一个辅助的数组
bool map[N][N];
bool InBlossom[N];
bool in_Queue[N];  // whether in queue

// 添加双向链路
void addBiLink(int x, int y)
{
	map[x][y] = map[y][x] = 1;
}

// 添加匹配链路
void addMatchLink(int x, int y)
{
	link[x] = y;
	link[y] = x;
}

void CreateGraph(){
	int x, y;
	//scanf("%d", &n);
	//while (scanf("%d%d", &x, &y) != EOF)  // 节点下标从1开始
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
	for (i = x; i; i = pre)    // 从x点往前回溯
	{ 
		i = Base[i]; 
		mark[i] = true; 
	}
	for (i = y; i; i = pre) {  // 环内环绕
		i = Base[i]; 
		if (mark[i])   // 在前一步mark了，在这一步遇到了，表明找到了环顶
		{
			lca = i; 
			break; 
		} 
	}  //寻找lca之旅……一定要注意i=Base[i]
	for (i = x; Base[i] != lca; i = pre){      // 将环内节点放在花中
		if (Base[pre] != lca) 
			Father[pre] = link[i]; //对于BFS树中的父边是匹配边的点，Father向后跳 ？？？
		InBlossom[Base[i]] = true;
		InBlossom[Base[link[i]]] = true;
	}
	for (i = y; Base[i] != lca; i = pre){
		if (Base[pre] != lca) 
			Father[pre] = link[i]; //同理 ？？？
		InBlossom[Base[i]] = true;
		InBlossom[Base[link[i]]] = true;
	}
#undef pre
	if (Base[x] != lca) 
		Father[x] = y;     //注意不能从lca这个奇环的关键点跳回来 ？？？
	if (Base[y] != lca) 
		Father[y] = x;   // ？？？
	for (i = 1; i <= n; i++)
		if (InBlossom[Base[i]]){
			Base[i] = lca;      // 放在花中
			if (!in_Queue[i]){     
				Q[++tail] = i;
				in_Queue[i] = true;     //要注意如果本来连向BFS树中父结点的边是非匹配边的点，可能是没有入队的
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
	for (int i = 1; i <= n; i++) Base[i] = i;  // 自成一朵花
	head = 0; tail = 1;   // reset queue
	Q[1] = Start; // enqueue
	in_Queue[Start] = 1; 
	while (head != tail){
		int x = Q[++head];  // dequeue
		for (int y = 1; y <= n; y++)    // BFS
			if (map[x][y] && Base[x] != Base[y] && link[x] != y)   // 不在同一朵花，且未匹配
				if (Start == y || link[y] && Father[link[y]])    // 精髓地用Father表示该点是否被访问。Start==y means a loop from the beginning; the latter condition means a loop
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