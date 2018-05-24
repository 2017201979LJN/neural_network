#include <cstdlib>
#include <cstring>
#include <iostream>
#include "matrix.h"

using namespace std;

int Matrix::index(int i, int j) const{
	return i * line + j;
}

Matrix::Matrix() {
	line = row = 0;
}

Matrix::Matrix(int row_, int line_) {
	row = row_;
	line = line_;
	for (int i = 0;i < row;++i)
		for (int j = 0;j < line;++j)
			matrix[index(i,j)] = 0.0;
}

Matrix::Matrix(int row_, int line_, double * mat) {
	row = row_;
	line = line_;
	for (int i = 0;i < row;++i)
		for (int j = 0;j < line;++j)
			matrix[index(i,j)] = mat[index(i,j)];
}

Matrix::Matrix(const Matrix & mat) {
	row = mat.row;
	line = mat.line;
	for (int i = 0;i < row;++i)
		for (int j = 0;j < line;++j)
			matrix[index(i,j)] = mat.matrix[mat.index(i,j)];
}

Matrix::~Matrix() {
}

Matrix Matrix::T() const{
	Matrix T_matrix = Matrix(line, row);
	for (int i = 0;i < row;++i)
		for (int j = 0;j < line;++j)
			T_matrix.matrix[T_matrix.index(j,i)] = matrix[index(i,j)];
	return T_matrix;
}

double Matrix::ret(int i, int j) const{
	return matrix[index(i,j)];
}

void Matrix::clear() {
	for (int i = 0;i < row;++i) 
		for (int j = 0;j < line;++j)
			matrix[index(i,j)] = 0.0;
}

void Matrix::show(ostream & os) const {
	for (int i = 0;i < row;++i) {
		for (int j = 0;j < line;++j)
			os<<matrix[index(i,j)]<<' ';
		os<<endl;
	}
}

Matrix & Matrix::operator = (const Matrix & mat) {
	if (this == &mat) 
		return *this;
	row = mat.row;
	line = mat.line;
	for (int i = 0;i < row;++i)
		for (int j = 0;j < line;++j)
			matrix[index(i,j)] = mat.matrix[mat.index(i,j)];
	return *this;
}

Matrix Matrix::operator + (const Matrix & mat) const {
	Matrix c = Matrix(row, line);
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < line; ++j)
			c.matrix[c.index(i,j)] = matrix[index(i,j)] + mat.matrix[mat.index(i,j)];
	return c;
}

Matrix Matrix::operator - (const Matrix & mat) const {
	Matrix c = Matrix(row, line);
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < line; ++j)
			c.matrix[c.index(i,j)] = matrix[index(i,j)] - mat.matrix[mat.index(i,j)];
	return c;
}

Matrix operator * (const Matrix & a, const Matrix & b) {
	Matrix c = Matrix(a.row, b.line);
	for (int i = 0; i < a.row;++i)
		for (int k = 0; k < a.line;++k) {
			double r = a.matrix[a.index(i,k)]; 
			for (int j = 0; j < b.line; ++j)
				c.matrix[c.index(i,j)] += b.matrix[b.index(k,j)] * r;  
		}
	return c;
}

Matrix operator ^ (const Matrix & a, const Matrix & b) {
	Matrix c =  Matrix(1,b.line);
	for (int i = 0; i < b.line; ++i)
		c.matrix[c.index(0,i)] = a.matrix[a.index(0,i)] * b.matrix[b.index(0,i)];
	return c;
}

Matrix operator * (double lambda, const Matrix & a) {
	Matrix b = Matrix(a.row,a.line);
	for (int i = 0; i < a.row; ++i)
		for (int j = 0; j < a.line; ++j)
			b.matrix[b.index(i,j)] = lambda * a.matrix[a.index(i,j)];
	return b;
}
