#include "calculator.h"

int main(void){
	Interface calculator;
	char operation;
	for(;;){
		calculator.showCalculateResult();
		calculator.showInterface();
		std::cin>>operation;
		calculator.doOperations(operation);
	}
}

