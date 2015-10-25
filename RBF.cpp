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
		sigma = s;
		w = new Matrix(neurons, inputs + 1);
		v = new Matrix(outputs, neurons + 1);
		w->random(inputs);
		v->random(neurons);
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
		output << inputs << " " << outputs << " " << neurons << " " << alfa << " ";
		output << w->toString() << " ";
		output << v->toString();
		output.close();
	}
}

void RBF::sigmoid(vector<double>* x) {
	for (int i = 0; i < x->size(); i++) {
		(*x)[i] = 1 / (1 + pow(exp(1.0), - (x->at(i))));
	}
}

void RBF::sigmoid_d(vector<double>* x) {
	for (int i = 0; i < x->size(); i++) {
		(*x)[i] = x->at(i) * (1 - x->at(i));
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
	input.push_back(-1); 

	Matrix *tran = v->transp();
	vector<double> back = tran->multi(error);
	sigmoid_d(&back);

	adjust_weight(error, v, hidden);
	adjust_weight(back, w, input);	
	delete(tran);
}

vector<double> RBF::compute(vector<double> input) {
	input.push_back(-1);
	hidden.resize(0);
	hidden = w->multi(input);
	sigmoid(&hidden);
	hidden.push_back(-1);
	return v->multi(hidden);
}

/*Takto nejak sa to pocita
y = suma (f(x) * w)

f(x) - je rbf funkcia. Rbf funkcia si v sebe pamata nejaky bod k ktory sa uz dalej nemeni (takzvany kernel).
Pre vstup x sa najprv vypocita vzdialenost |x-k| od bodu k. Toto funguje pre vstupny vektor x dlzky n
(vtedy k musi byt vektor tiez dlzky n a |x-k| je ich euklidovska vzdialenost). Toto nas ale nesere lebo mame
n=1. Ked uz mame vzdialenost tu potom prezenieme nejakou funkciou ktora rapidne klesa so stupajucou vzdialenostou
tu si mozes vybrat nejaku https://en.wikipedia.org/wiki/Radial_basis_function#Types ... mne sa paci 1 / (1 + x * x)
lebo sa rychlo pocita

takze realne:
f(x) = 1 / (1 + |x - k| * |x - k|)

w - su vahy ktore chceme cvicit

y = suma_{0 <= i < m} (f_{i}(x) * w_{i})

m - je pocet kernel bodov ktore mas rozhodene v priestore vsetkych vstupov. K tomu ako ich rozhadzovat
ti vela nepoviem, jak som ti povedal volam cv::kmeans ktore to za mna spravi a dalej ma to netrapi grin emoticon.
kvazi to ale chces mat v takych hodnotach aby si mala dobre pokryte hodnoty ktore ti mozu prijst na vstupe.
Takze ked vies ze vstupy mas od 0 do 10 tak chces mat v tomto rozmedzi tie body. Druhy hint je ze ak vies ze
napr medzi x=2 a x=5 sa ti rapidne meni y tak chces v tomto rozmedzi mat viac tych bodov. Posledny hint,
cim zlozitejsia funkcia tym viac kernelov chces, ja som napr mal 100 ale to bolo na 2d vstupy takze 10 az 20
by malo byt viac nez dost pre teba ... mozno aj menej. Treba si uvedomit ze kolko mas kernelov ... tolko mas
vah takze je to jak keby si si zvysovala vahy v klasickej neuronke. Jaj a posledny posledny hint ... nechces
mat prilis podobne body lebo prakticky to nijako nepomoze a bude to len akoby si mala jeden kernel so suctom
tych podobnych vah.

*/