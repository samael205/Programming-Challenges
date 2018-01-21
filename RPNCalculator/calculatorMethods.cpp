#include "calculator.h"

#include <fstream>
#include <chrono>
#include <thread>

Calculator::Calculator() : output(0), results(0), stack() {
	result = 0;
	std::ifstream checkFile("results.txt");
	if(!checkFile.good()){
		std::ofstream createResultsFile;
		createResultsFile.open("results.txt");
		createResultsFile.close();
	}
	checkFile.close();
}

Calculator::~Calculator() {}

std::string Calculator::buildPostfixExpression(const std::string & equation){
	std::stringstream postfix;
	std::stack<char> stack;
	stack.push('(');
	REP(i, equation.size()){
		const char currentChar = equation[i];
		if(std::isspace(currentChar)){}
		else if(std::isalnum(currentChar) || '.' == currentChar)
			postfix << currentChar;
		else if('(' == currentChar)
			stack.push(currentChar);
		else if(isOperator(currentChar)){
			char rightOperator = currentChar;
			while(!stack.empty() && isOperator(stack.top()) && precedence(stack.top(), rightOperator)){
				postfix<<' '<<stack.top();
				stack.pop();
			}
			postfix<<' ';
			stack.push(rightOperator);
		}
		else if(')' == currentChar){
			while(!stack.empty() && '(' != stack.top()){
				postfix<<' '<<stack.top();
				stack.pop();
			}
			if(stack.empty())
				throw std::runtime_error("missing left operator!");
			stack.pop();
			postfix<<' ';
		}else
			throw std::runtime_error("invalid operator!");			
	}
	while(!stack.empty() && '(' != stack.top()){
		postfix<<' '<<stack.top();
		stack.pop();
	}
	if(stack.empty())
		throw std::runtime_error("missing left operator!");
	stack.pop();
	if(!stack.empty())
		throw std::runtime_error("missing right operator!");
	return postfix.str();	
}

bool Calculator::isOperator(const char & charToCheck){
	switch(charToCheck){
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

bool Calculator::precedence(const char & leftOperator, const char & rightOperator){
	if(leftOperator =='^')
		return true;
	else if(rightOperator == '^')
		return false;
	else if(leftOperator == '*' || leftOperator == '/' || leftOperator == '%')
		return true;
	else if(rightOperator == '*' || rightOperator == '/' || rightOperator == '%')
		return false;
}

double Calculator::rpn(const std::string& expression){
	std::istringstream iss(expression);
	std::vector<double> stack;
	std::string token;
	while(iss >> token){
		double tokenNumber;
		if(std::istringstream(token) >> tokenNumber)
			stack.push_back(tokenNumber);
		else{
			double firstOperand, secondOperand;
			secondOperand = stack.back();
			stack.pop_back();
			firstOperand = stack.back();
			stack.pop_back();
			if(token == "*")
				stack.push_back(firstOperand * secondOperand);
			else if(token == "/")
				stack.push_back(firstOperand / secondOperand);
			else if(token == "-")
				stack.push_back(firstOperand - secondOperand);
			else if(token == "+")
				stack.push_back(firstOperand + secondOperand);
			else if(token == "^")
				stack.push_back(std::pow(firstOperand, secondOperand));
			else
				throw std::runtime_error("Error!");
		}	
	}
	return stack.back();
}

const void Calculator::show(bool muliResults) const{
	std::cout << "\033[2J\033[1;1H";
	std::cout<<"\033[1;31m-: \033[0m";
	std::cout<<"RPN Calculator v1.0";
	if(!muliResults)
		std::cout<<"\n"<<std::setw(15)<<"= "<<result;
	else{
		std::cout<<std::setw(7)<<results.size()<<"/"<<n<<"\n";
		REP(i, 50)
			std::cout<<"-";
		std::cout<<'\n';
		std::copy(results.begin(), results.end(), std::ostream_iterator<double>(std::cout, " "));
		std::cout<<'\n';	
		REP(i, 50)
			std::cout<<"-";		
	}
}

void Calculator::calculate(std::string & expression,  bool muliResults){
	std::string::iterator it = std::unique(expression.begin(), expression.end(), isBothSpace);
	expression.erase(it, expression.end());
	if(isAlpha(expression))
		return;
	result = rpn(buildPostfixExpression(expression));
	if(muliResults && results.size() < n)
		results.push_back(result);
}

bool Calculator::isAlpha(std::string stringToCheck){
	FOREACH(i, stringToCheck)
		if(std::isalpha(*i))
			return true;
	return false;	
}

void Calculator::saveResults(bool muliResults){
	std::ofstream file;
	file.open("results.txt", std::ios_base::app);
	if(muliResults)
		FOREACH(i, results)
			file<<*i<<" ";
	else
		file<<result;
	file<<"\n";
	file.close();	
	std::cout << "\033[2J\033[1;1H";
	std::cout<<"SAVED!\n";	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	 
}

void Calculator::clear(){
	results.clear();
	result = 0;
}

Interface::Interface() : Calculator() {
	memoryMode = false;
}

Interface::~Interface() {}

void const Interface::showInterface() const{
	std::cout<<"\nE.\tWrite Expression\n";
	std::cout<<"M.\tMemory Mode\n";
	std::cout<<"S.\tSave\n";
	std::cout<<"C.\tClear\n";
	std::cout<<"X.\tExit\n";
	std::cout<<": "<<std::flush;
}

void Interface::doOperations(char & choice){
	switch(std::tolower(choice)){
		case 'e':
		{
			std::cout<<"\nWrite expression: "<<std::flush;
			std::cin.clear();
			std::cin.ignore();
			std::string expression;
			std::getline(std::cin, expression);
			Calculator::calculate(expression, memoryMode);
		}
		break;	
		case 'm':
			char userChoice;
			std::cout << "\033[2J\033[1;1H";
			std::cout<<"Memory Mode ";
			if(memoryMode)
				std::cout<<"on";
			else
				std::cout<<"off";
			std::cout<<", should it ";
			if(memoryMode)
				std::cout<<"off";
			else
				std::cout<<"on";
			std::cout<<" now? (Y/N) "<<std::flush;
			std::cin>>userChoice;
			switch(std::tolower(userChoice)){
				case 'y':
					memoryMode = !memoryMode;
				break;
				case 'n':
				break;
				default:
					while(std::cin.get() != '\n' && !(std::cin))
						continue;
				break;
			}		
		break;
		case 's':
			Calculator::saveResults(memoryMode);
		break;
		case 'c':
			Calculator::clear();
		break;
		case 'x':
			std::cout << "\033[2J\033[1;1H";
			std::cout<<"See you later!\n";
			std::exit(EXIT_SUCCESS);	
		break;
		default:
			while(std::cin.get() != '\n')
				continue;
		break;
	}
}
