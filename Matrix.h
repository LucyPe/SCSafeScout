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
	Matrix(int, int);

	void random(int x);

	int size() {
		return data.size();
	};

	double get(int i) {
		return data[i];
	};

	void set(int i, double x) {
		data[i] = x;
	};

	Matrix* transp();
	vector<double> multi(vector<double>);
	string toString();
};


