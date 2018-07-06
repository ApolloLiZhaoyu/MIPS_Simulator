#pragma once
#include <string>
#include "Register.h"

using namespace std;

extern map <string, int> label;
extern map <string, int> regname;
extern struct Register reg[];

string get_phrase(string &str, size_t &pos) {
	string phr;
	while (str[pos] == ' ' || str[pos] == '\t' || str[pos] == ',') pos++;
	for (; pos < str.length(); pos++) {
		if (str[pos] == ' ' || str[pos] == '\t' || str[pos] == '\0' || str[pos] == '\n' || str[pos] == ',') break;
		else if (str[pos] == ':') continue;
		else phr += str[pos];
	}
	return phr;
}

int get_num(string &str, size_t &pos) {
	int num = 0;
	while (!(str[pos] >= '0' && str[pos] <= '9') && pos < str.length()) pos++;
	for (; pos < str.length(); pos++) {
		if (str[pos] >= '0' && str[pos] <= '9') num = num * 10 + str[pos] - '0';
		else break;
	}
	while (!(str[pos] >= '0' && str[pos] <= '9') && pos < str.length()) pos++;
	return num;
}

string get_string(string &str, size_t &pos) {
	string s;
	while (str[pos] == ' ' || str[pos] == ',' || str[pos] == '\t') pos++;
	bool exp; // 0 : '\''; 1 : '\"';
	if (str[pos] == '\'') {
		exp = false;
		pos++;
	}
	else if (str[pos] == '\"') {
		exp = true;
		pos++;
	}
	for (; pos < str.length(); pos++) {
		if (!exp && str[pos] == '\'') {
			pos++;
			break;
		}
		else if (exp && str[pos] == '\"') {
			pos++;
			break;
		}
		else if (str[pos] == '\\') {
			pos++;
			switch (str[pos]) {
			case 'n' :
				s += '\n';
				break;
			case 't' :
				s += '\t';
				break;
			}
		}
		else s += str[pos];
	}
	return s;
}

short get_short(string &str, size_t &pos) {
	short num = 0;
	while (!(str[pos] >= '0' && str[pos] <= '9') && pos < str.length()) pos++;
	for (; pos < str.length(); pos++) {
		if (str[pos] >= '0' && str[pos] <= '9') num = num * 10 + str[pos] - '0';
		else break;
	}
	while (!(str[pos] >= '0' && str[pos] <= '9') && pos < str.length()) pos++;
	return num;
}

int get_address(string &str) {

	bool flag = 0, neg = 0, isreg = 0;
	int offset = 0;
	int pos = 0;
	string address;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '(') {
			flag = 1;
		}
		if (str[i] == '$') {
			isreg = 1;
		}
	}
	if (!flag) {
		if (isreg) return reg[regname[str]].data;
		else return label[str];
	}
	else {
		if (isreg) {
			if (str[0] == '-') {
				neg = 1;
				pos++;
			}
			for (; pos < str.length(); pos++) {
				if (str[pos] == '(') break;
				else offset = offset * 10 + str[pos] - '0';
			}
			pos++;
			for (; pos < str.length(); pos++) {
				if (str[pos] == ')') break;
				else address += str[pos];
			}
			if (neg) offset = -offset;

			return reg[regname[address]].data + offset;
		}
		else {
			if (str[0] == '-') {
				neg = 1;
				pos++;
			}
			for (; pos < str.length(); pos++) {
				if (str[pos] == '(') break;
				else offset = offset * 10 + str[pos] - '0';
			}
			pos++;
			for (; pos < str.length(); pos++) {
				if (str[pos] == ')') break;
				else address += str[pos];
			}
			if (neg) offset = -offset;

			return label[address] + offset;
		}
	}
}