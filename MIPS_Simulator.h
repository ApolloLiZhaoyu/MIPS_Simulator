#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include "Parser.h"
#include "Pipeline.h"
#include "TokenScanner.h"

#define Max_Size 4 * 1024 * 1024

using namespace std;

extern char *mem;
extern int mem_pos;
extern map <string, int> op_num;

class MIPS_Simulator_Class {
private:

	vector <string> codeline;
	bool state; // 0 : .data; 1 : .text

public:
	MIPS_Simulator_Class() {
		state = 0;
	}

	~MIPS_Simulator_Class() { }

	void execute(fstream &code) {
		read(code);
		pipeline();
	}

	void interprete_data(string &str) {
		size_t pos = 0;
		size_t len = str.length();

		string token = get_phrase(str, pos);
		if (token == "") return;
		else if (token[0] == '#') return;
		else if (str[pos - 1] == ':') {
			label[token] = mem_pos;
			return;
		}
		else if (token == ".data") {
			return;
		}
		else if (token == ".text") {
			state = 1;
			return;
		}
		else if (token == ".align") {
			int n = get_num(str, pos);
			int pow = 1 << n;
			int k = ceil( (double) mem_pos / pow);
			mem_pos = k * pow;
			return;
		}
		else if (token == ".ascii") {
			string s = get_string(str, pos);
			for (size_t i = 0; i < s.length(); i++) {
				mem[mem_pos++] = s[i];
			}
			return;
		}
		else if (token == ".asciiz") {
			string s = get_string(str, pos);
			for (size_t i = 0; i < s.length(); i++) {
				mem[mem_pos++] = s[i];
			}
			mem[mem_pos++] = '\0';
			return;
		}
		else if (token == ".byte") {
			while (pos < str.length()) {
				string s = get_string(str, pos);
				mem[mem_pos++] = s[0];
			}
			return;
		}
		else if (token == ".half") {
			while (pos < str.length()) {
				short n = get_short(str, pos);
				char c[2];
				c[0] = mem[mem_pos++] = (char)n;
				c[1] = mem[mem_pos++] = (char)(n >> 8);
				/*char* c = reinterpret_cast<char*> (&n);
				mem[mem_pos++] = c[0];
				mem[mem_pos++] = c[1];*/
			}
			return;
		}
		else if (token == ".word") {
			while (pos < str.length()) {
				int n = get_num(str, pos);

				mem[mem_pos++] = (char)n;
				mem[mem_pos++] = (char)(n >> 8);
				mem[mem_pos++] = (char)(n >> 16);
				mem[mem_pos++] = (char)(n >> 24);

				/*char* c = reinterpret_cast<char*> (&n);
				mem[mem_pos++] = c[0];
				mem[mem_pos++] = c[1];
				mem[mem_pos++] = c[2];
				mem[mem_pos++] = c[3];*/
			}
			//cout << '\n';
			return;
		}
		else if (token == ".space") {

			int n = get_num(str, pos);
			mem_pos += n;
			return;
		}
		else {
			cerr << "Wrong Expression!!!" << '\n';
		}
	}

	bool interprete_text(string &str) {
		size_t pos = 0;
		size_t len = str.length();

		string token = get_phrase(str, pos);

		if (token == "") return false;
		else if (token[0] == '#') return false;
		else if (str[pos - 1] == ':') {

			label[token] = codeline.size();
			return false;
		}
		else if (token == ".text") {
			return false;
		}
		else if (token == ".data") {
			state = 0;
			return false;
		}
		else return true;
	}

	void read(fstream &code) {

		string tokenline;

		while (getline(code, tokenline)) {

			if (state == 0) {
				interprete_data(tokenline);
			}
			else {
				if (interprete_text(tokenline)) {
					codeline.push_back(tokenline);
				}
			}
		}
	}

