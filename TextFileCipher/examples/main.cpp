#include <iostream>
#include <string>
#include <vector>

#include "cipher.h"
#include "search.cpp"

using std::cin;
using std::cout;

int main(int argc, char ** argv){
	std::string pathToTextFiles;
	if(argc < 2){
		cout<<"Path to dir: "<<std::flush;
		std::getline(cin, pathToTextFiles);
	}else
		pathToTextFiles.assign(argv[1]);

	auto files = getTextFiles(pathToTextFiles);

	Cipher cipher(files, "vigenere");
	cipher.startEncrypt();
}