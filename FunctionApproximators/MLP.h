#pragma once
#include "FunctionApproximator.h"

class MLP : public FunctionApproximator {

private:
	Matrix* w;
	Matrix* v;

	vector<double> hidden;

	int neurons;

	void sigmoid(vector<double>*);
	void sigmoid_d(vector<double>*, vector<double>);

public:
	// params: input, output, hidden, alpha, file
	MLP(int, int, int, double, string);
	~MLP(void);

	void saveToFile();
	vector<double> compute(vector<double>);
	vector<double> error(vector<double>, vector<double>);
	void adjust(vector<double>, vector<double>, vector<double>);

	void weights();
};

