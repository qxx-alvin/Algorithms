#include <iostream>
#include "base2-fft.h"

using namespace std;


void main()
{
	int N0 = 0;
	cout << "�������г��ȣ�";
	cin >> N0;
	vector<double> x(N0);
	cout << "��������:";
	//double tmp;
	for (int i = 0; i < N0; i++) {
		//cin >> x[i];
		x[i] = 1;
	}


	int N;
	cout << "����DFT�任����:";
	cin >> N;
	int M = log2(N);
	// ���в���
	if (N <= N0) {
		cout << "DFT������Ҫ�������г���" << endl;
		return;
	}
	else {
		for (int i = N0; i < N; i++)
			x.push_back(0);
	}

	// FFT
	vector<complex<double>> X = dif_fft(x, N, M);

	//��ӡ
	for (int i = 0; i < N; i++)
		cout << X[i] << " " << endl;

	system("pause");
}