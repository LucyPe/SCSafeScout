#include "RBF.h"

#define PATH_READ "bwapi-data/write/"
#define PATH_WRITE "bwapi-data/write/"

RBF::RBF(int i, int o, int h, double a, double s, string f) : FunctionApproximator(i, o) {
	srand(time(NULL)); 
	
	fileName = f;
	ifstream input;
	input.open(PATH_READ + fileName);

	// exists file
	if (input.is_open()) {
		input >> inputs >> outputs >> neurons >> alfa >> sigma;
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
		alfa = a;
		sigma = s;
		w = new Matrix(neurons, inputs);
		v = new Matrix(outputs, neurons);
		v->random(neurons);
		
		// kernels
		double x = 400 / neurons;
		for (int i = 0; i < w->w; i++) {
			for (int j = 0; j < w->h; j++) {
				w->set(j + i*w->h, j * x);
			}
		}		
	}
}

RBF::~RBF(void) {
	delete(w);
	delete(v);
}

void RBF::saveToFile() {
	ofstream output;
	output.open(PATH_WRITE + fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << inputs << " " << outputs << " " << neurons << " " << alfa << " " << sigma << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

vector<double> RBF::error(vector<double> d, vector<double> y) {
	vector<double> res;
	for (int i = 0; i < d.size(); i++) {
		res.push_back(d[i] - y[i]);
	}
	return res;
}

void RBF::adjust_weight(vector<double> error, Matrix *weight, vector<double> layer) {
	for (int i = 0; i < weight->h; i++) {
		for (int j = 0; j < weight->w; j++) {
			weight->set(j + i*weight->w, weight->get(j + i*weight->w) + alfa* error[i] * layer[j]);
		}
	}
}

void RBF::adjust(vector<double> input, vector<double> error) {
	adjust_weight(error, v, hidden);
}

double RBF::RBFunction(int i, vector<double> input) {
	//triangle function
	double x = 0;
	for (int j = 0; j < w->w; j++) {
		x += pow(input[j] - w->get(j + i*w->h), 2.0);
	}
	if (x > 0) x = sqrt(x);
	x = x / 400; 

	if (x < 0) {
		return 0;
	}
	return x;
}


vector<double> RBF::compute(vector<double> input) {
	hidden.resize(0);
	for (int i = 0; i < neurons; i++) {
		hidden.push_back(RBFunction(i, input));
	}
	return v->multi(hidden);
}