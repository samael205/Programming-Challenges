#ifndef CIPHER_H_
#define CIPHER_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <istream>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

typedef std::vector<std::ofstream> Vostream;
typedef std::vector<std::string> Vstring;

class Cipher{
public:
	enum CipherMethods{ vigenere, rot13, caesar };
	CipherMethods cipherMethod;

	Cipher(const Vstring&, const std::string);
	~Cipher();

	void setFiles(const Vstring & path);
	void setEncryptMethod(const std::string method);
	void encryptFile(const std::string&);
	void startEncrypt();
	std::string getFileContent(std::ifstream&);
protected:
	std::string encrypt(std::string&);
	std::string vigenereCipher(std::string&);
	std::string makeVigenereKey(std::string);
	std::string caesarCipher(std::string&);
	std::string rot13Cipher(std::string&);
private:
	Vstring filesToEncrypt;
};

#endif 