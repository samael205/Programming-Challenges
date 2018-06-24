#include "calculator.h"
#include <fstream>

Interface::Interface() : output(0), results(0), stack() {
	result = 0;
	std::ifstream checkFile("results.txt");
	if(!checkFile.is_open()){
		std::ofstream createResultsFile;
		createResultsFile.open("results.txt");
		createResultsFile.close();
	}
	checkFile.close();
}

Interface::~Interface() {
	
}

std::string Interface::buildPostfixExpression(const std::string & equation){
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

bool Interface::isOperator(const char & charToCheck){
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

bool Interface::precedence(const char & leftOperator, const char & rightOperator){
	if(leftOperator =='^')
		return true;
	else if(rightOperator == '^')
		return false;
	else if(leftOperator == '*' || leftOperator == '/' || leftOperator == '%')
		return true;
	else if(rightOperator == '*' || rightOperator == '/' || rightOperator == '%')
		return false;
}

double Interface::rpn(const std::string& expression){
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

const void Interface::show(bool muliResults) const{
	std::cout << "\033[2J\033[1;1H"
		  <<"\033[1;31m-: \033[0m"
		  <<"RPN Calculator v1.1";

	if(!muliResults)
		std::cout<<"\n"<<std::setw(15)<<"= "<<result;
	else{
		std::cout<<std::setw(7)<<results.size()<<"/"<<n<<"\n";
		REP(i, 50) std::cout<<"-";
		std::cout<<'\n';
		std::copy(results.begin(), results.end(), std::ostream_iterator<double>(std::cout, " "));
		std::cout<<'\n';	
		REP(i, 50) std::cout<<"-";		
	}
}

void Interface::calculate(std::string & expression,  bool muliResults){
	std::string::iterator it = std::unique(expression.begin(), expression.end(), [&](char left, char right){
		return left == ' ' && left == right;
	});

	expression.erase(it, expression.end());
	if(isAlpha(expression)) return;

	result = rpn(buildPostfixExpression(expression));
	if(muliResults && results.size() < n)
		results.push_back(result);
}

bool Interface::isAlpha(std::string stringToCheck){
	FOREACH(i, stringToCheck) if(std::isalpha(*i)) return true;
	return false;	
}

void Interface::saveResults(bool muliResults){
	if(results.size() == 0 && muliResults)
		return;

	std::ofstream file;
	file.open("results.txt", std::ios_base::app);
	if(muliResults)
		FOREACH(i, results)
			file<<*i<<" ";
	else
		file<<result;
	file<<"\n";
	file.close();	
	std::cout << "\033[2J\033[1;1H"
	          <<"Saved!\n";	 

	std::this_thread::sleep_for(std::chrono::milliseconds(350)); 
}

void Interface::clear(){
	results.clear();
	result = 0;
}

Calculator::Calculator() : Interface() {
	memoryMode = false;
}

Calculator::~Calculator() {

}

void const Calculator::showMenu() const{
	std::cout<<"\nE.\tWrite Expression\n"
		 <<"M.\tMemory Mode\n"
		 <<"S.\tSave\n"
		 <<"C.\tClear\n"
		 <<"X.\tExit\n"
		 <<"-: "<<std::flush;
}

void Calculator::calculate(char & choice){
	switch(std::tolower(choice)){
		case 'e':
		{
			std::cout<<"\nExpression: "<<std::flush;
			std::cin.clear();
			std::cin.ignore();
			std::string expression;
			std::getline(std::cin, expression);
			Interface::calculate(expression, memoryMode);
		}
		break;	
		case 'm':
			char userChoice;
			std::cout<<"\033[2J\033[1;1H";
			memoryMode ? std::cout<<"off" : std::cout<<"on";			
			std::cout<<" Memory mode? (Y/N) "<<std::flush;
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
			Interface::saveResults(memoryMode);
		break;
		case 'c':
			Interface::clear();
		break;
		case 'x':
			std::cout << "\033[2J\033[1;1H"
				  <<"See you later!\n";
			std::exit(EXIT_SUCCESS);	
		break;
		default:
			while(std::cin.get() != '\n')
				continue;
		break;
	}
}
