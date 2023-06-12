#define _USE_MATH_DEFINES
#include <math.h>
//#include <iostream>
#include <vector>
#include <complex>

using namespace std;

vector<complex<double>> dit_fft(vector<double> x, int N, int M);
vector<complex<double>> dif_fft(vector<double> x, int N, int M);