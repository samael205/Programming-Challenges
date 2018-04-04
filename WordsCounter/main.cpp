#include <iostream>
#include <future>
#include <numeric>
#include <cstring>

#include "wordsCounter.h"

#define REP(i, n, v) for(int i=v; i<n; i++)

void findOptions(int argc, char ** argv);

int main(int argc, char *argv[]){
	if(argc < 2)
		std::exit(EXIT_FAILURE);

	findOptions(argc, argv);

	wc::filesContent content;

	REP(i, argc, 1){
		if(std::strlen(argv[i]) == 0)
			continue;

		std::ifstream file;
		file.open(argv[i]);

		if(!file.is_open()){
			std::cerr<<"Can't read "<<argv[i]<<" file!\n";
			continue;
		}

		std::string readContent = wc::getFileContent(file);
		file.close();

		content.push_back(readContent);
	}

	std::vector<std::future<void> > threads;
	for(auto it = content.begin(); it != content.end(); it++)
		threads.push_back(std::async(std::launch::async, wc::countWords, std::ref(*it)));

	for(auto it = threads.begin(); it != threads.end(); it++)
		it->get();		


	auto count = std::accumulate(wc::words.begin(), wc::words.end(), std::make_pair<int, int>(0, 0),
		wc::sumPair<int, int>());
	
	std::cout<<"Words:\t"<<count.first;	
	if(wc::readChars)
		std::cout<<"\tChars:\t"<<count.second;
	std::cout<<"\n";
}

void findOptions(int argc, char ** arguments){
	REP(i, argc, 1)
		if(std::strcmp(arguments[i], "--chars") == 0 ||
			std::strcmp(arguments[i], "-c") == 0){
			wc::readChars = true;
			arguments[i][0] = '\0';
			break;
		}
}

