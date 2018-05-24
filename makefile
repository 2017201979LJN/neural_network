neural_network: neural_network.o layer.o matrix.o node.o
	g++ -o neural_network neural_network.o layer.o matrix.o node.o -O3 -std=c++11
neural_network.o: neural_network.cpp layer.h matrix.h node.h
	g++ -c neural_network.cpp -O3 -std=c++11
layer.o: layer.cpp layer.h matrix.h node.h
	g++ -c layer.cpp -O3
matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp -O3
node.o:	node.cpp node.h
	g++ -c node.cpp -O3
clean:
	rm neural_network neural_network.o layer.o matrix.o node.o
