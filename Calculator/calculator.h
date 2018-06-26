#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <algorithm>
#include <vector>
#include <cmath>
#include <fstream>
#include <iterator>

#define REP(i, n) for(int i=0; i<n; i++)
#define FOREACH(it, x) for(auto it = x.begin(); it != x.end(); it++)

std::string infixToPostfix(const std::string&);
bool isOperator(const char&);
bool hasHigherPrecedence(const char &, const char &);
double calculate(const double &, const double&, const char*);
double solve(const std::string&);

typedef struct{
	double result = 0;
	std::vector<double> clipboard;
	bool memoryMode = false;
	
	const void show();
	void show_clipboard_content();
	void save();
	void clear();
	bool settings(const std::string&);
	void exec(std::string);
} Box;

#endif
