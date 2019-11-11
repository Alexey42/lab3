#pragma once
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

class Program {

	class Lexem {
	public:
		int val = 0;
		string op = "";
		string type;

		Lexem(int x1, int x2, string s, string t):type(t) {
			string tmp = "";
			for (int i = x1; i < x2; i++) tmp.push_back(s[i]);
			if (t == "digit") val = stoi(tmp);
			if (t == "neg_digit") val = -stoi(tmp);
		}
		Lexem(char sym, string t):type(t) {
			op.push_back(sym);
		}
	};

	bool isOp(char c) {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')') return true;
		else return false;
	}
	
	int Priority(string c) {
		if (c == "(") return 0;
		if (c == "+" || c == "-") return 1;
		if (c == "*" || c == "/") return 2;
		if (c == "^") return 3;
	}

	void Calc(vector<string> input) {
		int x = 0, y = 0;
		char op = ' ';
		stack<int> q;

		for (int i = 0; i < input.capacity(); i++) {
			if (isdigit(input[i][0]) || (input[i][0] == '-' && input[i].length() > 1)) {
				string tmp = input[i];
				q.push(stoi(input[i]));
				continue;
			}
			if (Priority(input[i]) == 1) {
				y = q.top(); q.pop();
				x = q.top(); q.pop();
				q.push(Plus(x, y, input[i][0]));
				continue;
			}
			if (Priority(input[i]) == 2) {
				y = q.top(); q.pop();
				x = q.top(); q.pop();
				q.push(Mult(x, y, input[i][0]));
				continue;
			}
			if (Priority(input[i]) == 3) {
				y = q.top(); q.pop();
				x = q.top(); q.pop();
				q.push(pow(x, y));
				continue;
			}
		}
		cout << endl << q.top() << endl;
	}

	void RPN(vector<Lexem> input) {
		vector<string> output;
		stack<string> ops;

		for (int i = 0; i < input.capacity(); i++) {
			if (input[i].type == "digit" || input[i].type == "neg_digit") {
				output.push_back(to_string(input[i].val)); 
			}
			if (input[i].type == "left") {
				ops.push("(");
				continue;
			}
			if (input[i].type == "right") {
				while (ops.top() != "(")
				{
					output.push_back(ops.top());
					ops.pop();
				}
				ops.pop();
				continue;
			}
			if (input[i].type == "bin_op") {
				if (!ops.empty() && Priority(ops.top()) >= Priority(input[i].op)) {
					output.push_back(ops.top());
					ops.pop();
					ops.push(input[i].op);
				}
				else ops.push(input[i].op);
				continue;
			}
		}
		while (!ops.empty()) {
			output.push_back(ops.top());
			ops.pop();
		}
		for (int i = 0; i < output.size(); i++) cout << output[i];

		Calc(output);
	}

public:
	int Parse(string input) {
		string status = "wait";
		vector<Lexem> output;
		int flag = 0;

		for (int i = 0; i < input.length(); i++) {
			if (status == "wait") {
				if (isdigit(input[i])) {
					int end = i + 1;
					while (isdigit(input[end])) end++;
					Lexem l(i, end, input, "digit"); 
					output.push_back(l);
					status = "wait_bin_op";
					i = end - 1;
					continue;
				}
				if (isOp(input[i])) {
					if (input[i] == '-') {
						status = "wait_negative_digit";
					}
					else {
						if (input[i] == '(') flag++;
						status = "wait_digit";
						output.push_back(Lexem(input[i], "bin_op"));
					}
					continue;
				}
				else return 0;
			}
			if (status == "wait_bin_op") {
				if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^') {
					status = "wait_digit";
					output.push_back(Lexem(input[i], "bin_op"));
					continue;
				}
				if (input[i] == ')') {
					flag--;
					status = "wait_bin_op";
					output.push_back(Lexem(input[i], "right"));
					continue;
				}
				else return 0;
			}
			if (status == "wait_digit") {
				if (isdigit(input[i])) {
					int end = i + 1;
					while (isdigit(input[end])) end++;
					Lexem l(i, end, input, "digit"); 
					output.push_back(l);
					status = "wait_bin_op";
					i = end - 1;
					continue;
				}
				if (input[i] == '-' && isOp(input[i - 1])) {
					status = "wait_negative_digit";
					continue;
				}
				if (input[i] == '(') {
					flag++;
					status = "wait_digit";
					output.push_back(Lexem(input[i], "left"));
					continue;
				}
				else return 0;
			}
			if (status == "wait_negative_digit") {
				if (isdigit(input[i])) {
					int end = i + 1;
					while (isdigit(input[end])) end++;
					Lexem l(i, end, input, "neg_digit"); 
					output.push_back(l);
					status = "wait_bin_op";
					i = end - 1;
					continue;
				}
				if (input[i] == '(') {
					flag++;
					status = "wait_negative_digit";
					output.push_back(Lexem(input[i], "left"));
					continue;
				}
				else return 0;
			}
		}

		if (flag == 0) RPN(output);

		return 0;
	}

	int Plus(int x, int y, char op) {
		if (op == '-') return x - y;
		if (op == '+') return x + y;
	}

	int Mult(int x, int y, char op) {
		if (op == '*') return x * y;
		if (op == '/') return x / y;
	}
};