#include "workers.h"
#include <cctype>
#include <boost/algorithm/string.hpp>

Workers::Workers(){
	name = occupation = infoAboutWorkers = "\0";
}

Workers::~Workers() {}

string Workers::getName(){
	std::fstream names;
	names.open("names.txt");
	if(!names.is_open())
		exit(EXIT_FAILURE);
	REP(i, rand()%200+1)
		std::getline(names, name);
	names.close();	
	return name;		
}

string Workers::getOccupation(){
	std::fstream occupations;
	occupations.open("occupation.txt");
	if(!occupations.is_open())
		exit(EXIT_FAILURE);
	REP(i, rand()%483+1)
		std::getline(occupations, occupation);
	occupations.close();
	return occupation;	
}

void Workers::generateWorkers(int n_workers){
	REP(i, n_workers)
		infoAboutWorkers += getName() + ";" + getOccupation() + ";" + std::to_string(earning()) + "\n";	
	boost::erase_all(infoAboutWorkers, "  ");
}

void Workers::saveInfoAboutWorkers(){
	std::fstream workersInfo;
	workersInfo.open("workers.txt", std::ios_base::app);
	workersInfo << infoAboutWorkers;
	workersInfo.close();
	infoAboutWorkers.clear();
}