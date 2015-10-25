#include "MLP.h"

#define PATH_READ "bwapi-data/write/"
#define PATH_WRITE "bwapi-data/write/"

MLP::MLP(int i, int o, int h, double a, string f) : FunctionApproximator(i, o) {
	srand(time(NULL)); 
	
	fileName = f;
	ifstream input;
	input.open(PATH_READ + fileName);

	// exists file
	if (input.is_open()) {
		input >> inputs >> outputs >> neurons >> alfa;
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
		alfa = a;
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
	output.open(PATH_WRITE + fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << inputs << " " << outputs << " " << neurons << " " << alfa << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

void MLP::sigmoid(vector<double>* x) {
	for (int i = 0; i < x->size(); i++) {
		(*x)[i] = 1 / (1 + pow(exp(1.0), - (x->at(i))));
	}
}

void MLP::sigmoid_d(vector<double>* x) {
	for (int i = 0; i < x->size(); i++) {
		(*x)[i] = x->at(i) * (1 - x->at(i));
	}
}

vector<double> MLP::error(vector<double> d, vector<double> y) {
	vector<double> res;
	for (int i = 0; i < d.size(); i++) {
		res.push_back(d[i] - y[i]);
	}
	return res;
}

void MLP::adjust_weight(vector<double> error, Matrix *weight, vector<double> layer) {
	for (int i = 0; i < weight->h; i++) {
		for (int j = 0; j < weight->w; j++) {
			weight->set(j + i*weight->w, weight->get(j + i*weight->w) + alfa* error[i] * layer[j]);
		}
	}
}

void MLP::adjust(vector<double> input, vector<double> error) {
	input.push_back(-1); 

	Matrix *tran = v->transp();
	vector<double> back = tran->multi(error);
	sigmoid_d(&back);

	adjust_weight(error, v, hidden);
	adjust_weight(back, w, input);	
	delete(tran);
}

vector<double> MLP::compute(vector<double> input) {
	input.push_back(-1);
	hidden.resize(0);
	hidden = w->multi(input);
	sigmoid(&hidden);
	hidden.push_back(-1);
	return v->multi(hidden);
}