#pragma once
#include <vector>   

using namespace std;

class Matrix {
private:
	vector<double> data;

public:
	// width, height
	int w, h;
	//params: height, width
	Matrix(int, int);
	//params: data array
	Matrix(vector<double>);

	// returns size of of data array
	int size();
	// returns data array
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


