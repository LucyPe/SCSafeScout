#pragma once
#include "Matrix.h"
#include <string>
#include <sstream> 
#include <iostream>
#include <time.h>  
#include <math.h> 
#include <iomanip>

Matrix::Matrix(int a, int b) {
	h = a;
	w = b;
	data.resize(w*h, 0);
}

Matrix::Matrix(vector<double> d) {
	h = 1;
	w = d.size();
	data = d;
}

int Matrix::size() {
	return data.size();
};

vector<double> Matrix::getData() {
	return data;
};

double Matrix::get(int i) {
	return data[i];
};


double Matrix::get(int x, int y) {
	return data[y*w + x];
};

void Matrix::add(int x, int y, double e) {
	data[y*w + x] += e;
};

void Matrix::set(int x, int y, double e) {
	data[y*w + x] = e;
};

void Matrix::set(int i, double e) {
	data[i] = e;
};

vector<double> Matrix::getRow(int r) {
	vector<double> row;
	for (int i = 0; i < w; i++) {
		row.push_back(data[r*w + i]);
	}
	return row;
}

void Matrix::setRow(int r, vector<double> input) {
	if (w == input.size()) {
		for (int i = 0; i < w; i++) {
			data[r*w + i] = input[i];
		}
	}
}

/* generate random values in matrix */
void Matrix::random(int x) {
	double factor = 1 / sqrt(x);
	for (int i = 0; i < data.size(); i++) {
		data[i] = (((static_cast<double>(rand()) / (RAND_MAX)) * 2) - 1) * factor;
	}
}

/* convert to string */
string Matrix::toString() {
	std::ostringstream sstream;
	sstream << std::setprecision(10);

	for (int i = 0; i < data.size(); i++) {
		sstream << data[i] << " ";
	}

	return sstream.str();
}

/* transpose matrix */
Matrix* Matrix::transp(bool s) {
	int newW = w;
	if (s) newW = w - 1;
	Matrix* tmp = new Matrix(newW, h);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < newW; j++) {
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
		cout << "Wrong size " << w << " " << in.size() << " " << h << endl;
		return out;
	}

	for (int i = 0; i < h; i++) {
		out[i] = 0;
		for (int k = 0; k < w; k++){
			out[i] += data[k + i*w] * in[k];
		}
	}
	return out;
}