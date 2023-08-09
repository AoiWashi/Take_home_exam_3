
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <bitset>

using namespace std;


typedef pair<string, string> mapItem;

bool CompareStr(string a, string b);
vector<string> SplitStrByDelim(string str, char delim);
string TrimStrByDelim(string str, char delim);
string space_trim(string str);
bool process_R_type(string command, vector<string> args, string* binary_code);
bool process_S_type(string command, vector<string> args, string* binary_code);
bool process_I_type(string command, vector<string> args, string* binary_code);
string Print_Binary(int num);
bool append_arguments(string arg, string* binary_code);
bool is_num(string str);

map<string, string> Opcode;
map<string, string> Operands;
map<string, string>func_code;

int main() {

	Opcode.insert(pair<string, string>("add", "000000"));
	Opcode.insert(pair<string, string>("sub", "000000"));
	Opcode.insert(pair<string, string>("slt", "000000"));
	Opcode.insert(pair<string, string>("sll", "000000"));
	Opcode.insert(pair<string, string>("beq", "000100"));
	Opcode.insert(pair<string, string>("addi", "001000"));


	Operands.insert(pair<string, string>("$zero", "00000"));
	Operands.insert(pair<string, string>("$at", "00001"));
	Operands.insert(pair<string, string>("$v0", "00010"));
	Operands.insert(pair<string, string>("$v1", "00011"));
	Operands.insert(pair<string, string>("$a0", "00100"));
	Operands.insert(pair<string, string>("$a1", "00101"));
	Operands.insert(pair<string, string>("$a2", "00110"));
	Operands.insert(pair<string, string>("$a3", "00111"));
	Operands.insert(pair<string, string>("$t0", "01000"));
	Operands.insert(pair<string, string>("$t1", "01001"));
	Operands.insert(pair<string, string>("$t2", "01010"));
	Operands.insert(pair<string, string>("$t3", "01011"));
	Operands.insert(pair<string, string>("$t4", "01100"));
	Operands.insert(pair<string, string>("$t5", "01101"));
	Operands.insert(pair<string, string>("$t6", "01110"));
	Operands.insert(pair<string, string>("$t7", "01111"));
	Operands.insert(pair<string, string>("$s0", "10000"));
	Operands.insert(pair<string, string>("$s1", "10001"));
	Operands.insert(pair<string, string>("$s2", "10010"));
	Operands.insert(pair<string, string>("$s3", "10011"));
	Operands.insert(pair<string, string>("$s4", "10100"));
	Operands.insert(pair<string, string>("$s5", "10101"));
	Operands.insert(pair<string, string>("$s6", "10110"));
	Operands.insert(pair<string, string>("$s7", "10111"));
	Operands.insert(pair<string, string>("$t8", "11000"));
	Operands.insert(pair<string, string>("$t9", "11001"));
	Operands.insert(pair<string, string>("$k0", "11010"));
	Operands.insert(pair<string, string>("$k1", "11011"));
	Operands.insert(pair<string, string>("$gp", "11100"));
	Operands.insert(pair<string, string>("$sp", "11101"));
	Operands.insert(pair<string, string>("$fp", "11110"));
	Operands.insert(pair<string, string>("$ra", "11111"));

	func_code["add"] = "100000";
	func_code["sub"] = "100010";
	func_code["slt"] = "101010";
	func_code["sll"] = "000000";

	string  instruction;
	cout << "Enter an instruction: ";
	getline(cin, instruction);
	int found = instruction.find(' ');
	instruction = TrimStrByDelim(instruction, ' ');

	if (found == (int)std::string::npos) {
		cout << "INVALID INSTRUCTION NAME";
	}
	else {
		string command = instruction.substr(0, instruction.find(' '));
		string inst_sub;
		string binary_code;


		int start_index = instruction.find(' ') + 1;
		inst_sub = instruction.substr(start_index, instruction.length() - start_index);
		inst_sub = space_trim(inst_sub);

		if (Opcode.count(command) != 0) {
			binary_code = Opcode[command];

			vector<string> arguments = SplitStrByDelim(inst_sub, ',');
			int arg_size = arguments.size();
			if (arg_size != 3) {
				cout << "INVALID REGISTER NAME";
				return 1;
			}
			bool success;
			if (command == "add" || command == "sub" || command == "slt") {
				success = process_R_type(command, arguments, &binary_code);

			}
			else if (command == "sll") {
				success = process_S_type(command, arguments, &binary_code);
			}
			else if (command == "addi" || command == "beq") {
				success = process_I_type(command, arguments, &binary_code);
			}
			if (!success) {
				return 1;
			}	
			cout << "Machine code of the " << instruction << " is " << binary_code;
		}		
		else if (command == "j") {
			binary_code = "000010";
			/*if (!is_num(inst_sub)) {
				cout << "INVALID REGISTER NAME";
				return 1;
			}*/
			int num = stoi(inst_sub);
			if (num > 67108863 || num < 0) {
				cout << "CONSTANT IS OUT OF THE RANGE";
				return false;
			}
			std::bitset<26> binary(num);
			std::string num_binary = binary.to_string();
			binary_code += num_binary;
			cout << "Machine code of the " << instruction << " is " << binary_code;
		}
		else {
			cout << "INVALID INSTRUCTION NAME";

		}
	}
	
	return 0;

}
bool process_R_type(string command, vector<string> args, string* binary_code) {
	int arg_size = args.size();
	for (int i = 0; i < arg_size; i++) {
		string arg = args[i];

		if (!append_arguments(arg, binary_code)) {
			cout << "INVALID REGISTER NAME";
			return false;
		}
	}
	string shiamo = "00000";
	*binary_code += shiamo;
	*binary_code += func_code[command];
	return true;
}

