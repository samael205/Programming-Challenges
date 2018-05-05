#include "Image.h"

int main(void){
	setenv("PYTHONPATH", ".", 1);
	
	Searcher getImages;
	Converter convertThen;
	std::string instruction;
	std::cout<<"example: \033[1;31mimage.jpeg .png > .bmp \033[0m\n"
			 <<"Write files or extension to convert: "<<std::flush;

	std::getline(std::cin, instruction);
	getImages.get_data(instruction);

	Convert(convertThen, getImages);	
}