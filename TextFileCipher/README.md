# Description

Search all text files in specific dir path

```cpp
void FileSearcher::search(const fs::path &dirRoot){
	if(!fs::exists(dirRoot) || !fs::is_directory(dirRoot))
		return;
	fs::recursive_directory_iterator it(dirRoot);
	fs::recursive_directory_iterator end;
	while(it != end){
		if(fs::is_regular_file(*it) && it->path().extension() == ".txt")
			filePaths.push_back(it->path().string());
		it++;
	}
}
```
and encrypt them 

```cpp
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
```
in Cipher constructor you set a cipher method 

```cpp
cipher encryptFiles(findFiles.filesPath(), "vigenere");
encryptFiles.StartEncrypt();
```

# Installation

- Copy main.cpp from examples

- Compile

```sh
$ g++ -std=c++11 -lboost_system -lboost_filesystem cipher.cpp cipher.h filesearcher.cpp filesearcher.h main.cpp -o encryptTextFiles
```

Be sure that your system has installed boost libraries
```sh
$ sudo aptitude install libboost-all-dev
```
