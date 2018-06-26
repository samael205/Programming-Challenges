#include <iostream>

#include "calculator.h"

int main(void){
	Box box;
	std::string input;
	for(;;){
		std::cout<<"\033[2J\033[1;1H"
			 <<"\033[1;31mCalculator v0.1\033[0m\n";
		box.show();
		std::getline(std::cin, input);	
		box.exec(input);
	}
}
