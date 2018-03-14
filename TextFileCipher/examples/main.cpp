#include "cipher.h"
#include "filesearcher.h"

int main(void){
	FileSearcher findFiles;

	std::string pathToTextFiles;
	cout<<"Path to dir: "<<std::flush;
	std::getline(cin, pathToTextFiles);

	findFiles.search(pathToTextFiles);

	Cipher encryptFiles(findFiles.filesPath(), "rot13");
	encryptFiles.StartEncrypt();

	Cipher encryptFiles(findFiles.filesPath(), "caesar");
	encryptFiles.StartEncrypt();

	Cipher encryptFiles(findFiles.filesPath(), "vigenere");
	encryptFiles.StartEncrypt();
}