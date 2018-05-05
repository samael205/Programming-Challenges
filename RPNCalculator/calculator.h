#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <bits/stdc++.h>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)
#define ALL(x) x.begin(), x.end()

typedef std::vector<std::string> vs;
typedef std::stack<std::string> ss;
typedef std::vector<double> vd;

class Interface{
public:
	Interface();
	~Interface();

	virtual void calculate(std::string&, bool muliResults);
	void saveResults(bool muliResults);
	void clear();
	const void show(bool muliResults) const;

	double rpn(const std::string&);
	std::string buildPostfixExpression(const std::string &);
protected:
	bool isOperator(const char &);
	bool precedence(const char &, const char &);
	bool isAlpha(std::string);
private:
	enum { n = 10 };
	vs output;
	ss stack;
	vd results;
	double result;
};

class Calculator : public Interface{
public:
	Calculator();
	~Calculator();

	void calculate(char&);
	void showCalculateResult(){ Interface::show(memoryMode); }
	const void showMenu() const;
private:
	bool memoryMode;
};

#endif