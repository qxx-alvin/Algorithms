#include <iostream>
#include <vector>

using namespace std;

// 获取k（下标）节点的第th个邻居的下标
// 参数：
//		k：下标，取值0~n*n-1
//		n：矩阵宽度
//		th：取值0~3，依次表示上下左右
// 返回：
//		若th方向上没有邻居，则返回-1
int neighbor(int k, int n, int th)
{
	int k_row = k / n;
	int k_col = k % n;
	int row, col;
	if (th == 0)
	{
		row = k_row - 1;
		col = k_col;
	}
	else if (th == 1)
	{
		row = k_row + 1;
		col = k_col;
	}
	else if (th == 2)
	{
		row = k_row;
		col = k_col - 1;
	}
	else if (th == 3)
	{
		row = k_row;
		col = k_col + 1;
	}
	if (row >= n || row < 0 || col >= n || col < 0)
		return -1;
	else
		return row * n + col;
}

void main()
{
	// 读入数据并计算路径
	int T;
	cin >> T;
	vector<int> n(T), t(T);
	vector<bool> res(T, false);   // 是否可达
	vector<int> min_time(T);			// 若可达，记录最小时间
	for (int i = 0; i < T; i++)
	{
		cin >> n[i];
		cin >> t[i];
		vector<char> graph(n[i] * n[i]);   // 存储图中各个节点的字符
		int S_ind;						// 记录出发点的位置
		for (int j = 0; j < n[i]; j++)
		{
			for (int k = 0; k < n[i]; k++)
			{
				cin >> graph[j * n[i] + k];
				if (graph[j * n[i] + k] == 'S')
				{
					S_ind = j * n[i] + k;
				}
			}
		}
		//========= 找路径 ========//
		vector<bool> visited(n[i] * n[i], 0);  // 节点是否已经访问过
		vector<int> dist(n[i]*n[i], 0);   // 维护到各个节点的最短距离，0表示无穷
		// 初始化构造visited和dist
		for (int j = 0; j < 4; j++)   // 四个邻居
		{
			int nei_ind = neighbor(S_ind, n[i], j);
			if (nei_ind != -1)
			{ 
				if (graph[nei_ind] != 'O')   // 障碍物
					dist[nei_ind] = 2;
			}
		}
		visited[S_ind] = true;

		int min_ind, min;
		for (int j = 0; j < n[i]*n[i] - 1; j++)
		{
			min = 1e8;
			min_ind = -1;
			// 寻找可达的最近节点
			for (int k = 0; k < n[i] * n[i]; k++)
			{
				if (!visited[k] && dist[k] != 0 && dist[k] < min)
				{
					min = dist[k];
					min_ind = k;
				}
			}
			// 到此节点的最短距离确定下来
			visited[min_ind] = 1;
			// 检查是否已经到达目的地
			if (graph[min_ind] == 'X')
			{
				if (dist[min_ind] > t[i])   // 超过所需时间
					res[i] = false;
				else			// 在所需时间内到达
				{
					res[i] = true;
					min_time[i] = dist[min_ind];
				}
				break;
			}
			// 更新S到min_ind的邻居的距离
			int dis = (graph[min_ind] == 'C' ? 1 : 2);  // 若当前节点为'C'，则到邻居距离为1，否则为2
			for (int k = 0; k < 4; k++)  // 检查四个邻居
			{
				int nei_ind = neighbor(min_ind, n[i], k);
				if (nei_ind != -1 && !visited[nei_ind] && graph[nei_ind] != 'O' &&
					(dist[nei_ind] == 0 || dist[nei_ind] != 0 && min + dis < dist[nei_ind]))
					dist[nei_ind] = min + dis;
			}
		}

	}

	// 输出
	for (int i = 0; i < T; i++)
	{
		if (res[i] == false)
			cout << "NO\n";
		else
			cout << "YES\n" << min_time[i] << endl;
	}

	char tmp;
	cin >> tmp;
}