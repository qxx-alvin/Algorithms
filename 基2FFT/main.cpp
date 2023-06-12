#include <iostream>
#include "base2-fft.h"

using namespace std;


void main()
{
	int N0 = 0;
	cout << "输入序列长度：";
	cin >> N0;
	vector<double> x(N0);
	cout << "输入序列:";
	//double tmp;
	for (int i = 0; i < N0; i++) {
		//cin >> x[i];
		x[i] = 1;
	}


	int N;
	cout << "输入DFT变换点数:";
	cin >> N;
	int M = log2(N);
	// 序列补零
	if (N <= N0) {
		cout << "DFT点数需要大于序列长度" << endl;
		return;
	}
	else {
		for (int i = N0; i < N; i++)
			x.push_back(0);
	}

	// FFT
	vector<complex<double>> X = dif_fft(x, N, M);

	//打印
	for (int i = 0; i < N; i++)
		cout << X[i] << " " << endl;

	system("pause");
}