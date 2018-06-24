# Description

Search all text files in specific dir path

```cpp
std::vector<std::string> getTextFiles(const std::string & path){
	std::vector<std::string> files;
	search(files, path);
	return files;
}
```
encrypt them 

```cpp
std::string Cipher::encrypt(std::string & contentToEncrypt){
	std::string encrypted;
	switch(cipherMethod){
		case rot13:
			encrypted =  rot13Cipher(contentToEncrypt);
		break;
		case caesar:
			encrypted =  caesarCipher(contentToEncrypt);
		break;
		case vigenere:
			encrypted =  vigenereCipher(contentToEncrypt);
		break;
	}
	return encrypted;
}
```

Setting Cipher

```cpp
Cipher cipher(files, "vigenere");
cipher.startEncrypt();
```

# Installation

- Copy main.cpp from examples

- Compile

```sh
$ g++ -std=c++11 -lboost_system -lboost_filesystem -Wl,--allow-multiple-definition cipher.cpp cipher.h search.cpp main.cpp -o cipher
```

Be sure that your system has installed boost libraries
```sh
$ sudo aptitude install libboost-all-dev
```
