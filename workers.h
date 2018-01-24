#ifndef WORKERS_H_
#define WORKERS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i!= x.end(); i++)

using std::cout;
using std::string;

class Workers{
private:
	string infoAboutWorkers;
	string name, occupation;
public:
	Workers();
	~Workers();
	
	string getName();
	string getOccupation();

	void saveInfoAboutWorkers();
	void generateWorkers(int=100);
protected:
	int earning() const { return rand()%15000+3000; }
};


#endif