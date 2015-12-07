#include "RBF.h"
#include <set>

#define PATH_READ "bwapi-data/write/"
#define PATH_WRITE "bwapi-data/write/"

RBF::RBF(int i, int o, int h, double a, double s, string f) : FunctionApproximator(i, o, a) {
	srand(time(NULL)); 
	
	fileName = f;
	ifstream input;
	input.open(PATH_READ + fileName);

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

		v->random(neurons);
	}
}

RBF::~RBF(void) {
}

double RBF::norm(vector<double> x, vector<double> y) {
	/*cout << "NORM:" << endl;
	for (int j = 0; j < x.size(); j++) {
		cout << x[j] << ", ";
	}
	cout << endl;


	for (int j = 0; j < y.size(); j++) {
		cout << y[j] << ", ";
	}
	cout << endl;*/

	double res = 0;
	for (int i = 0; i < x.size(); i++) {
		res += pow(x[i] - y[i], 2);
	}
	res = sqrt(res);
	/*cout << res << endl;*/

	return res;
}

void RBF::init_random(vector<vector<double>> data) {
	std::set<int> randoms;
	if (data.size() >= neurons) {
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

void RBF::init_fixed(double radius, double max) {
	double x = max / radius;
	for (int i = 0; i < w->h; i++) {
		for (int j = 0; j < w->w; j++) {
			w->set(j, i, x * i);
		}
	}
}

void RBF::init_self_organization(vector<vector<double>> data, int ep_total) {
	init_random(data);
	for (int ep = 0; ep < ep_total; ep++) {
		for (int k = 0; k < data.size(); k++) {
			//find winner
			double min_i = 0;
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

void RBF::saveToFile() {
	ofstream output;
	output.open(PATH_WRITE + fileName, std::ofstream::out | std::ofstream::trunc);
	if (output.is_open()) {
		output << inputs << " " << outputs << " " << neurons << " " << alpha << " " << sigma << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

void RBF::print() {
	cout << w->toString() << endl;
}

vector<double> RBF::error(vector<double> d, vector<double> y) {
	vector<double> res;
	for (int i = 0; i < d.size(); i++) {
		res.push_back(d[i] - y[i]);
	}
	return res;
}

void RBF::adjust(vector<double> input, vector<double> output, vector<double> target) {
	adjust_weight(error(output, target), v, hidden);
}

double RBF::RBFunction(int i, vector<double> input) {
	//triangle function
	/*double x = 0;
	for (int j = 0; j < w.cols; j++) {
		x += pow(input[j] - w->get(j + i*w->h), 2.0);
	}
	if (x > 0) x = sqrt(x);
	x = x / 400; 

	if (x < 0) {
		return 0;
	}
	return x;*/

	return exp((-pow(norm(input,w->getRow(i)),2))/ pow(sigma, 2));

}


vector<double> RBF::compute(vector<double> input) {
	hidden.resize(0);
	for (int i = 0; i < neurons; i++) {
		hidden.push_back(RBFunction(i, input));
	}
	return v->multi(hidden);
	return vector<double>();
}