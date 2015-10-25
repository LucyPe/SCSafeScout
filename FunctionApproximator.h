#pragma once
#include <vector>

using namespace std;

class FunctionApproximator {

protected:
	int inputs;
	int outputs;
	string fileName;

public:
	FunctionApproximator(int, int);
	~FunctionApproximator();

	// params: input
	virtual vector<double> compute(vector<double>) { return vector<double>(); };
	// params: target, output
	virtual vector<double> error(vector<double>, vector<double>) { return vector<double>(); };
	// params: input, error
	virtual void adjust(vector<double>, vector<double>) {};

	virtual void saveToFile() {};
};

