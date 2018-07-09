#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Parser.h"
#include "Register.h"
#include "TokenScanner.h"

using namespace std;

extern map <string, int> op_type;
extern map <string, int> regname;
extern map <string, int> label;
extern char *mem;
extern struct Register reg[];
extern int mem_pos;
extern vector <TokenScanner> expline;

int pipeline_state[6] = { 1, 0, 0, 0, 0, 1 }; // 0 : 未开始； 1 ： 已执行； 2 ： 停止
// pipeline_state[0] : 0 : 有分支， 1 : 没分支， 可以IF下一句;

class Pipeline_Class {
public:
	TokenScanner exp;
	int n1 = 0, n2 = 0, n3 = 0;
	long long ans = 0;
	int lonum = 0, hinum = 0;
	int tarline = 0, nowline = 0;
	int word = 0;
	short half = 0;
	char byte;
	bool jump = 0;
	int state = 1; //0 : break; 1 : run;
	int step = 0;
	size_t pos = 0;

	Pipeline_Class() { }

	void Instruction_Fetch(int &linenum, TokenScanner &token) {
		step = 1;
		nowline = linenum;
		exp = token;

		pipeline_state[1] = 1;

		if (exp.op >= 23 && exp.op <= 39) pipeline_state[0] = 0;
		else {
			pipeline_state[0] = 1;
			reg[34].data++;
		}

		return;
	}
	void Instruction_Decode_And_Data_Preparation() {
		step = 2;
		if (exp.op >= 1 && exp.op <= 13) {
			if (reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			if (exp.expstate[2] == isreg && reg[exp.num[2]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
			n2 = reg[exp.num[1]].data;
			if (exp.expstate[2] == isreg) n3 = reg[exp.num[2]].data;
			else n3 = exp.num[2];
		}
		else if (exp.op >= 14 && exp.op <= 15) {
			if (reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
			n2 = reg[exp.num[1]].data;
		}
		else if (exp.op == 16) {
			n1 = exp.num[0];
			reg[n1].occupied = 1;
			n2 = exp.num[1];
		}
		else if (exp.op >= 17 && exp.op <= 22) {
			if (reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			if (exp.expstate[2] == isreg && reg[exp.num[2]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
			n2 = reg[exp.num[1]].data;
			if (exp.expstate[2] == isreg) n3 = reg[exp.num[2]].data;
			else n3 = exp.num[2];
		}
		else if (exp.op >= 23 && exp.op <= 28) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			if (exp.expstate[1] == isreg && reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = reg[exp.num[0]].data;
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data;
			else n2 = exp.num[1];
			n3 = exp.num[2];
		}
		else if (exp.op >= 29 && exp.op <= 34) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = reg[exp.num[0]].data;
			n2 = exp.num[1];
		}
		else if (exp.op >= 35 && exp.op <= 36) {
			n1 = exp.num[0];
		}
		else if (exp.op == 37) {
			reg[31].occupied = 1;
			n1 = exp.num[0];
		}
		else if (exp.op == 38) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = reg[exp.num[0]].data;
		}
		else if (exp.op == 39) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			reg[31].occupied = 1;
			n1 = reg[exp.num[0]].data;
		}
		else if (exp.op >= 40 && exp.op <= 43) {
			if (exp.expstate[1] == isreg && reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
		}
		else if (exp.op >= 44 && exp.op <= 46) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			if (exp.expstate[1] == isreg && reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = reg[exp.num[0]].data;
		}
		else if (exp.op == 47) {
			if (reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
			n2 = exp.num[1];
		}
		else if (exp.op == 48) {
			if (reg[32].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
		}
		else if (exp.op == 49) {
			if (reg[33].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = exp.num[0];
			reg[n1].occupied = 1;
		}
		else if (exp.op == 50) {}
		else if (exp.op == 51) {
			if (reg[2].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			switch (reg[2].data) {
			case 0:
				pipeline_state[2] = 2;
				return;
			case 1:
				if (reg[4].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				n1 = reg[4].data;

				break;
			case 4:
				if (reg[4].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				n1 = reg[4].data;
				break;
			case 5:
				reg[2].occupied = 1;
				break;
			case 8:
				if (reg[4].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				if (reg[5].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				n1 = reg[4].data;
				n2 = reg[5].data;
				break;
			case 9:
				if (reg[4].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				n1 = reg[4].data;
				reg[2].occupied = 1;
				break;
			case 10:
				break;
			case 17:
				if (reg[4].occupied == 1) {
					pipeline_state[2] = 2;
					return;
				}
				n1 = reg[4].data;
				break;
			}
		}
		else if (exp.op >= 52 && exp.op <= 55) {
			if (reg[exp.num[0]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			if (reg[exp.num[1]].occupied == 1) {
				pipeline_state[2] = 2;
				return;
			}
			n1 = reg[exp.num[0]].data;
			n2 = reg[exp.num[1]].data;
			reg[32].occupied = 1;
			reg[33].occupied = 1;
		}

		pipeline_state[2] = 1;

		return;
	}
	void Execution() {
		step = 3;
		switch (exp.op) {
		case 1:
			ans = n2 + n3;
			break;
		case 2:
			ans = n2 + n3;
			break;
		case 3:
			ans = n2 + n3;
			break;
		case 4:
			ans = n2 - n3;
			break;
		case 5:
			ans = n2 - n3;
			break;
		case 6:
			ans = n2 * n3;
			break;
		case 7:
			ans = n2 * n3;
			break;
		case 8:
			ans = n2 / n3;
			break;
		case 9:
			ans = n2 / n3;
			break;
		case 10:
			ans = n2 ^ n3;
			break;
		case 11:
			ans = n2 ^ n3;
			break;
		case 12:
			ans = n2 % n3;
			break;
		case 13:
			ans = n2 % n3;
			break;
		case 14:
			ans = -n2;
			break;
		case 15:
			ans = (n2 >= 0 ? -n2 : n2);
			break;
		case 16:
			ans = n2;
			break;
		case 17:
			ans = (n2 == n3);
			break;
		case 18:
			ans = (n2 >= n3);
			break;
		case 19:
			ans = (n2 > n3);
			break;
		case 20:
			ans = (n2 <= n3);
			break;
		case 21:
			ans = (n2 < n3);
			break;
		case 22:
			ans = (n2 != n3);
			break;
		case 23:
			if (n1 == n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 24:
			if (n1 != n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 25:
			if (n1 >= n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 26:
			if (n1 <= n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 27:
			if (n1 > n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 28:
			if (n1 < n2) {
				jump = 1;
				tarline = n3;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 29:
			if (n1 == 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 30:
			if (n1 != 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 31:
			if (n1 <= 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 32:
			if (n1 >= 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 33:
			if (n1 > 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 34:
			if (n1 < 0) {
				jump = 1;
				tarline = n2;
			}
			else {
				pipeline_state[0] = 1;
				reg[34].data++;
			}
			break;
		case 35:
			jump = 1;
			tarline = n1;
			break;
		case 36:
			jump = 1;
			tarline = n1;
			break;
		case 37:
			jump = 1;
			tarline = n1;
			break;
		case 38:
			jump = 1;
			tarline = n1;
			break;
		case 39:
			jump = 1;
			tarline = n1;
			break;
		case 40:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 41:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 42:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 43:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 44:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 45:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 46:
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
			break;
		case 47:
			break;
		case 48:
			break;
		case 49:
			break;
		case 50:
			break;
		case 51:
			if (reg[2].data == 1) cout << reg[4].data;
			else if (reg[2].data == 5) cin >> ans;
			else if (reg[2].data == 10) state = 0;
			else if (reg[2].data == 17) state = 0;
			break;
		case 52:
			ans = n1 * n2;
			lonum = 0x00000000FFFFFFFF & ans;
			hinum = 0xFFFFFFFF00000000 & ans;
			hinum >> 32;
			break;
		case 53:
			ans = n1 * n2;
			lonum = ans & 0x00000000FFFFFFFF;
			hinum = (ans & 0xFFFFFFFF00000000) >> 32;
			break;
		case 54:
			lonum = n1 / n2;
			hinum = n1 % n2;
			break;
		case 55:
			lonum = n1 / n2;
			hinum = n1 % n2;
			break;
		}

		pipeline_state[3] = 1;

		return;
	}
	void Memory_Access() {
		step = 4;
		pipeline_state[4] = 1;

		switch (exp.op)	{
		case 40:
			ans = n2;
			return;
		case 41:
			byte = mem[n2];
			return;
		case 42:
			char tmp2[2];
			tmp2[0] = mem[n2];
			tmp2[1] = mem[n2 + 1];
			half = *(reinterpret_cast<short*>(tmp2));
			return;
		case 43:
			char tmp4[4];
			tmp4[0] = mem[n2];
			tmp4[1] = mem[n2 + 1];
			tmp4[2] = mem[n2 + 2];
			tmp4[3] = mem[n2 + 3];
			word = *(reinterpret_cast<int*>(tmp4));
			return;
		case 44:
			mem[n2] = (char)n1;
			return;
		case 45:
			mem[n2] = (char)n1;
			mem[n2 + 1] = (char)(n1 >> 8);
			return;
		case 46:
			mem[n2] = (char)n1;
			mem[n2 + 1] = (char)(n1 >> 8);
			mem[n2 + 2] = (char)(n1 >> 16);
			mem[n2 + 3] = (char)(n1 >> 24);
			return;
		case 51:
			if (reg[2].data == 4) {
				for (int i = n1; ; i++) {
					if (mem[i] == 0) break;
					else cout << mem[i];
				}
				return;
			}
			else if (reg[2].data == 8) {
				string tmp;
				int now_pos = n1;
				cin >> tmp;
				for (int i = 0; i < tmp.length(); i++) {
					mem[now_pos++] = tmp[i];
				}
				mem[now_pos++] = '\0';
				return;
			}
			else if (reg[2].data == 9) {
				ans = mem_pos;
				mem_pos += n1;
				return;
			}
		}

		/*if (exp.op == 40) {
			ans = n2;
			return;
		}
		else if (exp.op == 41) {
			byte = mem[n2];
			return;
		}
		else if (exp.op == 42) {
			char tmp[2];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			half = *(reinterpret_cast<short*>(tmp));
			return;
		}
		else if (exp.op == 43) {
			char tmp[4];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			tmp[2] = mem[n2 + 2];
			tmp[3] = mem[n2 + 3];
			word = *(reinterpret_cast<int*>(tmp));
			return;
		}
		else if (exp.op == 44) {
			mem[n2] = (char)n1;
			return;
		}
		else if (exp.op == 45) {
			mem[n2] = (char)n1;
			mem[n2 + 1] = (char)(n1 >> 8);
			return;
		}
		else if (exp.op == 46) {
			mem[n2] = (char)n1;
			mem[n2 + 1] = (char)(n1 >> 8);
			mem[n2 + 2] = (char)(n1 >> 16);
			mem[n2 + 3] = (char)(n1 >> 24);

			return;
		}
		else if (exp.op == 51) {
			if (reg[2].data == 4) {
				for (int i = n1; ; i++) {
					if (mem[i] == 0) break;
					else cout << mem[i];
				}
			}
			else if (reg[2].data == 8) {
				string tmp;
				int now_pos = n1;
				cin >> tmp;
				for (int i = 0; i < tmp.length(); i++) {
					mem[now_pos++] = tmp[i];
				}
				mem[now_pos++] = '\0';
			}
			else if (reg[2].data == 9) {
				ans = mem_pos;
				mem_pos += n1;
			}
		}
		else pipeline_state[4] = 0;*/

		pipeline_state[4] = 0;

		return;
	}
	void Write_Back() {
		step = 5; 
		if (jump) {
			reg[34].data = tarline;
			pipeline_state[0] = 1;
		}
		if (exp.op >= 1 && exp.op <= 13) {
			reg[n1].data = ans;
			reg[n1].occupied = 0;
		}
		else if (exp.op >= 14 && exp.op <= 15) {
			reg[n1].data = ans;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 16) {
			reg[n1].data = ans;
			reg[n1].occupied = 0;
		}
		else if (exp.op >= 17 && exp.op <= 22) {
			reg[n1].data = ans;
			reg[n1].occupied = 0;
		}
		else if (exp.op >= 23 && exp.op <= 28) {}
		else if (exp.op >= 29 && exp.op <= 34) {}
		else if (exp.op >= 35 && exp.op <= 36) {}
		else if (exp.op == 37) {
			reg[31].data = nowline + 1;
			reg[31].occupied = 0;
		}
		else if (exp.op == 38) {}
		else if (exp.op == 39) {
			reg[31].data = nowline + 1;
			reg[31].occupied = 0;
		}
		else if (exp.op == 40) {
			reg[n1].data = ans;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 41) {
			reg[n1].data = byte;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 42) {
			reg[n1].data = half;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 43) {
			reg[n1].data = word;
			reg[n1].occupied = 0;
		}
		else if (exp.op >= 44 && exp.op <= 46) {}
		else if (exp.op == 47) {
			reg[n1].data = reg[n2].data;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 48) {
			reg[n1].data = reg[32].data;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 49) {
			reg[n1].data = reg[33].data;
			reg[n1].occupied = 0;
		}
		else if (exp.op == 50) {}
		else if (exp.op == 51) {
			switch (reg[2].data) {
			case 5:
				reg[2].data = ans;
				reg[2].occupied = 0;
				break;
			case 9:
				reg[2].data = ans;
				reg[2].occupied = 0;
				break;
			}
		}
		else if (exp.op >= 52 && exp.op <= 55) {
			reg[32].data = hinum;
			reg[32].occupied = 0;
			reg[33].data = lonum;
			reg[33].occupied = 0;
		}

		pipeline_state[5] = 1;

		return;
	}

	void print() {
		for (int i = 0; i < 35; i++) {
			cout << /*"reg[" << i << "] = " <<*/ reg[i].data << ' ';
		}
		cout << '\n';
	}

};