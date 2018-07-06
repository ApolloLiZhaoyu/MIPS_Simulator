#pragma once
#include <vector>

enum ExpressionState{ none, isreg, isimm, islab };

struct TokenScanner {
	int op;
	//string r1, r2, r3;
	int num[3] = { 0 };
	int offset = 0;
	ExpressionState expstate[3];

	friend ostream& operator<< (ostream &os, TokenScanner &token) {
		cout << "op = " << token.op << endl;
		cout << token.expstate[0] << " : " << token.num[0] << endl;
		cout << token.expstate[1] << " : " << token.num[1] << endl;
		cout << token.expstate[2] << " : " << token.num[2] << endl;
		cout << "offset = " << token.offset << endl;
		return os;
	}
};

vector <TokenScanner> expline;