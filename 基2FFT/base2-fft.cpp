#include "base2-fft.h"



// ���㵹�����У��ݹ���ż��������
// newArray����ʱ���飬��������array1��
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
	// ���������벿�ּ�������
	interleave(array1, newArray, start_ind, len / 2);
	interleave(array1, newArray, start_ind + len / 2, len / 2);

}

// ���㵹���±����У���λ����
// params:
//		N�����г���
//		M��ÿ�����ö��ٸ�������λ��ʾ
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


// ���㵹���±����У���λ��1����2���ҽ�λ
// params:
//		N�����г���
//		M��ÿ�����ö��ٸ�������λ��ʾ
vector<int> bitFlipIndex_msbAdd(int N, int M)
{
	vector<int> ind(N, 0);
	for (int i = 1; i < N - 1; i++)
	{
		ind[i] = ind[i - 1];
		int weight = N / 2;
		for (int j = 0; j < M; j++)
		{
			if (ind[i] < weight)  // ���ý�λ
			{
				ind[i] = ind[i] + weight;
				break;
			}
			else   // ��λ
			{
				ind[i] = ind[i] - weight;
				weight /= 2;   // ��������һλ
			}
		}
	}
	ind[N - 1] = N - 1;
	return ind;
}


// ʱ���ȡ��2FFT
// xΪʵ�������Ѿ�����
vector<complex<double>> dit_fft(vector<double> x, int N, int M)
{
	vector<complex<double>> X(N);
	vector<int> ind = bitFlipIndex_msbAdd(N, M);   // ����
	for (int i = 0; i < N; i++)
		X[i] = complex<double>(x[ind[i]], 0);

	for (int i = 1; i <= M; i++)  // ��M��
	{
		int B = pow(2, i - 1);   // ��ͬ����ת������Ŀ 
		for (int j = 0; j < B; j++)
		{
			complex<double> w = polar(1.0, -2 * M_PI *j / B / 2);  // ��ת����
			for (int k = 0; k < N / B / 2; k++)  // ÿ����ת���Ӹ���ĵ��ε���Ŀ
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


// Ƶ���ȡ��2FFT
// xΪʵ�������Ѿ�����
vector<complex<double>> dif_fft(vector<double> x, int N, int M)
{
	vector<complex<double>> X(N);
	for (int i = 0; i < N; i++)
		X[i] = complex<double>(x[i], 0);

	for (int i = M; i >= 1; i--)  // ��M��
	{
		int B = pow(2, i - 1);   // ��ͬ����ת������Ŀ 
		for (int j = 0; j < B; j++)
		{
			complex<double> w = polar(1.0, -2 * M_PI *j / B / 2);  // ��ת����
			for (int k = 0; k < N / B / 2; k++)  // ÿ����ת���Ӹ���ĵ��ε���Ŀ
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

	vector<int> ind = bitFlipIndex_msbAdd(N, M);   // ����
	vector<complex<double>> X1(N);
	for (int i = 0; i < N; i++)
		X1[i] = X[ind[i]];
	
	return X1;
}