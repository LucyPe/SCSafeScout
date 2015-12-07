#pragma once
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <math.h>   
#include <ctime>
#include "Matrix.h"

using namespace std;

class FunctionApproximator {

protected:
	int inputs;
	int outputs;
	double alpha;
	string fileName;

	void adjust_weight(vector<double>, Matrix*, vector<double>);

public:
	FunctionApproximator(int, int, double);
	~FunctionApproximator();

	// params: input
	virtual vector<double> compute(vector<double>) { return vector<double>(); };
	// params: target, output
	virtual vector<double> error(vector<double>, vector<double>) { return vector<double>(); };
	// params: input, output, target
	virtual void adjust(vector<double>, vector<double>, vector<double>) {};

	virtual void saveToFile() {};
};

