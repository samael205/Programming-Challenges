# Linux

example for debian

compile using g++

```sh
$ g++ -std=c++11 -lboost_system -lboost_filesystem cipher.cpp cipher.h filesearcher.cpp filesearcher.h main.cpp -o run
```

Be sure that your linux has installed boost libraries
```sh
$ sudo aptitude install libboost-all-dev
```
