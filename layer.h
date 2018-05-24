#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

#include "node.h"
#include "matrix.h"

class Layer{
	private :
		int position;
		int size;
		Node nodes[1000];
	public : 
		Layer();
		Layer(int a_position, int a_size, double (*a_func) (double));
		Layer(const Layer & temp);
		~Layer();
		Layer & operator = (const Layer & temp);		
		
		void set_z_value(Matrix & line);
		void set_cost_value(Matrix & line);
		Matrix make_out_matrix();
		Matrix make_cost_matrix();
		Matrix make_der_matrix();
};

#endif
