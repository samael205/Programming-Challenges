#include <iostream>

#include "calculator.h"

bool isOperator(const char & check){
	switch(check){
		case '+':	
		case '-':
		case '*':
		case '/':
		case '^':
			return true;
		default:
			return false;
	}
}

bool hasHigherPrecedence(const char & left, const char & right){
	if(left == '^') return true;
	else if(right == '^') return false;
	else if(left == '*' or left == '/') return true;
	else if(right == '*' or right == '/') return false;
}

std::string infixToPostfix(const std::string & expression){
	std::stringstream postfix;
	std::stack<char> stack;
	FOREACH(it, expression){
		char currentSymbol = *it; 
		if(std::isspace(currentSymbol)) continue;
		if(std::isalnum(currentSymbol) or currentSymbol == '.') postfix<<currentSymbol;
		else if( currentSymbol  == '(') stack.push( currentSymbol );
		else if(isOperator(currentSymbol)){
			char rightOperand = currentSymbol;
			while(!stack.empty() and isOperator(stack.top()) and hasHigherPrecedence(stack.top(), rightOperand)){
				postfix<<" "<<stack.top();
				stack.pop();
			}		
			postfix<<" ";
			stack.push(rightOperand);
		}
		else if(currentSymbol == ')'){
			while(!stack.empty() and stack.top() != '('){
				postfix<<" "<<stack.top();
				stack.pop();
			}
			if(stack.empty())
				throw std::runtime_error("Missing left operator!\n");
			stack.pop();
			postfix<<" ";
		}
	}
	while(!stack.empty() and  stack.top() != '('){
		postfix<<" "<<stack.top();
		stack.pop();
	}	
	return postfix.str();
}

double calculate(const double & val1, const double & val2, const char * operation){
	switch(*operation){
		case '+':
			return val1 + val2;
		case '-':
			return val1 - val2;
		case '*':
			return val1 * val2;
		case '/':
			return val1 / val2;
		case '^':
			return std::pow(val1, val2);
		default:
			return 0;
	}
}

double solve(const std::string & expression){
	std::vector<double> stack;
	std::istringstream iss(expression);
	std::string token;
	double value;
	while(iss >> token){
		if(std::istringstream(token) >> value) stack.push_back(value);
		else{
			double first, second;
			second = stack.back();
			stack.pop_back();
			first = stack.back();
			stack.pop_back();
			auto calculatedValue = calculate(first, second, token.c_str());
			stack.push_back(calculatedValue);
		}
	}
	return stack.back();
}

void Box::show_clipboard_content(){
	REP(i, clipboard.size())
		std::cout<<clipboard[i]<<" ";
	std::cout<<"\n";
}

const void Box::show(){
	std::cout.precision(2);
	if(memoryMode) show_clipboard_content();	
	std::cout<<"\t= "<<result
		 <<"\nM. Memory mode"
		 <<"\nS. Save"
		 <<"\nC. Clear"
		 <<"\nE. Exit"
		 <<"\n-: "<<std::flush;
}

void Box::save(){
	std::fstream saveFile("calculations.txt", std::ios_base::out | std::ios_base::app);
	std::ostream_iterator<double> output(saveFile, " ");
	std::copy(clipboard.begin(), clipboard.end(), output);
	saveFile << "\n";
	saveFile.close();
}

void Box::clear(){
	result = 0;
	clipboard.clear();
}

static bool is_alpha(const std::string & input){
	return !input.empty() and std::find_if(input.begin(),
		input.end(), [](char c){ return std::isalpha(c);}) != input.end();
}

bool Box::settings(const std::string & input){
	if(!is_alpha(input) or input.length() != 1) return false;

	std::string option = input;
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);
	if(option == "e") std::exit(0);
	else if(option == "s") save();
	else if(option == "m") memoryMode = !memoryMode;
	else if(option == "c") clear();
	return true;
}

void Box::exec(std::string input){
	if(settings(input)) return;
	std::string postfixExpression = infixToPostfix(input);
	result = solve(postfixExpression);
	if(memoryMode) clipboard.push_back(result);
}


