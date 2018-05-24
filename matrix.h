#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include <iostream>
#include <fstream>

class Matrix{
	private :
		int row, line;
		double matrix[10000];
		int index(int i, int j) const;
	public :
		Matrix();
		Matrix(int row_, int line_);
		Matrix(int row_, int line_, double * matrix_);
		Matrix(const Matrix & mat);
		~Matrix();
		Matrix T() const;
		double ret(int i,int j) const;
		void clear();
		void show(std::ostream & os) const;
		Matrix & operator = (const Matrix & mat);
		Matrix operator + (const Matrix & mat) const;
		Matrix operator - (const Matrix & mat) const;
		friend Matrix operator * (const Matrix & a, const Matrix & b);
		friend Matrix operator ^ (const Matrix & a, const Matrix & b);
		friend Matrix operator * (double lambda, const Matrix & a);
};

#endif
