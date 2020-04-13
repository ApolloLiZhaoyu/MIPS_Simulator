#include <cstring>
#include "MIPS_Simulator.h"

using namespace std;

MIPS_Simulator_Class mips_simulaotor;
map <string, int> regname;
map <string, int> label;
map <string, int> op_num;
map <int, int> predictor_num;

char *mem;
int mem_pos = 0;

void init() {
	mem = new char[Max_Size];
	memset(mem, 0, Max_Size);

	reg[29].data = Max_Size - 1;
	// 1 : 算数与逻辑指令，有三个运算数;          2 ： 算数与逻辑指令，有两个运算数;
	// 3 : 常数操作指令;                        4 ： 比较指令;
	// 5 ：分支与跳转指令，有两个运算数;          6 ： 分支与跳转指令，有一个运算数;
	// 7 : 分支与跳转指令，只有label;            8 ： 分支与跳转指令，只有register;
	// 9 ： load指令;                          10 ： store指令;
	// 11 ： 数据移动指令，有两个运算数;         12 ： 数据移动指令， 有一个运算数;
	// 13 ： nop;                              14 : syscall;
	// 15 :  rmul, rmulu, rdiv, rdivu
	/*op_type["add"] = 1;
	op_type["addu"] = 1;
	op_type["addiu"] = 1;
	op_type["sub"] = 1;
	op_type["subu"] = 1;
	op_type["mul"] = 1;
	op_type["mulu"] = 1;
	op_type["rmul"] = 15;
	op_type["rmulu"] = 15;
	op_type["div"] = 1;
	op_type["divu"] = 1;
	op_type["rdiv"] = 15;
	op_type["rdivu"] = 15;
	op_type["xor"] = 1;
	op_type["xoru"] = 1;
	op_type["neg"] = 2;
	op_type["negu"] = 2;
	op_type["rem"] = 1;
	op_type["remu"] = 1;
	op_type["li"] = 3;
	op_type["seq"] = 4;
	op_type["sge"] = 4;
	op_type["sgt"] = 4;
	op_type["sle"] = 4;
	op_type["slt"] = 4;
	op_type["sne"] = 4;
	op_type["b"] = 8;
	op_type["beq"] = 5;
	op_type["bne"] = 5;
	op_type["bge"] = 5;
	op_type["ble"] = 5;
	op_type["bgt"] = 5;
	op_type["blt"] = 5;
	op_type["beqz"] = 6;
	op_type["bnez"] = 6;
	op_type["blez"] = 6;
	op_type["bgez"] = 6;
	op_type["bgtz"] = 6;
	op_type["bltz"] = 6;
	op_type["j"] = 8;
	op_type["jr"] = 7;
	op_type["jal"] = 8;
	op_type["jalr"] = 7;
	op_type["la"] = 9;
	op_type["lb"] = 9;
	op_type["lh"] = 9;
	op_type["lw"] = 9;
	op_type["sb"] = 10;
	op_type["sh"] = 10;
	op_type["sw"] = 10;
	op_type["move"] = 11;
	op_type["mfhi"] = 12;
	op_type["mflo"] = 12;
	op_type["nop"] = 13;
	op_type["syscall"] = 14;*/

	regname["$zero"] = 0;
	regname["$at"] = 1;
	regname["$v0"] = 2;
	regname["$v1"] = 3;
	regname["$a0"] = 4;
	regname["$a1"] = 5;
	regname["$a2"] = 6;
	regname["$a3"] = 7;
	regname["$t0"] = 8;
	regname["$t1"] = 9;
	regname["$t2"] = 10;
	regname["$t3"] = 11;
	regname["$t4"] = 12;
	regname["$t5"] = 13;
	regname["$t6"] = 14;
	regname["$t7"] = 15;
	regname["$s0"] = 16;
	regname["$s1"] = 17;
	regname["$s2"] = 18;
	regname["$s3"] = 19;
	regname["$s4"] = 20;
	regname["$s5"] = 21;
	regname["$s6"] = 22;
	regname["$s7"] = 23;
	regname["$t8"] = 24;
	regname["$t9"] = 25;
	regname["$gp"] = 28;
	regname["$sp"] = 29;
	regname["$fp"] = 30;
	regname["$ra"] = 31;
	regname["$hi"] = 32;
	regname["$lo"] = 33;
	regname["$pc"] = 34;

	regname["$0"] = 0;
	regname["$1"] = 1;
	regname["$2"] = 2;
	regname["$3"] = 3;
	regname["$4"] = 4;
	regname["$5"] = 5;
	regname["$6"] = 6;
	regname["$7"] = 7;
	regname["$8"] = 8;
	regname["$9"] = 9;
	regname["$10"] = 10;
	regname["$11"] = 11;
	regname["$12"] = 12;
	regname["$13"] = 13;
	regname["$14"] = 14;
	regname["$15"] = 15;
	regname["$16"] = 16;
	regname["$17"] = 17;
	regname["$18"] = 18;
	regname["$19"] = 19;
	regname["$20"] = 20;
	regname["$21"] = 21;
	regname["$22"] = 22;
	regname["$23"] = 23;
	regname["$24"] = 24;
	regname["$25"] = 25;
	regname["$28"] = 28;
	regname["$29"] = 29;
	regname["$30"] = 30;
	regname["$31"] = 31;
	regname["$32"] = 32;
	regname["$33"] = 33;
	regname["$34"] = 34;

	op_num["add"] = 1;
	op_num["addu"] = 2;
	op_num["addiu"] = 3;
	op_num["sub"] = 4;
	op_num["subu"] = 5;
	op_num["mul"] = 6;
	op_num["mulu"] = 7;
	op_num["div"] = 8;
	op_num["divu"] = 9;
	op_num["xor"] = 10;
	op_num["xoru"] = 11;
	op_num["rem"] = 12;
	op_num["remu"] = 13;
	op_num["neg"] = 14;
	op_num["negu"] = 15;
	op_num["li"] = 16;
	op_num["seq"] = 17;
	op_num["sge"] = 18;
	op_num["sgt"] = 19;
	op_num["sle"] = 20;
	op_num["slt"] = 21;
	op_num["sne"] = 22;
	op_num["beq"] = 23;
	op_num["bne"] = 24;
	op_num["bge"] = 25;
	op_num["ble"] = 26;
	op_num["bgt"] = 27;
	op_num["blt"] = 28;
	op_num["beqz"] = 29;
	op_num["bnez"] = 30;
	op_num["blez"] = 31;
	op_num["bgez"] = 32;
	op_num["bgtz"] = 33;
	op_num["bltz"] = 34;
	op_num["b"] = 35;
	op_num["j"] = 36;
	op_num["jal"] = 37;
	op_num["jr"] = 38;
	op_num["jalr"] = 39;
	op_num["la"] = 40;
	op_num["lb"] = 41;
	op_num["lh"] = 42;
	op_num["lw"] = 43;
	op_num["sb"] = 44;
	op_num["sh"] = 45;
	op_num["sw"] = 46;
	op_num["move"] = 47;
	op_num["mfhi"] = 48;
	op_num["mflo"] = 49;
	op_num["nop"] = 50;
	op_num["syscall"] = 51;
	op_num["rmul"] = 52;
	op_num["rmulu"] = 53;
	op_num["rdiv"] = 54;
	op_num["rdivu"] = 55;
}

int main(int argc, char *argv[] ) {

	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	string file_path = argv[1];
	fstream fin(file_path);

	init();

	mips_simulaotor.execute(fin);

	fin.close();
	delete mem;
	return 0;
}
