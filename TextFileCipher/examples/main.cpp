#include <iostream>
#include <string>
#include <vector>

#include "cipher.h"
#include "search.h"

using std::cin;
using std::cout;

int main(int argc, char ** argv){
	if(argc < 2) std::exit(EXIT_FAILURE);

	std::string pathToTextFiles, cipherMethod;
	
	pathToTextFiles.assign(argv[2]);
	cipherMethod.assign(argv[1]);

	auto files = getTextFiles(pathToTextFiles);

	Cipher cipher(files, cipherMethod);
	cipher.startEncrypt();
}
