#pragma once
#include "RBF.h"
#include "../Utility.h"
#include "../Const.h"
#include <set>
#include <string>

RBF::RBF(int c, double a, double s, double start, double radius, string file) {
	ifstream input;
	fileName = file;
	input.open(fileName);

	w = vector<double>();
	v = vector<double>();

	// exists file
	if (input.is_open()) {
		input >> centers >> alpha >> sigma;
		w.resize(centers);
		v.resize(centers);
		for (int i = 0; i < w.size(); i++) {
			input >> w[i];
		}
		for (int i = 0; i < v.size(); i++) {
			input >> v[i];
		}
		input.close();
	}
	// new FA
	else {
		alpha = a;
		centers = c;
		sigma = s;
		w.resize(centers);
		v.resize(centers);
		for (int i = 0; i < w.size(); i++) {
			w[i] = start + radius * i;
		}
	}
}

RBF::~RBF(void) {}

void RBF::saveToFile() {
	ofstream output;
	output.open(fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << centers << " " << alpha << " " << sigma << " ";
		for (int i = 0; i < w.size(); i++) {
			output << w[i] << " ";
		}
		for (int i = 0; i < v.size(); i++) {
			output << v[i] << " ";
		}
		output.close();
	}
}

double RBF::error(double target, double output) {
	return (target - output);
}

void RBF::adjust(double input, double output, double target) {
	double err = error(target, output);
	for (int k = 0; k < centers; k++){
		v[k] += alpha * err * hidden[k];
	}
}

double RBF::RBFunction(double input, double center) {
	double x = sqrt((input - center) * (input - center));
	return triangleRBF(x);
	//return gaussRBF(x * sigma);
}

double gaussRBF(double dist) {
	return std::exp(-(dist * dist));
}

double RBF::triangleRBF(double dist) {
	if (dist > sigma) return 0;
	return sigma - dist;
}

double RBF::compute(double input) {
	hidden.clear();
	for (int i = 0; i < centers; i++) {
		hidden.push_back(RBFunction(input, w[i]));
	}
	// multi
	double out = 0;
	for (int k = 0; k < centers; k++){
		out += v[k] * hidden[k];
	}
	return out;
}