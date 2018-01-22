#include "cipher.h"
#include "filesearcher.h"

int main(void){
	FileSearcher findFiles;
	Cipher encrypt;

	encrypt.setCipherMethod();

	cin.clear();
	cin.ignore();
	std::string pathToTextFiles;
	
	cout<<"Path to dir: "<<std::flush;
	std::getline(cin, pathToTextFiles);

	findFiles.search(pathToTextFiles);
	encrypt.encryptFiles(findFiles.filesPath());
}