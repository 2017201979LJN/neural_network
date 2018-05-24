#include <cmath>
#include <cstdlib>
#include <iostream>
#include "node.h"

using namespace std;

Node::Node() {
	layer = number = 0; 
	a_value = z_value = 0.0;
	cost_value = der_value = 0.0;
	func = NULL;
}

Node::Node(int a_layer, int a_number, double (*f)(double)) {
	layer = a_layer;
	number = a_number;
	z_value = 0.0;
	func = f;
	a_value = func(z_value);
	cost_value = 0.0;
	der_value = derivative(0.0);
}

double Node::output() {
	a_value = func(z_value);
	return a_value;
}

double Node::get_cost() {
	return cost_value;
}

double Node::get_der() {
	return der_value;
}

void Node::set_z_value(double value) {
	z_value = value;
	a_value = func(z_value);
	der_value = derivative(z_value);
}

void Node::set_bias() {
	a_value = 1.0;
}

void Node::set_cost_value(double value) {
	cost_value = value;
}

Node & Node::operator = (const Node & temp) {
	if (this == &temp)
		return *this;
	layer = temp.layer;
	number = temp.number;
	a_value = temp.a_value;
	z_value = temp.z_value;
	cost_value = temp.cost_value;
	der_value = temp.der_value;
	func = temp.func;
	return *this;
}

double Node::derivative(double x) const{
	return (func(x + epsilon) - func(x)) / epsilon;
}

double sigmod(double x) {
	return 1.0 / (1.0 + exp(-x));
}

double ReLU(double x) {
	if (x > 0) return x;
	else return 0;
}
