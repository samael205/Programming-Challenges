#include <iostream>
#include <string>
#include <vector>

#include "cipher.h"
#include "search.cpp"

using std::cin;
using std::cout;

int main(int argc, char ** argv){
	if(argc < 2) std::exit(EXIT_FAILURE);

	std::string pathToTextFiles, cipherMethod;
	
	pathToTextFiles.assign(argv[1]);
	cipherMethod.assign(argv[2]);

	auto files = getTextFiles(pathToTextFiles);

	Cipher cipher(files, cipherMethod);
	cipher.startEncrypt();
}
