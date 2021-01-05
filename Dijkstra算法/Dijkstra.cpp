/* Dijkstra algorithm */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <Windows.h>

//==================自定义以下内容，作为算法输入==================//
#define N_NODE 5            // 节点数目
double delay_matrix[N_NODE][N_NODE] = { -1, -1, -1, -1, -1, 
										-1, -1, 2, 4, 3, 
										-1, 2, -1, -1, -1, 
										-1, 4, -1, -1, -1, 
										-1, 3, -1, -1, -1};  // 邻接矩阵（-1表示不连通）
//==============================================================//

void main()
{
	int i, j, k;

	for (int n = 0; n < N_NODE; n++)
	{
		printf("节点%d路由表：\n-----------------------\n目的节点\t下一跳\n-----------------------\n", n+1);
		double dist_array[N_NODE];				//modified during the process
		memcpy(dist_array, delay_matrix[n], sizeof(dist_array));

		int visited[N_NODE];
		memset(visited, 0, sizeof(visited));
		visited[n] = 1;

		int min_ind;
		double min;

		int trans[N_NODE];
		for (i = 0; i < N_NODE; i++)
			trans[i] = -1;						//-1 means no relay

		for (i = 0; i < N_NODE - 1; i++)
		{
			min = DBL_MAX;
			min_ind = -1;
			for (j = 0; j < N_NODE; j++)
			{
				if (!visited[j] && dist_array[j] != -1 && dist_array[j] < min)		
				{
					min = dist_array[j];
					min_ind = j;
				}
			}
			if (min_ind == -1)	// each non-visited node is unreachable
			{
				for (j = 0; j < N_NODE; j++)
				{
					if (!visited[j])
					{
						trans[j] = -2; // -2 means unreachable
					}
				}
				break;
			}
			visited[min_ind] = 1;

			for (j = 0; j < N_NODE; j++)
			{
				if (!visited[j] && delay_matrix[min_ind][j] != -1 && (dist_array[j] == -1 || (dist_array[j] != -1
					&& min + delay_matrix[min_ind][j] < dist_array[j])))
				{
					dist_array[j] = min + delay_matrix[min_ind][j];				//update distance
					trans[j] = min_ind;				//last hop												// this value remains -1 for the neighbors of current node 
				}
			}
		}

		for (j = 0; j < N_NODE; j++)
		{
			if (trans[j] == -2)
				printf("%d		x\n", j + 1);
			else
			{
				k = j;
				while (trans[k] != -1)					//find the source
					k = trans[k];
				printf("%d		%d\n", j + 1, k + 1);
			}
		}
		printf("-----------------------\n\n");
	}
	system("pause");
}

