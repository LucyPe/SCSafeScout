#pragma once
#include "FunctionApproximator.h"

class MLP : public FunctionApproximator {

private:
	Matrix* w;
	Matrix* v;

	vector<double> hidden;
	int neurons;

	// params: layer
	void sigmoid(vector<double>*);
	// params: layer, x
	void sigmoid_d(vector<double>*, vector<double>);

public:
	// params: input, output, hidden, alpha, file
	MLP(int, int, int, double, string);
	~MLP(void);

	void saveToFile();
	// params: input
	vector<double> compute(vector<double>);
	// params: target, output
	vector<double> error(vector<double>, vector<double>);
	// params: input, output, target
	void adjust(vector<double>, vector<double>, vector<double>);
};