	void pipeline() {

		//for (auto x : codeline) cout << x << '\n';

		//int line = 1;

		for (auto x : codeline) {

			TokenScanner nowline;

			size_t pos = 0;

			string op = get_phrase(x, pos);

			string r1 = get_phrase(x, pos);
			string r2 = get_phrase(x, pos);
			string r3 = get_phrase(x, pos);
			
			if ((op == "mul" || op == "mulu" || op == "div" || op == "divu") && r3 == "") {
				op = 'r' + op;
			}

			nowline.op = op_num[op];

			//cout << "line " << line << endl;
			//cout << "op " << op << endl;
			//line++;

			if (nowline.op >= 1 && nowline.op <= 13) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = regname[r2];
				nowline.expstate[1] = isreg;
				if (isregname(r3)) {
					nowline.num[2] = regname[r3];
					nowline.expstate[2] = isreg;
				}
				else {
					nowline.num[2] = stoi(r3);
					nowline.expstate[2] = isimm;
				}
			}
			else if (nowline.op >= 14 && nowline.op <= 15) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = regname[r2];
				nowline.expstate[1] = isreg;
				nowline.expstate[2] = none;
			}
			else if (nowline.op == 16) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = stoi(r2);
				nowline.expstate[1] = isimm;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 17 && nowline.op <= 22) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = regname[r2];
				nowline.expstate[1] = isreg;
				if (isregname(r3)) {
					nowline.num[2] = regname[r3];
					nowline.expstate[2] = isreg;
				}
				else {
					nowline.num[2] = stoi(r3);
					nowline.expstate[2] = isimm;
				}
			}
			else if (nowline.op >= 23 && nowline.op <= 28) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				if (isregname(r2)) {
					nowline.num[1] = regname[r2];
					nowline.expstate[1] = isreg;
				}
				else {
					nowline.num[1] = stoi(r2);
					nowline.expstate[1] = isimm;
				}
				nowline.num[2] = label[r3];
				nowline.expstate[2] = islab;
			}
			else if (nowline.op >= 29 && nowline.op <= 34) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = label[r2];
				nowline.expstate[1] = islab;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 35 && nowline.op <= 37) {
				nowline.num[0] = label[r1];
				nowline.expstate[0] = islab;
				nowline.expstate[1] = none;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 38 && nowline.op <= 39) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.expstate[1] = none;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 40 && nowline.op <= 43) {
				nowline.num[0] = regname[r1];
				if (haveop(r2)) {
					nowline.offset = getoffset(r2);
				}
				if (isregname(r2)) {
					nowline.num[1] = regname[getregname(r2)];
					nowline.expstate[1] = isreg;
				}
				else {
					nowline.num[1] = label[r2];
					nowline.expstate[1] = islab;
				}
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 44 && nowline.op <= 46) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				if (haveop(r2)) {
					nowline.offset = getoffset(r2);
				}
				if (isregname(r2)) {
					nowline.num[1] = regname[getregname(r2)];
					nowline.expstate[1] = isreg;
				}
				else {
					nowline.num[1] = label[r2];
					nowline.expstate[1] = islab;
				}
				nowline.expstate[2] = none;
			}
			else if (nowline.op == 47) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.num[1] = regname[r2];
				nowline.expstate[1] = isreg;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 48 && nowline.op <= 49) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				nowline.expstate[1] = none;
				nowline.expstate[2] = none;
			}
			else if (nowline.op == 50) {
				nowline.expstate[0] = none;
				nowline.expstate[1] = none;
				nowline.expstate[2] = none;
			}
			else if (nowline.op == 51) {
				nowline.expstate[0] = none;
				nowline.expstate[1] = none;
				nowline.expstate[2] = none;
			}
			else if (nowline.op >= 52 && nowline.op <= 55) {
				nowline.num[0] = regname[r1];
				nowline.expstate[0] = isreg;
				if (isregname(r2)) {
					nowline.num[1] = regname[r2];
					nowline.expstate[1] = isreg;
				}
				else {
					nowline.num[1] = stoi(r2);
					nowline.expstate[1] = isimm;
				}
				nowline.expstate[2] = none;
			}

			expline.push_back(nowline);

		}

		//for (auto x : expline) cout << x << endl;

		Pipeline_Class pipeline;

		reg[34].data = label["main"];

		while (pipeline.state) {
			//cout << reg[34].data << '\n';

			pipeline.Instruction_Fetch(reg[34].data, expline[reg[34].data]);
			pipeline.Instruction_Decode_And_Data_Preparation();
			pipeline.Execution();
			pipeline.Memory_Access();
			pipeline.Write_Back();
			
			//pipeline.print();
		}
	}

	inline bool isregname(string &str) {
		for (int i = 0; i < str.length(); i++) {
			if(str[i] == '$') return true;
		}
		return false;
	}

	inline bool haveop(string &str) {
		for (int i = 0; i < str.length(); i++) {
			if(str[i] == '(') return true;
		}
		return false;
	}

	inline int getoffset(string &str) {
		bool neg = 0;
		int pos = 0, ans = 0;
		if (str[0] == '-') {
			neg = 1;
			pos++;
		}
		for (; pos < str.length(); pos++) {
			if (str[pos] == '(') break;
			else ans = ans * 10 + str[pos] - '0';
		}

		return neg == 0 ? ans : -ans;
	}

	inline string getregname(string &str) {
		string s;
		bool flag = 0;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == ')') break;
			if (flag) s += str[i];
			if (str[i] == '$') {
				flag = 1;
				s += str[i];
			}
		}
		return s;
	}

};