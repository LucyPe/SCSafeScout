#include "MLP.h"

MLP::MLP(int i, int o, int h, double a, string f) : FunctionApproximator(i, o, a) {
	srand(time(NULL)); 
	
	fileName = f;
	ifstream input;
	input.open(fileName);

	// exists file
	if (input.is_open()) {
		input >> inputs >> outputs >> neurons >> alpha;
		w = new Matrix(neurons, inputs + 1);
		for (int i = 0; i < w->size(); i++) {
			double x;
			input >> x;
			w->set(i, x);
		}
		v = new Matrix(outputs, neurons + 1);
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
		w = new Matrix(neurons, inputs + 1);
		v = new Matrix(outputs, neurons + 1);

		w->random(inputs);
		v->random(neurons);
	}
}

MLP::~MLP(void) {
	delete(w);
	delete(v);
}

void MLP::saveToFile() {
	ofstream output;
	output.open(fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << inputs << " " << outputs << " " << neurons << " " << alpha << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

void MLP::weights() {
	cout << w->toString() << endl;
	cout << v->toString() << endl;
}

void MLP::sigmoid(vector<double>* l) {
	for (int i = 0; i < l->size(); i++) {
		(*l)[i] = 1 / (1 + pow(exp(1.0), -(l->at(i))));
	}
}

void MLP::sigmoid_d(vector<double> *e, vector<double> out) {
	for (int i = 0; i < e->size(); i++) {
		(*e)[i] = (*e)[i] * out[i] * (1 - out[i]);
	}
}

/*
vector<double> MLP::error(vector<double> target, vector<double> output) {
	vector<double> err;
	err.resize(output.size());
	for (int i = 0; i < err.size(); i++) {
		double res = 0;
		if (i == target.at(0)) res = 1;
		res = (res - output[i]);
		err[i] = res;
	}
	return err;
}*/


vector<double> MLP::error(vector<double> target, vector<double> output) {
	vector<double> err;
	err.resize(output.size());
	for (int i = 0; i < err.size(); i++) {
		err[i] = (target[i] - output[i]);
	}
	return err;
}

void MLP::adjust(vector<double> input, vector<double> output, vector<double> target) {
	input.push_back(-1); 

	vector<double> e_out = error(target, output);
	sigmoid_d(&e_out, output);
	Matrix *tran = v->transp(true);
	vector<double> e_hid = tran->multi(e_out);
	sigmoid_d(&e_hid, hidden);
	adjust_weight(e_out, v, hidden);
	adjust_weight(e_hid, w, input);

	delete(tran);
}

vector<double> MLP::compute(vector<double> input) {
	input.push_back(-1);

	hidden = w->multi(input);
	sigmoid(&hidden);
	hidden.push_back(-1);

	vector<double> output = v->multi(hidden);
	sigmoid(&output);
	return output;
}