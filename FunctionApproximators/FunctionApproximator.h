#pragma once
#include "Matrix.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class FunctionApproximator {

protected:
	int inputs;
	int outputs;
	double alpha;
	string fileName;

	//params: error, weight, layer
	void adjust_weight(vector<double>, Matrix*, vector<double>);

public:
	// params: inputs, outputs, alpha, fileName
	FunctionApproximator(int, int, double, string);
	~FunctionApproximator();

	// params: input
	virtual vector<double> compute(vector<double>) { return vector<double>(); };
	// params: target, output
	virtual vector<double> error(vector<double>, vector<double>) { return vector<double>(); };
	// params: input, output, target
	virtual void adjust(vector<double>, vector<double>, vector<double>) {};
	// params: input, output
	void adjustToZero(vector<double>, vector<double>);

	virtual void saveToFile() {};
};

