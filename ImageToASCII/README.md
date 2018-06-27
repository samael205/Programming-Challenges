# Description

Search all Image files with specific extension in current folder

```cpp
struct search{
	vp files;
	search() : files(0) { }

	void find_files(const std::string & search_file, const fs::path & = fs::current_path());
	void convert_files(py function_to_call, py arguments);

};
```
Convert them to ASCII art using python script

# Installation

- export python path

```sh
$ export CPLUS_INCLUDE_PATH=/usr/include/python2.7
```
- compile 

```sh
$ g++ -std=c++11 -lpython2.7 -lboost_system -lboost_filesystem Images.cpp -o converter
```

Be sure that your system has installed boost libraries

```sh
$ sudo aptitude install libboost-all-dev
```
