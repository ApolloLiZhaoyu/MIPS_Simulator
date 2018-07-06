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

class Pipeline_Class {
public:
	TokenScanner exp;
	int n1, n2, n3;
	long long ans = 0;
	int lonum = 0, hinum = 0;
	int tarline = 0, nowline = 0;
	int word = 0;
	short half = 0;
	char byte;
	bool jump = 0;
	int state = 1; //0 : break; 1 : run;
	size_t pos = 0;

	Pipeline_Class() { }

	void Instruction_Fetch(int &linenum, TokenScanner &token) {
		nowline = linenum;
		exp = token;
		ans = 0;
		lonum = 0, hinum = 0;
		tarline = 0;
		word = 0;
		half = 0;
		byte = 0;
		jump = 0;
		state = 1;
		pos = 0;
		return;
	}
	void Instruction_Decode_And_Data_Preparation() {

		if (exp.op >= 1 && exp.op <= 13) {
			n1 = exp.num[0];
			n2 = reg[exp.num[1]].data;
			if (exp.expstate[2] == isreg) n3 = reg[exp.num[2]].data;
			else n3 = exp.num[2];
		}
		else if (exp.op >= 14 && exp.op <= 15) {
			n1 = exp.num[0];
			n2 = reg[exp.num[1]].data;
		}
		else if (exp.op == 16) {
			n1 = exp.num[0];
			n2 = exp.num[1];
		}
		else if (exp.op >= 17 && exp.op <= 22) {
			n1 = exp.num[0];
			n2 = reg[exp.num[1]].data;
			if (exp.expstate[2] == isreg) n3 = reg[exp.num[2]].data;
			else n3 = exp.num[2];
		}
		else if (exp.op >= 23 && exp.op <= 28) {
			n1 = reg[exp.num[0]].data;
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data;
			else n2 = exp.num[1];
			n3 = exp.num[2];
		}
		else if (exp.op >= 29 && exp.op <= 34) {
			n1 = reg[exp.num[0]].data;
			n2 = exp.num[1];
		}
		else if (exp.op >= 35 && exp.op <= 37) {
			n1 = exp.num[0];
		}
		else if (exp.op >= 38 && exp.op <= 39) {
			n1 = reg[exp.num[0]].data;
		}
		else if (exp.op >= 40 && exp.op <= 43) {
			n1 = exp.num[0];
		}
		else if (exp.op >= 44 && exp.op <= 46) {
			n1 = reg[exp.num[0]].data;
		}
		else if (exp.op == 47) {
			n1 = exp.num[0];
			n2 = exp.num[1];
		}
		else if (exp.op >= 48 && exp.op <= 49) {
			n1 = exp.num[0];
		}
		else if (exp.op == 50) {

		}
		else if (exp.op == 51) {
			switch (reg[2].data) {
			case 1:
				n1 = reg[4].data;
				break;
			case 2:
				n1 = reg[4].data;
				break;
			case 4:
				n1 = reg[4].data;
				break;
			case 8:
				n1 = reg[4].data;
				n2 = reg[5].data;
				break;
			case 9:
				n1 = reg[4].data;
				break;
			case 10:
				break;
			case 17:
				n1 = reg[4].data;
				break;
			}
		}
		else if (exp.op >= 52 && exp.op <= 55) {
			n1 = reg[exp.num[0]].data;
			n2 = reg[exp.num[1]].data;
		}

		//cout << n1 << ' ' << n2 << ' ' << n3 << '\n';
		return;
	}
	void Execution() {
		switch (exp.op)	{
		case 1 :
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
			break;
		case 24:
			if (n1 != n2) {
				jump = 1;
				tarline = n3;
			}
			break;
		case 25:
			if (n1 >= n2) {
				jump = 1;
				tarline = n3;
			}
			break;
		case 26:
			if (n1 <= n2) {
				jump = 1;
				tarline = n3;
			}
			break;
		case 27:
			if (n1 > n2) {
				jump = 1;
				tarline = n3;
			}
			break;
		case 28:
			if (n1 < n2) {
				jump = 1;
				tarline = n3;
			}
			break;
		case 29:
			if (n1 == 0) {
				jump = 1;
				tarline = n2;
			}
			break;
		case 30:
			if (n1 != 0) {
				jump = 1;
				tarline = n2;
			}
			break;
		case 31:
			if (n1 <= 0) {
				jump = 1;
				tarline = n2;
			}
			break;
		case 32:
			if (n1 >= 0) {
				jump = 1;
				tarline = n2;
			}
			break;
		case 33:
			if (n1 > 0) {
				jump = 1;
				tarline = n2;
			}
			break;
		case 34:
			if (n1 < 0) {
				jump = 1;
				tarline = n2;
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
			else if (reg[2].data == 4) {
				for (int i = n1; ; i++) {
					if (mem[i] == 0) break;
					else cout << mem[i];
				}
			}
			else if (reg[2].data == 5) cin >> ans;
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
			else if (reg[2].data == 10) {
				state = 0;
			}
			else if (reg[2].data == 17) {
				state = 0;
			}
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
		/*if (exp.op == "add") ans = n2 + n3;
		else if (exp.op == "addu") ans = n2 + n3;
		else if (exp.op == "addiu") ans = n2 + n3;
		else if (exp.op == "sub") ans = n2 - n3;
		else if (exp.op == "subu") ans = n2 - n3;
		else if (exp.op == "mul") ans = n2 * n3;
		else if (exp.op == "mulu") ans = n2 * n3;
		else if (exp.op == "rmul") {
			ans = n1 * n2;
			lonum = 0x00000000FFFFFFFF & ans;
			hinum = 0xFFFFFFFF00000000 & ans;
			hinum >> 32;
		}
		else if (exp.op == "rmulu") {
			ans = n1 * n2;
			lonum = ans & 0x00000000FFFFFFFF;
			hinum = (ans & 0xFFFFFFFF00000000) >> 32;
		}
		else if (exp.op == "div") ans = n2 / n3;
		else if (exp.op == "divu") ans = n2 / n3;
		else if (exp.op == "rdiv") {
			lonum = n1 / n2;
			hinum = n1 % n2;
			//cout << "rdiv : " << "lonum = " << lonum << " hinum = " << hinum << '\n';
		}
		else if (exp.op == "rdivu") {
			lonum = n1 / n2;
			hinum = n1 % n2;
		}
		else if (exp.op == "xor") ans = n2 ^ n3;
		else if (exp.op == "xoru") ans = n2 ^ n3;
		else if (exp.op == "neg") ans = -n2;
		else if (exp.op == "negu") ans = (n2 >= 0 ? -n2 : n2);
		else if (exp.op == "rem") ans = n2 % n3;
		else if (exp.op == "remu") ans = n2 % n3;
		else if (exp.op == "li") ans = n2;
		else if (exp.op == "seq") ans = (n2 == n3);
		else if (exp.op == "sge") ans = (n2 >= n3);
		else if (exp.op == "sgt") ans = (n2 > n3);
		else if (exp.op == "sle") ans = (n2 <= n3);
		else if (exp.op == "slt") ans = (n2 < n3);
		else if (exp.op == "sne") ans = (n2 != n3);
		else if (exp.op == "b") {
			jump = 1;
			tarline = n1;
		}
		else if (exp.op == "beq") {
			if (n1 == n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "bne") {
			if (n1 != n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "bge") {
			if (n1 >= n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "ble") {
			if (n1 <= n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "bgt") {
			if (n1 > n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "blt") {
			if (n1 < n2) {
				jump = 1;
				tarline = n3;
			}
		}
		else if (exp.op == "beqz") {
			if (n1 == 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "bnez") {
			if (n1 != 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "blez") {
			if (n1 <= 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "bgez") {
			if (n1 >= 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "bgtz") {
			if (n1 > 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "bltz") {
			if (n1 < 0) {
				jump = 1;
				tarline = n2;
			}
		}
		else if (exp.op == "j") {
			jump = 1;
			tarline = n1;
		}
		else if (exp.op == "jr") {
			jump = 1;
			tarline = n1;
		}
		else if (exp.op == "jal") {
			jump = 1;
			tarline = n1;
		}
		else if (exp.op == "jalr") {
			jump = 1;
			tarline = n1;
		}
		else if (exp.op == "la") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "lb") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "lh") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "lw") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "sb") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "sh") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "sw") {
			if (exp.expstate[1] == isreg) n2 = reg[exp.num[1]].data + exp.offset;
			else n2 = exp.num[1] + exp.offset;
		}
		else if (exp.op == "move");
		else if (exp.op == "mfhi");
		else if (exp.op == "mflo");
		else if (exp.op == "nop");
		else if (exp.op == "syscall") {
			if (reg[2].data == 1) cout << reg[4].data;
			else if (reg[2].data == 4) {
				for (int i = n1; ; i++) {
					if (mem[i] == 0) break;
					else cout << mem[i];
				}
			}
			else if (reg[2].data == 5) cin >> ans;
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
			else if (reg[2].data == 10) {
				state = 0;
			}
			else if (reg[2].data == 17) {
				state = 0;
			}
		}*/
		return;
	}
	void Memory_Access() {
		if (exp.op == 40) {
			ans = n2;
		}
		else if (exp.op == 41) {
			byte = mem[n2];
		}
		else if (exp.op == 42) {
			char tmp[2];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			half = *(reinterpret_cast<short*>(tmp));
		}
		else if (exp.op == 43) {
			char tmp[4];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			tmp[2] = mem[n2 + 2];
			tmp[3] = mem[n2 + 3];
			word = *(reinterpret_cast<int*>(tmp));
		}
		else if (exp.op == 44) {
			mem[n2] = (char) n1;
		}
		else if (exp.op == 45) {
			mem[n2] = (char) n1;
			mem[n2 + 1] = (char)(n1 >> 8);
		}
		else if (exp.op == 46) {
			mem[n2] = (char)n1;
			mem[n2 + 1] = (char)(n1 >> 8);
			mem[n2 + 2] = (char)(n1 >> 16);
			mem[n2 + 3] = (char)(n1 >> 24);
		}
		/*if (exp.op == "la") ans = n2; 
		else if (exp.op == "lb") {
			byte = mem[n2];
		}
		else if (exp.op == "lh") {
			char tmp[2];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			half = *(reinterpret_cast<short*>(tmp));
		}
		else if (exp.op == "lw") {
			char tmp[4];
			tmp[0] = mem[n2];
			tmp[1] = mem[n2 + 1];
			tmp[2] = mem[n2 + 2];
			tmp[3] = mem[n2 + 3];
			word = *(reinterpret_cast<int*>(tmp));
		}
		else if (exp.op == "sb") {
			mem[n2] = (char)reg[n1].data;
		}
		else if (exp.op == "sh") {
			mem[n2] = (char)reg[n1].data;
			mem[n2 + 1] = (char)(reg[n1].data >> 8);
		}
		else if (exp.op == "sw") {
			mem[n2] = (char)reg[n1].data;
			mem[n2 + 1] = (char)(reg[n1].data >> 8);
			mem[n2 + 2] = (char)(reg[n1].data >> 16);
			mem[n2 + 3] = (char)(reg[n1].data >> 24);
		}*/
		return;
	}
	void Write_Back() {
		if (jump) {
			reg[34].data = tarline;
		}
		else reg[34].data++;
		if (exp.op >= 1 && exp.op <= 13) {
			reg[n1].data = ans;
		}
		else if (exp.op >= 14 && exp.op <= 15) {
			reg[n1].data = ans;
		}
		else if (exp.op == 16) {
			reg[n1].data = ans;
		}
		else if (exp.op >= 17 && exp.op <= 22) {
			reg[n1].data = ans;
		}
		else if (exp.op >= 23 && exp.op <= 28) { }
		else if (exp.op >= 29 && exp.op <= 34) { }
		else if (exp.op >= 35 && exp.op <= 37) {
			if (exp.op == 37) reg[31].data = nowline + 1;
		}
		else if (exp.op >= 38 && exp.op <= 39) {
			if (exp.op == 39) reg[31].data = nowline + 1;
		}
		else if (exp.op == 40) reg[n1].data = ans;
		else if (exp.op == 41) reg[n1].data = byte;
		else if (exp.op == 42) reg[n1].data = half;
		else if (exp.op == 43) reg[n1].data = word;
		else if (exp.op >= 44 && exp.op <= 46) { }
		else if (exp.op == 47) reg[n1].data = reg[n2].data;
		else if (exp.op == 48) reg[n1].data = reg[32].data;
		else if (exp.op == 49) reg[n1].data = reg[33].data;
		else if (exp.op == 50) { }
		else if (exp.op == 51) {
			switch (reg[2].data) {
			case 5:
				reg[2].data = ans;
				break;
			case 9:
				reg[2].data = ans;
				break;
			}
		}
		else if (exp.op >= 52 && exp.op <= 55) {
			reg[32].data = hinum;
			reg[33].data = lonum;
		}
		return;
	}
	void print() {
		for (int i = 0; i < 35; i++) {
			cout << /*"reg[" << i << "] = " <<*/ reg[i].data << ' ';
		}
		cout << '\n';
	}
};