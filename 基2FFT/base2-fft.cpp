#include "base2-fft.h"



// 计算倒序序列：递归奇偶交错排列
// newArray是临时数组，结果输出到array1中
void interleave(vector<double>& array1, vector<double>& newArray, int start_ind, int len)
{
	if (len <= 2)
		return;
	for (int i = 0; i < len / 2; i++)
	{
		newArray[start_ind + i] = array1[start_ind + i * 2];
		newArray[start_ind + i + len / 2] = array1[start_ind + i * 2 + 1];
	}

	array1 = newArray;
	// 对左右两半部分继续交错
	interleave(array1, newArray, start_ind, len / 2);
	interleave(array1, newArray, start_ind + len / 2, len / 2);

}

// 计算倒序下标序列：按位交换
// params:
//		N：序列长度
//		M：每个数用多少个二进制位表示
vector<int> bitFlipIndex(int N, int M)
{
	vector<int> ind;
	for (int i = 0; i < N; i++)
	{
		ind.push_back(i);
		for (int j = 0; j < M / 2; j++)
		{
			int bitLeft = (i >> (M - j - 1)) & 1;
			int bitRight = (i >> j) & 1;
			if (bitLeft != bitRight)
			{
				int tmp = (1 << j) + (1 << (M - j - 1));
				ind[i] = ind[i] ^ tmp;
			}
		}
	}
	return ind;
}


// 计算倒序下标序列：高位加1，逢2向右进位
// params:
//		N：序列长度
//		M：每个数用多少个二进制位表示
vector<int> bitFlipIndex_msbAdd(int N, int M)
{
	vector<int> ind(N, 0);
	for (int i = 1; i < N - 1; i++)
	{
		ind[i] = ind[i - 1];
		int weight = N / 2;
		for (int j = 0; j < M; j++)
		{
			if (ind[i] < weight)  // 不用进位
			{
				ind[i] = ind[i] + weight;
				break;
			}
			else   // 进位
			{
				ind[i] = ind[i] - weight;
				weight /= 2;   // 继续看下一位
			}
		}
	}
	ind[N - 1] = N - 1;
	return ind;
}


// 时域抽取基2FFT
// x为实序列且已经补零
vector<complex<double>> dit_fft(vector<double> x, int N, int M)
{
	vector<complex<double>> X(N);
	vector<int> ind = bitFlipIndex_msbAdd(N, M);   // 倒序
	for (int i = 0; i < N; i++)
		X[i] = complex<double>(x[ind[i]], 0);

	for (int i = 1; i <= M; i++)  // 共M级
	{
		int B = pow(2, i - 1);   // 不同的旋转因子数目 
		for (int j = 0; j < B; j++)
		{
			complex<double> w = polar(1.0, -2 * M_PI *j / B / 2);  // 旋转因子
			for (int k = 0; k < N / B / 2; k++)  // 每个旋转因子负责的蝶形的数目
			{
				int ind1 = j + 2 * B*k;
				int ind2 = ind1 + B;
				complex<double> x1 = X[ind1] + X[ind2] * w;
				complex<double> x2 = X[ind1] - X[ind2] * w;
				X[ind1] = x1;
				X[ind2] = x2;
			}

		}
	}
	return X;
}


// 频域抽取基2FFT
// x为实序列且已经补零
vector<complex<double>> dif_fft(vector<double> x, int N, int M)
{
	vector<complex<double>> X(N);
	for (int i = 0; i < N; i++)
		X[i] = complex<double>(x[i], 0);

	for (int i = M; i >= 1; i--)  // 共M级
	{
		int B = pow(2, i - 1);   // 不同的旋转因子数目 
		for (int j = 0; j < B; j++)
		{
			complex<double> w = polar(1.0, -2 * M_PI *j / B / 2);  // 旋转因子
			for (int k = 0; k < N / B / 2; k++)  // 每个旋转因子负责的蝶形的数目
			{
				int ind1 = j + 2 * B*k;
				int ind2 = ind1 + B;
				complex<double> x1 = X[ind1] + X[ind2];
				complex<double> x2 = (X[ind1] - X[ind2]) * w;
				X[ind1] = x1;
				X[ind2] = x2;
			}

		}
	}

	vector<int> ind = bitFlipIndex_msbAdd(N, M);   // 倒序
	vector<complex<double>> X1(N);
	for (int i = 0; i < N; i++)
		X1[i] = X[ind[i]];
	
	return X1;
}