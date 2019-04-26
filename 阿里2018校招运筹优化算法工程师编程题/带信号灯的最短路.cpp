// 带信号灯的最短路
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#define MAX_CROSS 100
#define INF 1000000

int waitTimeInCross[MAX_CROSS];
int nCross;
int nRoad;
int roadTime[MAX_CROSS][MAX_CROSS];
int minDis[MAX_CROSS][MAX_CROSS];


void main()
{
	FILE *fp = fopen("D:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\my project\\算法\\阿里巴巴2018校园招聘运筹优化算法工程师编程题\\阿里巴巴2018校园招聘运筹优化算法工程师编程题\\data.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	fscanf(fp, "%d", &nCross);
	int cross, waitT;
	for (int i = 0; i < nCross; i++)
	{
		fscanf(fp, "%d,%d", &cross, &waitT);
		waitTimeInCross[cross] = waitT;
	}

	for (int i = 0; i < nCross; i++)
		for (int j = 0; j < nCross; j++)
			roadTime[i][j] = i == j ? 0 : INF;

	fscanf(fp, "%d", &nRoad);
	int start, end, roadT;
	for (int i = 0; i < nRoad; i++)
	{
		fscanf(fp, "%d,%d,%d", &start, &end, &roadT);
		roadTime[start][end] = roadT;
		roadTime[end][start] = roadT;
	}
	fclose(fp);
	
	// modified Dijkstra
	for (int i = 0; i < nCross; i++)
	{
		// i is start cross
		int visited[MAX_CROSS] = { 0 };
		visited[i] = 1;
		// intialize
		for (int j = 0; j < nCross; j++)
		{
			minDis[i][j] = roadTime[i][j];
		}
		for (int n = 0; n < nCross - 1; n++) // nCross - 1 search
		{
			// find the min
			int curMinDis = INF;
			int curMinCross;
			for (int k = 0; k < nCross; k++)
			{
				if (!visited[k])		// find the outer set
				{
					if (minDis[i][k] < curMinDis)
					{
						curMinDis = minDis[i][k];
						curMinCross = k;
					}
				}
			}
			// curMinCross determined
			visited[curMinCross] = 1;

			// update
			for (int k = 0; k < nCross; k++)
			{
				if (!visited[k])
				{
					int wait = (minDis[i][curMinCross] % (2 * waitTimeInCross[curMinCross])) >= waitTimeInCross[curMinCross] ? 2 * waitTimeInCross[curMinCross] - minDis[i][curMinCross] % (2 * waitTimeInCross[curMinCross]) : 0;
					if (minDis[i][curMinCross] + wait + roadTime[curMinCross][k] < minDis[i][k])
					{
						minDis[i][k] = minDis[i][curMinCross] + wait + roadTime[curMinCross][k];
					}
				}
			}
		}
	}

	// output 
	for (int i = 0; i < nCross; i++)
		for (int j = 0; j < nCross; j++)
		{
			if (i != j)
				printf("%d %d %d\n", i+1, j+1, minDis[i][j]);
		}

	getchar();
}