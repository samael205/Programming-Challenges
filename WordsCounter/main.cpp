#include <iostream>
#include <cstring>

#include "wordsCounter.h"

#define REP(i, n, v) for(int i=v; i<n; i++)
#define FOREACH(it, v) for(auto it = v.begin(); it != v.end(); it++)

void CheckIfUserAskForHelp(int argc, char ** argv);
void Options(int argc, char ** argv);

int main(int argc, char *argv[]){
	std::ios_base::sync_with_stdio(false);

	if(argc < 2) std::exit(EXIT_FAILURE);

	CheckIfUserAskForHelp(argc, argv);
	Options(argc, argv);

	wc::result filesResult;

	REP(i, argc, 1){
		if(std::strlen(argv[i]) == 0) continue;

		std::ifstream file;
		file.open(argv[i], std::ios_base::binary);

		if(!file.is_open()){
			std::cerr<<"Can't read "<<argv[i]<<" file!\n";
			continue;
		}

		std::stringstream fileSummary;
		fileSummary<<wc::countWords(file);
		file.close();
		fileSummary<<'\t'<<std::string(argv[i]);

		filesResult.push_back(fileSummary.str());
	}

	FOREACH(it, filesResult)
		std::cout<<*it<<"\n";

	if(filesResult.size() > 1) std::cout<<wc::totalSummary();				
}

void CheckIfUserAskForHelp(int argc, char ** arguments){
	REP(i, argc, 1){
		if(std::strcmp(arguments[i], "--help") == 0 ||
			std::strcmp(arguments[i], "-h") == 0){
			std::cout << "\e[1m-m, --chars\n\t\e[0mprint the character counts"
					  << "\n\e[1m-l, --lines\n\t\e[0mprint the new lines  counts"
					  <<"\n\e[1m-c, --bytes\n\t\e[0mprint the bytes counts\n";
			std::exit(EXIT_SUCCESS);
		}
	}
}

void Options(int argc, char ** arguments){
	REP(i, argc, 1){
		if(std::strcmp(arguments[i], "--chars") == 0 ||
			std::strcmp(arguments[i], "-m") == 0){
			wc::readChars = true;
			arguments[i][0] = '\0';
		} 
		if(std::strcmp(arguments[i], "--lines") == 0 ||
			std::strcmp(arguments[i], "-l") == 0){
			wc::readNewLines = true;
			arguments[i][0] = '\0';
		}
		if(std::strcmp(arguments[i], "-c") == 0 ||
			std::strcmp(arguments[i], "--bytes") == 0){
			wc::readBytes = true;
			arguments[i][0] = '\0';
		}
	}
}

