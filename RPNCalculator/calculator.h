#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <iomanip>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)
#define ALL(x) x.begin(), x.end()

typedef std::vector<std::string> vs;
typedef std::stack<std::string> ss;
typedef std::vector<double> vd;

class Calculator{
private:
	enum { n = 10 };
	vs output;
	ss stack;
	vd results;
	double result;
public:
	Calculator();
	~Calculator();

	virtual void calculate(std::string&, bool muliResults);
	virtual void saveResults(bool muliResults);
	virtual void clear();
	virtual const void show(bool muliResults) const;


	virtual double rpn(const std::string&);
	virtual std::string buildPostfixExpression(const std::string &);

protected:
	static bool isBothSpace(char leftChar, char rightChar) { return leftChar == ' ' && leftChar == rightChar; }
	bool isOperator(const char &);
	bool precedence(const char &, const char &);
	bool isAlpha(std::string);
};

class Interface : public Calculator{
private:
	bool memoryMode;
public:
	Interface();
	~Interface();

	const void showInterface() const;
	void showCalculateResult(){ Calculator::show(memoryMode); }
	void doOperations(char&);
};

#endif