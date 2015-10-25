#include "Matrix.h"

Matrix::Matrix(int a, int b) {
	h = a;
	w = b;
	data.resize(w*h);
}

/* generate radnom values in matrix */
void Matrix::random(int x) {
	srand(time(NULL));
	double factor = 1 / sqrt(x);
	for (int i = 0; i < data.size(); i++) {
		data[i] = ((((double)rand() / (RAND_MAX)) * 2) - 1) * factor;
	}
}

/* convert to string */
string Matrix::toString() {
	std::ostringstream sstream;

	for (int i = 0; i < data.size(); i++) {
		sstream << data[i] << " ";
	}

	return sstream.str();
}

/* generate transparent matrix */
Matrix* Matrix::transp() {
	Matrix* tmp = new Matrix(w-1, h);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w-1; j++) {
			tmp->set(i + j*h, data[j + i*w]);
		}
	}
	return tmp;
}

/* multiply vector and matrix */
vector<double> Matrix::multi(vector<double> in) {
	vector<double> out;
	out.resize(h);
	if (w != in.size()) {
		cout << "Wrong size" << endl;
	}

	for (int i = 0; i < h; i++) {
		out[i] = 0;
		for (int k = 0; k < w; k++){
			out[i] += data[k + i*w] * in[k];
		}
	}
	return out;
}