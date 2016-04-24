#pragma once
#include <vector>

using namespace std;

class RBF {
private:
	vector<double> w;
	vector<double> v;

	vector<double> hidden;

	double alpha;
	string fileName;

	int centers;
	double sigma;
	double radius;

	double RBFunction(double, double);
	double triangleRBF(double);

public:
	// params: kernels, alpha, sigma, center_start, cener_radius, file
	RBF(int, double, double, double, double, string);
	~RBF(void);

	void saveToFile();

	// params: input
	double compute(double);
	// params: target, output
	double error(double, double);
	// params: input, output, target
	void adjust(double, double);
};

