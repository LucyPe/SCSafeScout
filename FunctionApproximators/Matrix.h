#pragma once
#include <vector>   

using namespace std;

class Matrix {
private:
	vector<double> data;

public:
	int w, h;
	//params: height, width
	Matrix(int, int);
	//params: data array
	Matrix(vector<double>);

	int size();
	vector<double> getData();
	double get(int i);
	double get(int x, int y);
	void set(int i, double e);
	void set(int x, int y, double e);
	void add(int x, int y, double e);
	
	vector<double> getRow(int);
	void setRow(int, vector<double>);

	void random(int x);
	Matrix* transp(bool);
	vector<double> multi(vector<double>);
	string toString();
};


