#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

class Node{
	private :
		int layer,number;
		double (*func)(double);
		double a_value;
		double z_value;
		double cost_value;
		double der_value;
	public :
		Node();
		Node(int a_layer,int a_number, double (*f)(double));
		double output();
		double get_cost();
		double get_der();
		void set_z_value(double value);
		void set_bias();
		void set_cost_value(double value);
		double derivative(double x) const;		
		Node & operator = (const Node & temp);
};
const static double epsilon = 1e-10; 
double sigmod(double x);
double x(double x);
double ReLU(double x);
#endif
