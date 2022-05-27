#include<iostream>
#include<fstream>
#include<string>
#include<map>
using namespace std;

fstream input;
fstream temp;
fstream output;
long pc = 4194304;	//PC ban đầu có địa chỉ 00400000 tương ứng.
map<string, long> Lable;	//lưu địa chỉ label
map<string, long>::iterator it1;
map<string, pair<string, string>> Core;	//lưu các opcode và function
map<string, pair<string, string>>::iterator it2;
string R_type[] = { "add", "addu", "and","nor","or","slt","jr", "sltu", "sll", "srl", "sub", "subu"};
string I_type[] = { "addi", "addiu", "andi", "beq", "bne", "lbu", "lhu", "lui", "lw", "sw", "ll", "ori", "slti", "sltiu", "sb", "sc", "sh"};
string J_type[2] = { "j", "jal" };

void InputToTemp() {
	int numbline = 0;
	string s;
	int flag = 0;
	int check = 0;
	bool seeWord = false;
	while (input) {
		seeWord = false;
		getline(input, s);
		if (s == "") continue;
		if (s == "\n") continue;
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '.') {
				if (s[i + 1] == 't' || s[i + 1] == 'd') {
					flag++;
					break;
				}
			}
			if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) seeWord = true;
			if (flag <= 1) break;
			if (s[i] == '\t' || s[i] == ' ') {
				if (!seeWord) continue;
				if (s[i + 1] != ' ' && s[i + 1] != '\t') temp << " ";
				continue;
			}
			if (s[i] == ' ' && !seeWord)continue;
			if (s[i] == '#')break;
			if (s[i] == ',') continue;
			if (s[i] == ':') {
				check = 0;
				temp << ':';
				for (int j = i + 1; j < s.size(); j++) {
					if ((s[j] == ' ' || s[j] == '\t') && !check)continue;
					check++;
					if(check == 1) temp << '\n';
					if (s[j] == ',') continue;
					temp << s[j];
				}
				break;
			}
			temp << s[i];
		}
		if (flag > 2&& seeWord) temp << '\n';
		if (flag == 2) flag++;
		}
}
void CalculateImmediate() {
	temp.close();
	temp.open("temp.txt");
	long lable = pc;
	string s;
	while (temp) {
		int check = false;
		getline(temp, s);
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == ':') {
				Lable.insert(make_pair(s, lable));
				check = true;
				break;
			}
		}
		if(!check) lable += 4;
	}
}
string RegisterBin(string s) {
	if (s == "$0" || s == "$zero")return "00000";
	if (s == "$1" || s == "$at")return "00001";
	if (s == "$2" || s == "$v0")return "00010";
	if (s == "$3" || s == "$v1")return "00011";
	if (s == "$4" || s == "$a0")return "00100";
	if (s == "$5" || s == "$a1")return "00101";
	if (s == "$6" || s == "$a2")return "00110";
	if (s == "$7" || s == "$a3")return "00111";
	if (s == "$8" || s == "$t0")return "01000";
	if (s == "$9" || s == "$t1")return "01001";
	if (s == "$10" || s == "$t2")return "01010";
	if (s == "$11" || s == "$t3")return "01011";
	if (s == "$12" || s == "$t4")return "01100";
	if (s == "$13" || s == "$t5")return "01101";
	if (s == "$14" || s == "$t6")return "01110";
	if (s == "$15" || s == "$t7")return "01111";
	if (s == "$16" || s == "$s0")return "10000";
	if (s == "$17" || s == "$s1")return "10001";
	if (s == "$18" || s == "$s2")return "10010";
	if (s == "$19" || s == "$s3")return "10011";
	if (s == "$20" || s == "$s4")return "10100";
	if (s == "$21" || s == "$s5")return "10101";
	if (s == "$22" || s == "$s6")return "10110";
	if (s == "$23" || s == "$s7")return "10111";
	if (s == "$24" || s == "$t8")return "11000";
	if (s == "$25" || s == "$t9")return "11001";
	if (s == "$26" || s == "$k0")return "11010";
	if (s == "$27" || s == "$k1")return "11011";
	if (s == "$28" || s == "$gp")return "11100";
	if (s == "$29" || s == "$sp")return "11101";
	if (s == "$30" || s == "$fp")return "11110";
	if (s == "$31" || s == "$ra")return "11111";
	else return "00000";
}
void GenerateBin() {
	pair<string, string> p;
	p = { "000000", "100000" };
	Core.insert(make_pair("add", p));
	p = { "000000", "100001" };
	Core.insert(make_pair("addu", p));
	p = { "000000", "100100" };
	Core.insert(make_pair("and", p));
	p = { "000000", "001000" };
	Core.insert(make_pair("jr", p));
	p = { "001000", "" };
	Core.insert(make_pair("addi", p));
	p = { "001001", "" };
	Core.insert(make_pair("addiu", p));
	p = { "001100", "" };
	Core.insert(make_pair("andi", p));
	p = { "000100", "" };
	Core.insert(make_pair("beq", p));
	p = { "000101", "" };
	Core.insert(make_pair("bne", p));
	p = { "100100", "" };
	Core.insert(make_pair("lbu", p));
	p = { "100101", "" };
	Core.insert(make_pair("lhu", p));
	p = { "001111", "" };
	Core.insert(make_pair("lui", p));
	p = { "100011", "" };
	Core.insert(make_pair("lw", p));
	p = { "101011", "" };
	Core.insert(make_pair("sw", p));
	p = { "000010", "" };
	Core.insert(make_pair("j", p));
	p = { "000011", "" };
	Core.insert(make_pair("jal", p));
	p = { "000000", "100111" };
	Core.insert(make_pair("nor", p));
	p = { "000000", "100101" };
	Core.insert(make_pair("or", p));
	p = { "000000", "101010" };
	Core.insert(make_pair("slt", p));
	p = { "000000", "101011" };
	Core.insert(make_pair("sltu", p));
	p = { "000000", "000000" };
	Core.insert(make_pair("sll", p));
	p = { "000000", "000010" };
	Core.insert(make_pair("srl", p));
	p = { "000000", "100010" };
	Core.insert(make_pair("sub", p));
	p = { "000000", "100011" };
	Core.insert(make_pair("subu", p));
	p = { "110000", "" };
	Core.insert(make_pair("ll", p));
	p = { "001101", "" };
	Core.insert(make_pair("ori", p));
	p = { "001010", "" };
	Core.insert(make_pair("slti", p));
	p = { "001011", "" };
	Core.insert(make_pair("sltiu", p));
	p = { "101000", "" };
	Core.insert(make_pair("sb", p));
	p = { "111000", "" };
	Core.insert(make_pair("sc", p));
	p = { "101001", "" };
	Core.insert(make_pair("sh", p));
}
int DefineFormat(string s) {
	for (int i = 0; i < 12; i++)
		if (s == R_type[i]) return 1;
	for (int i = 0; i < 17; i++)
		if (s == I_type[i]) return 2;
	for (int i = 0; i < 2; i++)
		if (s == J_type[i]) return 3;
	return 0;
}
string Dec_to_Bin_16bit(string s) {
	if (s.empty()) return "0000000000000000";
	int n = stoi(s);
	string bin = "";
	for (int i = 15; i >= 0; i--) {
		int k = n >> i;
		if (k & 1)
			bin += "1";
		else
			bin += "0";
	}
	return bin;
}
string Dec_to_Bin_5bit(string s) {
	if (s.empty()) return "00000";
	int n = stoi(s);
	string bin = "";
	for (int i = 4; i >= 0; i--) {
		int k = n >> i;
		if (k & 1)
			bin += "1";
		else
			bin += "0";
	}
	return bin;
}
string Dec_to_Bin_26bit(long s) {
	if (s == 0) return "00000000000000000000000000";
	int n = s;
	string bin = "";
	for (int i = 25; i >= 0; i--) {
		int k = n >> i;
		if (k & 1)
			bin += "1";
		else
			bin += "0";
	}
	return bin;
}

