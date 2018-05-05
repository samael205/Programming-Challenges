# Description

Search all Image files with specific extension or names in current dir

```cpp
void Images::search(const vs & dataToSearch, const fs::path & rootDirPath);
```
and call python script to create copy current file with preferred extension

```cpp
void Convert(const ConvertImages & convert, const Images & imagesToConvert){
	FOREACH(imageToConvert, imagesToConvert.filesPath){
		PyTuple_SetItem(convert.arguments, 0, PyString_FromString(imageToConvert->filename().string().c_str()));
		FOREACH(extensionToSet, imagesToConvert.extensionsToConvert){
			PyTuple_SetItem(convert.arguments, 1, PyString_FromString(extensionToSet->c_str()));
			PyObject_CallObject(convert.function, convert.arguments);
		}
	}	
}
```

# Installation 

- Copy main.cpp file from examples 

- export python path

```sh
$ export CPLUS_INCLUDE_PATH=/usr/include/python2.7
```
- Compile

```sh
$ g++ -std=c++11 -lpython2.7 -lboost_system -lboost_filesystem Image.cpp Image.h main.cpp -o run
```
Be sure that your system has installed boost libraries

```sh
$ sudo aptitude install libboost-all-dev
```
