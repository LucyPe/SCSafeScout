#pragma once
#include "RBF.h"
#include "../Utility.h"
#include "../Const.h"
#include <set>
#include <string>

RBF::RBF(int i, int o, int h, double a, double s, double start, double radius, string f, bool zero) : FunctionApproximator(i, o, a, f) {
	ifstream input;
	input.open(fileName);

	// exists file
	if (input.is_open()) {
		input >> inputs >> outputs >> neurons >> alpha >> sigma;
		w = new Matrix(neurons, inputs);
		for (int i = 0; i < w->size(); i++) {
			double x;
			input >> x;
			w->set(i, x);
		}
		v = new Matrix(outputs, neurons);
		for (int i = 0; i < v->size(); i++) {
			double x;
			input >> x;
			v->set(i, x);
		}
		input.close();
	}
	// new FA
	else {
		neurons = h;
		sigma = s;
		w = new Matrix(neurons, inputs);
		v = new Matrix(outputs, neurons);

		if (!zero) v->random(neurons);

		init_fixed(start, radius);
	}
}

RBF::~RBF(void) {
}

double RBF::norm(vector<double> x, vector<double> y) {
	double res = 0;
	for (unsigned int i = 0; i < x.size(); i++) {
		res += (x[i] - y[i]) * (x[i] - y[i]);
	}
	return sqrt(res);
}

void RBF::init_fixed(double start, double radius) {
	for (int i = 0; i < w->h; i++) {
		for (int j = 0; j < w->w; j++) {
			w->set(j, i, start + radius * i);
		}
	}
	Utility::printToFile("bwapi-data/write/gg.txt", w->toString());
	Utility::printToFile("bwapi-data/write/gg.txt", v->toString());
}

/*
void RBF::init_random(vector<vector<double>> data) {
	std::set<int> randoms;
	if (data.size() >= (size_t)neurons) {
		for (int i = 0; i < neurons; i++) {
			int x;
			//do {
				x = rand() % data.size();
				cout << x << endl;
			//} while (randoms.find(x) == randoms.end());
			randoms.insert(x);
			for (int j = 0; j < inputs; j++) {
				cout << data[x][j] << ", ";
			}
			cout << endl;

			w->setRow(i, data[x]);
			vector<double> row = w->getRow(i);

			for (int j = 0; j < inputs; j++) {
				cout << row[j] << ", ";
			}
			cout << endl;

			//cout << i << endl;
		}
	}
}

void RBF::init_self_organization(vector<vector<double>> data, int ep_total) {
	init_random(data);
	for (int ep = 0; ep < ep_total; ep++) {
		for (size_t k = 0; k < data.size(); k++) {
			//find winner
			int min_i = 0;
			double min = norm(data[k], w->getRow(0));
			//cout << "min:" << min << " " << min_i << endl;

			for (int i = 1; i < neurons; i++) {
				double newmin = norm(data[k], w->getRow(i));
				
				//cout << "newmin:" << min << " " << newmin << endl;
				if (newmin < min) {
					min_i = i;
					min = newmin;
				}
				//cout << "min:" << min << " " << min_i<< endl;
			}
			//cout << endl;
			// update kernel
			vector<double> kernel = w->getRow(min_i);

			for (int i = 0; i < inputs; i++) {
				kernel[i] += alpha * (data[k][i] - kernel[i]);
			}
			w->setRow(min_i, kernel);
		}

		for (int j = 0; j < w->size(); j++) {
			cout << w->get(j) << ", ";
		}
		cout << endl;
		
	}
}
*/

void RBF::saveToFile() {
	ofstream output;
	output.open(fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << inputs << " " << outputs << " " << neurons << " " << alpha << " " << sigma << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

vector<double> RBF::error(vector<double> target, vector<double> output) {
	vector<double> res;
	for (unsigned int i = 0; i < target.size(); i++) {
		res.push_back(target[i] - output[i]);
	}
	return res;
}

void RBF::adjust(vector<double> input, vector<double> output, vector<double> target) {
	vector<double> err = error(target, output);
	adjust_weight(err, v, hidden);
	//Utility::printToFile(Const::PATH_WRITE + (string) "aa.txt", "adjust");
	//Utility::printToFile("bwapi-data/write/gg.txt", "E:"  + std::to_string(err[0]) + " V: " + v->toString());
}

double RBF::RBFunction(vector<double> input, vector<double> center) {
	double x = norm(input, center);
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

vector<double> RBF::compute(vector<double> input) {
	hidden.clear();
	for (int i = 0; i < neurons; i++) {
		hidden.push_back(RBFunction(input, w->getRow(i)));
	}
	return v->multi(hidden);
}