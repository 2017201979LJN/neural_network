#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "layer.h"
#include "matrix.h"
#include "node.h"

using namespace std;

const int maxn = 1000;
const int maxm = 60000;
const int maxt = 30000;
const int maxl = 10;
int m_data, m_test;
int mini_batch = 100;
double in_data[maxm][maxn];
double out_data[maxm][maxn] = {0.0};
double in_test[maxt][maxn];
double out_test[maxt][maxn];
double alpha;
int n_theta;
int n_layer;
int layer_size[maxl];
Matrix theta[maxl];
Matrix delta[maxl];
Layer layers[maxl];
Matrix descent[maxl];
int limit;
int seed = 2333;
int st,en;
int kind[maxl];
void init(char * arg) {
	ifstream fin;
	int figure;
	fin.open(arg);
//	cout<<"input train data size:";
	fin>>m_data;
//	cout<<m_data<<endl;
//	cout<<"input in_feature number:";
	fin>>st;
//	cout<<"input out_feature number:";
	fin>>en;
//	cout<<"input train data:"<<endl;
	memset(out_data,0,sizeof(out_data));
	for (int i = 0; i < m_data;++i) {
		//fin>>figure;
		out_data[i][figure] = 1;
		for (int j = 0; j < st; ++j) {
			fin>>in_data[i][j];
		}
		for (int j = 0; j < en; ++j)
			fin>>out_data[i][j];
	}
//	cout<<"input layer number size:";
	fin.close();
	fin.open("model.in");
	fin>>n_layer; n_theta = n_layer - 1;
	fin>>kind[0];
	layer_size[0] = st; layer_size[n_layer - 1] = en; 
//	cout<<"input hidden layer size:";
	for (int i = 1; i < n_layer - 1; ++i) {
		fin>>layer_size[i]>>kind[i];
	}
	fin>>kind[n_layer - 1];
	fin>>limit;
	fin>>alpha;
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
	}*/
	fin.open("test_new.csv");
	fin>>m_test;
	for (int i = 0; i < m_test;++i) {
		for (int j = 0; j < st; ++j) {
			fin>>in_test[i][j];
		}
	}
}

void forth_propagation(double * in_value, int feature) {
	Matrix in_mat(feature, 1, in_value);
	for (int i = 0; i < n_layer; ++i) {
		layers[i].set_z_value(in_mat);
		Matrix out_mat = layers[i].make_out_matrix();
		if (i < n_layer - 1) in_mat = theta[i] * out_mat;
	}
}

void back_propagation(double * cost_value, int feature) {
	Matrix cost_mat(1, feature, cost_value);
	Matrix der_mat = layers[n_layer - 1].make_der_matrix();
	cost_mat = cost_mat ^ der_mat;
	for (int i = n_layer - 1; i >= 1; --i) {
		if (i > 0) {
			der_mat = layers[i - 1].make_der_matrix();
			Matrix out_mat = layers[i - 1].make_out_matrix();
			descent[i - 1] = descent[i - 1] + (1.0/(mini_batch * 1.0)) * ((out_mat * cost_mat).T());
			cost_mat = (cost_mat * theta[i -1]) ^ (der_mat);
		}
	}
}

double one_process(double * in_data, double * out_data,int in_feature, int out_feature) {
	forth_propagation(in_data, in_feature);
	double diff[out_feature];
	double COST = 0.0;
	Matrix out = layers[n_layer - 1].make_out_matrix();
	for (int i = 0;i < out_feature; ++i) {
		//cout<<i<<":"<<out_data[i]<<"!"<<endl;
		diff[i] = (out.ret(i, 0) - out_data[i]);
		COST = COST + diff[i] * diff[i];
	}
	back_propagation(diff, out_feature);
	return COST;
}

double Rand() {
	return (1.0*((rand() % 10000) - 5000))/50000.0;
}

void test(int step) {
	ofstream fout;
	fout.open("result.out",ios::app);
	
	fout<<step<<":"<<endl;
	int wrong_tot = 0;
	int wrong[10]={0},size[10]={0};
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
	}
	fout<<"train :"<<endl;
	for (int i = 0;i < 10;++i)
		fout<<"figure "<<i<<" 's right rate : "<<(1 - (wrong[i] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
	fout<<"total right rate"<<" : "<<(1 - (wrong_tot * 1.0) / (m_data * 1.0)) * 100.0<<' '<<'%'<<endl;
	/*wrong_tot = 0;
	memset(wrong,0,sizeof(wrong));
	memset(size,0,sizeof(size));
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
	}
	fout<<"test :"<<endl;
	for (int i = 0;i < 10;++i)
		fout<<"figure "<<i<<" 's right rate : "<<(1 - (wrong[i] * 1.0) / (size[i] * 1.0)) * 100.0<<' '<<'%'<<endl;
	fout<<"total right rate"<<" : "<<(1 - (wrong_tot * 1.0) / (m_test * 1.0)) * 100.0<<' '<<'%'<<endl;
	*/
	fout.close();
	string file = "result";
	string number = to_string(step);
	file = file + "_" + number + ".csv";
	fout.open(file);
	fout<<"ImageId,Label"<<endl;
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
		}
		fout<<i + 1<<","<<maxi<<endl;
	}
	fout.close();
}

int main(int argv, char ** argc) {
	init(argc[1]);
	srand(seed);
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
			int last = 0;
			int sigma = 0.0;
			for (int k = 0; k < size; ++k) {
				val[k] = Rand();
			}
			theta[i] = Matrix(layer_size[i + 1], layer_size[i] + 1, val);
			delta[i] = Matrix(layer_size[i + 1], layer_size[i] + 1);
			descent[i] = Matrix(layer_size[i + 1], layer_size[i] + 1);
		}
	}
	int batch[m_data];
	for (int i = 0;i < m_data;++ i)
		batch[i] = i;
	random_shuffle(batch,batch+m_data);
	int step = 0;
	int now = 0;
	while (step < limit) {
		for (int i = 0;i < n_theta; ++i)
			descent[i].clear();
		double co = 0.0;
		for (int i = now;i < now + mini_batch;++i) {
			//cout<<batch[i]<<endl;
			//for (int j = 0;j < en; ++j)
			//	cout<<out_data[batch[i]][j];
			//cout<<endl;
			co = co + one_process(in_data[batch[i]], out_data[batch[i]], st, en);
		}
		now = (now + mini_batch) % m_data;
		step++;
		cout<<step<<" : "<<co<<endl;
		for (int i = 0;i < n_theta; ++i)
			theta[i] = theta[i] - (alpha * descent[i]);
		if ((step % 5000 == 0)) {
			test(step);
		}
	}
	/*test(limit);
	ofstream fout;
	fout.open("model.out");
	for (int i = 0; i < n_theta; ++i)
		theta[i].show(fout);
	fout.close();*/
	/*
	int wrong_tot = 0;
	for (int i = 0; i < m_test; ++i) {
		forth_propagation(in_test[i], st);
		Matrix out = layers[n_layer - 1].make_out_matrix();
		double maxn = -1;
		int maxi;
		for (int j = 0; j < en; ++j)  
			if (out.ret(j,0) > maxn) {
				maxn = out.ret(j,0);
				maxi = j;
		}
		if (out_test[i][maxi] != 1) {
			wrong_tot++;
		}
	}
	cout<<1 - (wrong_tot * 1.0) / (m_test * 1.0)<<endl;*/
}
