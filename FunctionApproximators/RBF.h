#pragma once
#include "FunctionApproximator.h"

class RBF : public FunctionApproximator {
private:
	Matrix* w;
	Matrix* v;

	vector<double> hidden;

	//centers
	int neurons;
	double sigma;

	double RBFunction(int, vector<double>);

	double norm(vector<double>, vector<double>);

public:
	// params: input, output, hidden, alpha, sigma, kernels, file
	RBF(int, int, int, double, double, string);
	~RBF(void);

	void saveToFile();
	//void self_organization(vector<double>);

	void init_random(vector<vector<double>>);
	void init_fixed(double, double);
	void init_self_organization(vector<vector<double>>, int);

	vector<double> compute(vector<double>);
	vector<double> error(vector<double>, vector<double>);
	void adjust(vector<double>, vector<double>, vector<double>);

	void print();
};

