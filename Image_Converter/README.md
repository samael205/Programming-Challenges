# Linux 

example for debian

We need export c++ path

'''sh
export CPLUS_INCLUDE_PATH=/usr/include/python2.7
'''

Now compile using g++

'''sh
g++ -std=c++11 -lpython2.7 -lboost_system -lboost_filesystem Image.cpp Image.h main.cpp -o run
'''

Be sure that your linux has installed boost libraries
'''sh
sudo aptitude install libboost-all-dev
'''
