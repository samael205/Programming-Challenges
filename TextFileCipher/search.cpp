#include "search.h"

void search(std::vector<std::string> & files, const fs::path &dirRoot){
	if(!fs::exists(dirRoot) || !fs::is_directory(dirRoot))
		return;
	fs::recursive_directory_iterator it(dirRoot);
	fs::recursive_directory_iterator end;
	while(it != end){
		if(fs::is_regular_file(*it) && it->path().extension() == ".txt")
			files.push_back(it->path().string());
		it++;
	}
}

std::vector<std::string> getTextFiles(const std::string & path){
	std::vector<std::string> files;
	search(files, path);
	return files;
}
