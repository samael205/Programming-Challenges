#ifndef CIPHER_H_
#define CIPHER_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

using std::cout;
using std::cin;

typedef std::vector<std::ofstream> Vostream;
typedef std::vector<std::string> Vstring;

class Cipher{
private:
	enum CipherMethods{ vigenere, rot13, caesar };
	CipherMethods cipherMethod;
public:
	Cipher();
	~Cipher();

	void setCipherMethod();
	void encryptFile(std::string&);
	void encryptFiles(Vstring);

protected:
	std::string fileContent(std::ifstream&);
	
	std::string encrypt(std::string&);
	std::string vigenereCipher(std::string&);
	std::string makeVigenereKey(std::string);
	std::string caesarCipher(std::string&);
	std::string rot13Cipher(std::string&);
	const void showMenu() const;
};


#endif 