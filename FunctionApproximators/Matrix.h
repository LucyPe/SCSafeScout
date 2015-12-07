#pragma once
#include <string>
#include <sstream> 
#include <iostream>
#include <vector>
#include <time.h>  
#include <math.h>    

using namespace std;

class Matrix {
private:
	vector<double> data;

public:
	int w, h;
	//height, width
	Matrix(int, int);
	Matrix(vector<double>);

	int size() {
		return data.size();
	};

	vector<double> getData() {
		return data;
	};

	double get(int i) {
		return data[i];
	};


	double get(int x, int y) {
		return data[y*w + x];
	};

	void add(int x, int y, double e) {
		data[y*w + x] += e;
	};

	void set(int x, int y, double e) {
		data[y*w + x] = e;
	};

	void set(int i, double e) {
		data[i] = e;
	};

	vector<double> getRow(int);
	void setRow(int, vector<double>);
	void random(int x);
	Matrix* transp(bool);
	vector<double> multi(vector<double>);
	string toString();
};


