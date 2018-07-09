#pragma once
#include <bitset>
#include <vector>

using namespace std;

class Predictor {
public:
	bitset<2> counter;
	// 00: ǿ��ѡ��Strongly not taken
	// 01: ����ѡ��Weakly not taken
	// 10: ��ѡ��Weakly taken
	// 11: ǿѡ��Strongly taken
	
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