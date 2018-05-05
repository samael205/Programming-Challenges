#include "calculator.h"

int main(void){
	Calculator calculator;
	char operation;
	while(true){
		calculator.showCalculateResult();
		calculator.showMenu();
		std::cin>>operation;
		calculator.calculate(operation);
	}
}

