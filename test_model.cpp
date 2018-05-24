#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include "node.h"
#include "layer.h"
#include "matrix.h"

using namespace std;

const int maxn = 1000;
const int maxm = 60000;
const int maxt = 30000;
const int maxl = 10;
int m_data, m_test;
int mini_batch = 100;
double in_data[maxm][maxn];
double out_data[maxm][maxn] = {0};
double in_test[maxt][maxn];
double out_test[maxt][maxn];
int n_theta;
int n_layer;
int st,en;
int layer_size[maxl];
Layer layers[maxl];
Matrix theta[maxl];
int kind[maxl];

void forth_propagation(double * in_value, int feature) {
	Matrix in_mat(feature, 1, in_value);
	for (int i = 0; i < n_layer; ++i) {
		layers[i].set_z_value(in_mat);
		Matrix out_mat = layers[i].make_out_matrix();
		if (i < n_layer - 1) in_mat = theta[i] * out_mat;
	}
}

int main() {
	ifstream fin;
	fin.open("data_new.csv");
	fin>>m_data;
	fin>>st;
	fin>>en;
	int figure;
	for (int i = 0; i < m_data;++i) {
		fin>>figure;
		for (int j = 0; j < st; ++j) {
			fin>>in_data[i][j];
		}
		out_data[i][figure] = true;
	}
	fin.close();
	fin.open("model.in");
	fin>>n_layer; n_theta = n_layer - 1;
	fin>>kind[0];
	layer_size[0] = st; layer_size[n_layer - 1] = en; 
	for (int i = 1; i < n_layer - 1; ++i) {
		fin>>layer_size[i]>>kind[i];
	}
	fin>>kind[n_layer - 1];
	fin.close();
	/*fin.open("test.in");
	fin>>m_test;
	for (int i = 0; i < m_test;++i) {
		for (int j = 0; j < st; ++j) {
			fin>>in_test[i][j];
		}
		for (int j = 0; j < en; ++j) {
			fin>>out_test[i][j];
		}
	}
	fin.close();*/
	fin.open("model.out");
	for (int i = 0; i < n_layer; ++i) {
		if (kind[i] == 0)
			layers[i] = Layer(i, layer_size[i], sigmod);
		if (kind[i] == 1)
			layers[i] = Layer(i, layer_size[i], ReLU);
		if (kind[i] == 2)
			layers[i] = Layer(i, layer_size[i], exp);
		if (i < n_theta) {
			int size = layer_size[i + 1] * (layer_size[i] + 1);
			double val[size];
			for (int k = 0; k < size; ++k) {
				fin>>val[k];
			}
			theta[i] = Matrix(layer_size[i + 1], layer_size[i] + 1, val);
		}
	}
	fin.close();
	int wrong_tot = 0;
	int wrong[10]={0},size[10]={0};
	int wrong_pattern[10][10] = {0};
	for (int i = 0; i < m_data; ++i) {
		forth_propagation(in_data[i], st);
		Matrix out = layers[n_layer - 1].make_out_matrix();
		double maxn = -1;
		int maxi,ansi;
		for (int j = 0; j < en; ++j) {  
			if (out.ret(j,0) > maxn) {
				maxn = out.ret(j,0);
				maxi = j;
			}
			if (out_data[i][j] == 1)
				ansi = j;
			}
		size[ansi]++;
		if (ansi != maxi) {
			wrong_tot++;
			wrong[ansi]++;
		}
		wrong_pattern[ansi][maxi] ++;
	}
	ofstream fout;
	fout.open("result.out");
	fout<<"train :"<<endl;
	for (int i = 0;i < 10;++i) {
			fout<<"figure "<<i<<" 's right rate : "<<(1 - (wrong[i] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
			for (int j = 0;j < 10; ++j)
				fout<<i<<" -> "<<j<<" : "<<((wrong_pattern[i][j] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
			fout<<"---------------"<<endl;
		}
	fout<<"total right rate"<<" : "<<(1 - (wrong_tot * 1.0) / (m_data * 1.0)) * 100.0<<' '<<'%'<<endl;
	/*wrong_tot = 0;
	memset(wrong,0,sizeof(wrong));
	memset(size,0,sizeof(size));
	memset(wrong_pattern,0,sizeof(wrong_pattern));
	for (int i = 0; i < m_test; ++i) {
		forth_propagation(in_test[i], st);
		Matrix out = layers[n_layer - 1].make_out_matrix();
		double maxn = -1;
		int maxi,ansi;
		for (int j = 0; j < en; ++j) {  
			if (out.ret(j,0) > maxn) {
				maxn = out.ret(j,0);
				maxi = j;
			}
			if (out_test[i][j] == 1)
				ansi = j;
		}
		size[ansi]++;
		if (ansi != maxi) {
			wrong_tot++;
			wrong[ansi]++;
		}
		wrong_pattern[ansi][maxi] ++;
	}
	fout<<"test :"<<endl;
	for (int i = 0;i < 10;++i) {
			fout<<"figure "<<i<<" 's right rate : "<<(1 - (wrong[i] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
			for (int j = 0;j < 10; ++j)
				fout<<i<<" -> "<<j<<" : "<<((wrong_pattern[i][j] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
			fout<<"---------------"<<endl;
		}
	fout<<"total right rate"<<" : "<<(1 - (wrong_tot * 1.0) / (m_test * 1.0)) * 100.0<<' '<<'%'<<endl;*/
}
