#include "cipher.h"

Cipher::Cipher(){
	cipherMethod = vigenere;
}

Cipher::~Cipher() { }

const void Cipher::showMenu() const{
	cout<<"Cipher v0.1\n";
	cout<<"V.\tvigenere\n";
	cout<<"R.\trot13\n";
	cout<<"C.\tcaesar\n";
	cout<<"-: "<<std::flush;
}

void Cipher::setCipherMethod(){
	cout << "\033[2J\033[1;1H";
	char setMethod;
	showMenu();
	cin>>setMethod;
	switch(std::tolower(setMethod)){
		case 'v':
			cipherMethod = vigenere;
		break;
		case 'r':
			cipherMethod = rot13;
		break;
		case 'c':
			cipherMethod = caesar;
		break;
		default:
			while(cin.get() != '\n')
				continue;
		break;
	}
}

void Cipher::encryptFile(std::string & filePath){
	std::string content;
	std::ifstream file;
	file.open(filePath);
	content = fileContent(file); 
	file.close();
	std::ofstream saveFile;
	saveFile.open(filePath);
	saveFile << encrypt(content);
	saveFile.close();
	cout << "\033[2J\033[1;1H";
	cout<<"Done!\n";
}

void Cipher::encryptFiles(Vstring filesPath){
	FOREACH(filePath, filesPath)
		encryptFile(*filePath);		
}

std::string Cipher::encrypt(std::string & contentToEncrypt){
	std::string encrypted;
	switch(cipherMethod){
		case rot13:
			return rot13Cipher(contentToEncrypt);
		break;
		case caesar:
			return caesarCipher(contentToEncrypt);
		break;
		case vigenere:
			return vigenereCipher(contentToEncrypt);
		break;
	}
}

std::string Cipher::fileContent(std::ifstream & fileToRead){
	std::string bufor, allContent = "";
	while(std::getline(fileToRead, bufor))
		allContent += bufor + "\n";
	return allContent;
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