void Process() {
	temp.close();
	temp.open("temp.txt");
	string s, rd, rt, rs, imm, address, bin, shamt;
	while (temp>>s) {
		rd = rt = rs = imm = address = bin = shamt = "";
		if (s == "" || s == "\n")continue;
		it1 = Lable.find(s);
		if (it1 != Lable.end()) continue;
		int format = DefineFormat(s);
		if (format == 1) {
			if (s == "jr") {
				temp >> rs;
			}
			else if (s == "sll" || s == "srl") {
				temp >> rd;
				temp >> rt;
				temp >> shamt;
			}
			else {
				temp >> rd;
				temp >> rs;
				temp >> rt;
			}
			it2 = Core.find(s);
			bin += it2->second.first;
			bin += RegisterBin(rs) + RegisterBin(rt) + RegisterBin(rd) + Dec_to_Bin_5bit(shamt) + it2->second.second;
		}
		else if (format == 2) {
			if (s == "lw" || s == "sw" || s == "lbu" || s == "lhu" || s == "sb" || s == "sc"|| s == "sh" || s == "ll") {
				string a;
				bool check = false;
				temp >> rt;
				temp >> a;
				for (int i = 0; i < a.length(); i++) {
					if (a[i] == '(') {
						check = true;
						continue;
					}
					if (a[i] == ')') check = false;
					if (((a[i] >= '0' && a[i] <= '9') || a[i] == '-') && !check) imm += a[i];
					if (check) rs += a[i];
				}
			}
			else if (s == "lui") {
				temp >> rt;
				temp >> imm;
			}
			else if (s == "beq" || s == "bne") {
				temp >> rs;
				temp >> rt;
				temp >> address;
				address += ':';
				it1 = Lable.find(address);
				int n = (it1->second - pc)/4;
				n--;
				imm = to_string(n);
			}
			else {
				temp >> rt;
				temp >> rs;
				temp >> imm;
			}
			it2 = Core.find(s);
			bin += it2->second.first;
			bin += RegisterBin(rs) + RegisterBin(rt) + Dec_to_Bin_16bit(imm);
		}
		else if (format == 3) {
			it2 = Core.find(s);
			temp >> address;
			address += ':';
			it1 = Lable.find(address);
			bin += it2->second.first + Dec_to_Bin_26bit(it1->second/4);
		}
		output << bin;
		output << "\n";
		pc += 4;
	}
}
int main() {
	input.open("input.txt");
	output.open("output.txt");
	temp.open("temp.txt");

	InputToTemp();
	CalculateImmediate();
	GenerateBin();
	Process();

	temp.close();
	input.close();
	output.close();
}