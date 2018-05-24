#include <cstdlib>
#include <iostream>
#include "layer.h"
#include "matrix.h"
#include "node.h"

using namespace std;

Layer::Layer() {
	position = size = 0;
}

Layer::Layer(int a_position, int a_size, double (*a_func) (double)) {
	position = a_position;
	size = a_size;
	for (int i = 0; i < size + 1; ++i)
		nodes[i] = Node(a_position, i , a_func);
}

Layer::Layer(const Layer & temp) {
	position = temp.position;
	size = temp.size;
	for (int i = 0;i < size + 1; ++i)
		nodes[i] = temp.nodes[i];
}

Layer::~Layer() {
}

Matrix Layer::make_out_matrix() {
	double val[size + 1];
	for (int i = 0;i < size; ++i)
		val[i] = nodes[i].output();
	val[size] = 1.0;
	Matrix mat(size + 1, 1, val);
	return mat;
}

Matrix Layer::make_cost_matrix() {
	double val[size];
	for (int i = 0;i < size; ++i)
		val[i] = nodes[i].get_cost();
	Matrix mat(1, size, val);
	return mat;
}

Matrix Layer::make_der_matrix() {
	double val[size];
	for (int i = 0;i < size; ++i)
		val[i] = nodes[i].get_der();
	Matrix mat(1, size, val);
	return mat;
}

Layer & Layer::operator = (const Layer & temp) {
	if (this == &temp) {
		return *this;
	}
	position = temp.position;
	size = temp.size;
	for (int i = 0;i < size + 1;++i)
		nodes[i] = temp.nodes[i];
	return *this;
}

void Layer::set_z_value(Matrix & line) {
	for (int i = 0;i < size; ++i)
		nodes[i].set_z_value(line.ret(i,0));
	nodes[size].set_bias();
}

void Layer::set_cost_value(Matrix & row) {
	for (int i = 0;i < size; ++i) 
		nodes[i].set_cost_value(row.ret(0,i));
}
