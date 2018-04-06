#include <iostream>
#include <future>
#include <numeric>
#include <cstring>

#include "wordsCounter.h"

#define REP(i, n, v) for(int i=v; i<n; i++)
#define FOREACH(it, v) for(auto it = v.begin(); it != v.end(); it++)

void Options(int argc, char ** argv);

int main(int argc, char *argv[]){
	std::ios_base::sync_with_stdio(false);

	if(argc < 2)
		std::exit(EXIT_FAILURE);

	Options(argc, argv);

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
	FOREACH(it, content)
		threads.push_back(std::async(std::launch::async, wc::countWords, std::ref(*it)));

	FOREACH(it, threads)
		it->get();		

	auto count = std::accumulate(wc::words.begin(), wc::words.end(), std::make_pair<int, int>(0, 0),
		wc::sumPair<int, int>());
	
	std::cout<<"Words:\t"<<count.first;	
	if(wc::readChars)
		std::cout<<"\tChars:\t"<<count.second;
	if(wc::readNewLines)
		std::cout<<"\tLines:\t"<<wc::newLines;
	std::cout<<"\n";
}

void Options(int argc, char ** arguments){
	REP(i, argc, 1){
		if(std::strcmp(arguments[i], "--chars") == 0 ||
			std::strcmp(arguments[i], "-c") == 0){
			wc::readChars = true;
			arguments[i][0] = '\0';
		} 
		if(std::strcmp(arguments[i], "--lines") == 0 ||
			std::strcmp(arguments[i], "-l") == 0){
			wc::readNewLines = true;
			arguments[i][0] = '\0';
		}
	}
}

