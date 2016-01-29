#pragma once
#include "FunctionApproximator.h"

FunctionApproximator::FunctionApproximator(int i, int o, double a, string file) {
	inputs = i;
	outputs = o;
	alpha = a;
	fileName = file;
}

FunctionApproximator::~FunctionApproximator() {}

void FunctionApproximator::adjust_weight(vector<double> error, Matrix* weight, vector<double> layer) {
	for (int i = 0; i < weight->h; i++) {
		for (int j = 0; j < weight->w; j++) {
			weight->set(j + i*weight->w, weight->get(j + i*weight->w) + alpha* error[i] * layer[j]);
		}
	}
}

void FunctionApproximator::adjustToZero(vector<double> input, vector<double> output) {
	double old_alpha = alpha;
	alpha = 1.0;

	vector<double> target = vector<double>();
	for (int i = 0; i <= output.size(); i++) {
		target.push_back(-output[0]);
	}
	adjust(input, output, target);
	alpha = old_alpha;
}