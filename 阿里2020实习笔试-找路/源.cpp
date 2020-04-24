#include <iostream>
#include <vector>

using namespace std;

// ��ȡk���±꣩�ڵ�ĵ�th���ھӵ��±�
// ������
//		k���±꣬ȡֵ0~n*n-1
//		n��������
//		th��ȡֵ0~3�����α�ʾ��������
// ���أ�
//		��th������û���ھӣ��򷵻�-1
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
	// �������ݲ�����·��
	int T;
	cin >> T;
	vector<int> n(T), t(T);
	vector<bool> res(T, false);   // �Ƿ�ɴ�
	vector<int> min_time(T);			// ���ɴ��¼��Сʱ��
	for (int i = 0; i < T; i++)
	{
		cin >> n[i];
		cin >> t[i];
		vector<char> graph(n[i] * n[i]);   // �洢ͼ�и����ڵ���ַ�
		int S_ind;						// ��¼�������λ��
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
		//========= ��·�� ========//
		vector<bool> visited(n[i] * n[i], 0);  // �ڵ��Ƿ��Ѿ����ʹ�
		vector<int> dist(n[i]*n[i], 0);   // ά���������ڵ����̾��룬0��ʾ����
		// ��ʼ������visited��dist
		for (int j = 0; j < 4; j++)   // �ĸ��ھ�
		{
			int nei_ind = neighbor(S_ind, n[i], j);
			if (nei_ind != -1)
			{ 
				if (graph[nei_ind] != 'O')   // �ϰ���
					dist[nei_ind] = 2;
			}
		}
		visited[S_ind] = true;

		int min_ind, min;
		for (int j = 0; j < n[i]*n[i] - 1; j++)
		{
			min = 1e8;
			min_ind = -1;
			// Ѱ�ҿɴ������ڵ�
			for (int k = 0; k < n[i] * n[i]; k++)
			{
				if (!visited[k] && dist[k] != 0 && dist[k] < min)
				{
					min = dist[k];
					min_ind = k;
				}
			}
			// ���˽ڵ����̾���ȷ������
			visited[min_ind] = 1;
			// ����Ƿ��Ѿ�����Ŀ�ĵ�
			if (graph[min_ind] == 'X')
			{
				if (dist[min_ind] > t[i])   // ��������ʱ��
					res[i] = false;
				else			// ������ʱ���ڵ���
				{
					res[i] = true;
					min_time[i] = dist[min_ind];
				}
				break;
			}
			// ����S��min_ind���ھӵľ���
			int dis = (graph[min_ind] == 'C' ? 1 : 2);  // ����ǰ�ڵ�Ϊ'C'�����ھӾ���Ϊ1������Ϊ2
			for (int k = 0; k < 4; k++)  // ����ĸ��ھ�
			{
				int nei_ind = neighbor(min_ind, n[i], k);
				if (nei_ind != -1 && !visited[nei_ind] && graph[nei_ind] != 'O' &&
					(dist[nei_ind] == 0 || dist[nei_ind] != 0 && min + dis < dist[nei_ind]))
					dist[nei_ind] = min + dis;
			}
		}

	}

	// ���
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