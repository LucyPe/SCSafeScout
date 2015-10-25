#pragma once
#include "Matrix.h"
#include "FunctionApproximator.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <math.h>   
#include <ctime>

using namespace std;

class RBF : public FunctionApproximator {
private:
	Matrix *w;
	Matrix *v;

	vector<double> hidden;

	//int inputs;
	//int outputs;
	int neurons;
	double alfa;
	double sigma;

	void adjust_weight(vector<double>, Matrix*, vector<double>);
	void sigmoid(vector<double>*);
	void sigmoid_d(vector<double>*);

public:
	// params: input, output, hidden, alpha, sigma, file
	RBF(int, int, int, double, double, string);
	~RBF(void);

	void saveToFile();
	vector<double> compute(vector<double>);
	vector<double> error(vector<double>, vector<double>);
	void adjust(vector<double>, vector<double>);	
};

