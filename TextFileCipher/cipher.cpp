#include "cipher.h"

static Cipher::CipherMethods encryptMethod(const std::string setMethod){
	if(setMethod == "rot13")
		return Cipher::CipherMethods::rot13;
	else if(setMethod == "caesar")
		return Cipher::CipherMethods::caesar;
	else if(setMethod == "vigenere")
		return Cipher::CipherMethods::vigenere;
	else
		return Cipher::CipherMethods::caesar;
}

Cipher::Cipher(const Vstring & filesPaths, const std::string method){
	filesToEncrypt = filesPaths;
	cipherMethod = encryptMethod(method);
}

Cipher::~Cipher() { }

void Cipher::setFiles(const Vstring & filesPaths){
	filesToEncrypt = filesPaths;
}

void Cipher::setEncryptMethod(const std::string method){
	cipherMethod = encryptMethod(method);
}

std::string Cipher::encrypt(std::string & contentToEncrypt){
	std::string encrypted;
	switch(cipherMethod){
		case rot13:
			encrypted = rot13Cipher(contentToEncrypt);
			break;
		case caesar:
			encrypted = caesarCipher(contentToEncrypt);
			break;
		case vigenere:
			encrypted = vigenereCipher(contentToEncrypt);
			break;
	}
	return encrypted;
}

void Cipher::encryptFile(const std::string & fileToEncrypt){
	std::cout<<"\t"<<fileToEncrypt<<" ";
	std::string content;
	std::ifstream file;
	file.open(fileToEncrypt);
	content = getFileContent(file); 
	file.close();
	std::ofstream saveFile;
	saveFile.open(fileToEncrypt);
	saveFile << encrypt(content);
	saveFile.close();
}

void Cipher::startEncrypt(){
	std::cout<<"Encrypt: \n";
	FOREACH(file, filesToEncrypt)
		encryptFile(*file);	
	std::cout<<"\nDone\n";		
}

std::string Cipher::getFileContent(std::ifstream & file){
	std::string content;
	content.assign(std::istreambuf_iterator<char>(file),
				   std::istreambuf_iterator<char>());
	return content;
}

std::string Cipher::caesarCipher(std::string & contentToEncrypt){
	static std::string letters = "abcdefghijklmnopqrstuvwxyz";
	std::string::size_type position;
	FOREACH(it, contentToEncrypt)
		if(!std::isspace(*it)){
			position = letters.find(std::tolower(*it));
			*it = letters[(static_cast<int>(position)+3)%26];		
		}
	return contentToEncrypt;
}

std::string Cipher::rot13Cipher(std::string & contentToEncrypt){
	static std::string const
		lcalph = "abcdefghijklmnopqrstuvwxyz",
    	ucalph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string encryptedContent;
    std::string::size_type position;
    encryptedContent.reserve(contentToEncrypt.length());
    FOREACH(it, contentToEncrypt){
    	if( (position = lcalph.find(*it)) != std::string::npos)
    		encryptedContent.push_back(lcalph[(position + 13)%26]);
    	else if( (position = ucalph.find(*it)) != std::string::npos)
    		encryptedContent.push_back(ucalph[(position + 13)%26]);
    	else
    		encryptedContent.push_back(*it);
    }	
    return encryptedContent;
}

std::string Cipher::vigenereCipher(std::string & contentToEncrypt){
	std::string encrypted,
	 key = makeVigenereKey(contentToEncrypt);
	for(int i = 0,  j = 0; i < contentToEncrypt.length(); i++){
		char checkChar = contentToEncrypt[i];
		if(checkChar >= 'a' && checkChar <= 'z')
			checkChar += 'A' - 'a';
		else if(checkChar < 'A' || checkChar > 'Z')
			continue;
		encrypted += (checkChar += key[j] - 2*'A') % 26 + 'A';
		j = (j + 1) % key.length();
	}
	return encrypted;
}

std::string Cipher::makeVigenereKey(std::string key){
	std::string createdKey = "";
	REP(i, key.size()){
		if(key[i] >= 'A' && key[i] <= 'Z')
			createdKey += key[i];
		else if(key[i] >= 'a' && key[i] <= 'z')
			createdKey += key[i] + 'A' - 'a';
	}
	return createdKey;
}