bool process_S_type(string command, vector<string> args, string* binary_code) {
	int arg_size = args.size();
	for (int i = 0; i < arg_size - 1; i++) {
		string arg = args[i];

		if (!append_arguments(arg, binary_code)) {
			cout << "INVALID REGISTER NAME";
			return false;
		}
	}
	if (!is_num(args[2])) {
		cout << "INVALID REGISTER NAME";
		return false;
	}
	int num = stoi(args[2]);
	if (num > 31 || num < 0) {
		cout << "CONSTANT IS OUT OF THE RANGE";
		return false;
	}
	string rs2 = "00000";
	*binary_code += rs2;
	std::bitset<5> binary(num);
	std::string num_binary = binary.to_string();
	*binary_code += num_binary;
	*binary_code += func_code[command];
	return true;
}

bool process_I_type(string command, vector<string> args, string* binary_code) {
	int arg_size = args.size();
	for (int i = 0; i < arg_size - 1; i++) {
		string arg = args[i];

		if (!append_arguments(arg, binary_code)) {
			cout << "INVALID REGISTER NAME";
			return false;
		}
	}
	if (!is_num(args[2])) {
		cout << "INVALID REGISTER NAME";
		return false;
	}
	int num = stoi(args[2]);
	if (num > 65535 || num < 0) {	
		cout << "CONSTANT IS OUT OF THE RANGE";
		return false;
	}
	std::bitset<16> binary(num);
	std::string num_binary = binary.to_string();
	*binary_code += num_binary;
	return true;
}


bool append_arguments(string arg, string* binary_code) {

	if (Operands.count(arg) != 0) {
		*binary_code += Operands[arg];
		return true;
	}
	else {
		return false;
	}
}

bool is_num(string str) {
	int length = str.length();
	for (int i = 0; i < length; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

string space_trim(string str) {

	int length = str.length();
	string final_return = "";
	if (str.find(' ') == std::string::npos) {
		final_return = str;
	}
	else {
		for (int i = 0; i < length; i++) {
			if (str[i] != ' ') {
				final_return += str[i];
			}
		}
	}
	return final_return;
}

bool CompareStr(string a, string b) {
	if (a.empty() && b.empty())return true;
	int length_a = a.length();
	int length_b = b.length();
	if (length_a != length_b) return false;
	for (int i = 0; i < length_a; i++) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

vector<string> SplitStrByDelim(string str, char delim) {
	if (str.empty()) vector<string> res(1);
	int length = str.length();
	if (length == 0) vector<string> res(1);
	string newStr = TrimStrByDelim(str, delim);
	int delimCount = 0;
	length = newStr.length();
	for (int i = 0; i < length; i++) {
		if (str[i] == delim) delimCount++;
	}
	int lastPos = 0;
	vector<string> res(delimCount + 1);
	int segIndex = 0;
	int i = 0;
	do {
		if (newStr[i] == delim) {
			int l = i - lastPos; // -1 for excluding delim
			res[segIndex] = newStr.substr(lastPos, l);
			segIndex++;
			lastPos = i + 1;
		}
		i++;
	} while (i < length);
	int l = i - lastPos; // -1 for excluding out of range index
	res[segIndex] = newStr.substr(lastPos, l);
	return res;
}
string TrimStrByDelim(string str, char delim) {
	if (str.empty()) return str;
	int length = str.length();
	if (length == 0) return str;
	bool charFound = false;
	int firstChar = 0;
	int lastChar = 0;
	for (int i = 0; i < length; i++) {
		if (str[i] == delim) {
			if (!charFound) {
				firstChar++;
			}
		}
		else {
			//found
			if (!charFound) {
				lastChar = firstChar;
				charFound = true;
			}
			else {
				lastChar = i;
			}
		}
	}
	int l = lastChar - firstChar + 1;
	return str.substr(firstChar, l);
}