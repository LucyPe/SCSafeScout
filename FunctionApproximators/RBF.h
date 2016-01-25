#pragma once
#include "FunctionApproximator.h"

class RBF : public FunctionApproximator {
private:
	Matrix* w;
	Matrix* v;
	vector<double> radius;

	vector<double> hidden;

	//centers
	int neurons;
	double sigma;

	double norm(vector<double>, vector<double>);

	double RBFunction(vector<double>, vector<double>);
	double triangleRBF(double);

public:
	// params: input, output, kernels, alpha, sigma, file
	RBF(int, int, int, double, double, string);
	~RBF(void);

	void saveToFile();

	void init_random(vector<vector<double>>);
	void init_fixed(double, double);
	void init_self_organization(vector<vector<double>>, int);

	// params: input
	vector<double> compute(vector<double>);
	// params: target, output
	vector<double> error(vector<double>, vector<double>);
	// params: input, output, target
	void adjust(vector<double>, vector<double>, vector<double>);


	/*Debug functions*/
	void print();
};

