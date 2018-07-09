#pragma once
#include <bitset>
#include <vector>

using namespace std;

class Predictor {
public:
	bitset<2> counter;
	// 00: 强不选择Strongly not taken
	// 01: 弱不选择Weakly not taken
	// 10: 弱选择Weakly taken
	// 11: 强选择Strongly taken
	
	Predictor() { counter[1] = 1; }

	Predictor& operator++() {
		if (!counter[0]) {
			counter[0] = 1;
		}
		else {
			if (!counter[1]) counter[1] = 1;
		}
		return *this;
	}

	Predictor& operator--() {
		if (counter[0]) {
			counter[0] = 0;
		}
		else {
			if (counter[1]) {
				counter[1] = 0;
				counter[0] = 1;
			}
		}
		return *this;
	}

	bool take() {
		if (counter[1]) return true;
		else return false;
	}

};

vector <Predictor> predictor;