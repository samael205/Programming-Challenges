#include <iostream>
#include <future>
#include <numeric>

#include "wordsCounter.h"

#define REP(i, n, v) for(int i=v; i<n; i++)

int main(int argc, char *argv[]){
	if(argc < 2)
		std::exit(EXIT_FAILURE);

	wc::filesContent content;

	REP(i, argc, 1){
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

	int allWords = std::accumulate(wc::words.begin(), wc::words.end(), 0);
	std::cout<<"Words:\t"<<allWords<<'\n';
}