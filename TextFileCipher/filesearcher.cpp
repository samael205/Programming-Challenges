#include "filesearcher.h"
#include <iostream>

FileSearcher::FileSearcher() : filePaths(0) {}

FileSearcher::~FileSearcher() {}

void FileSearcher::search(const fs::path &dirRoot){
	if(!fs::exists(dirRoot) || !fs::is_directory(dirRoot))
		return;
	fs::recursive_directory_iterator it(dirRoot);
	fs::recursive_directory_iterator end;
	while(it != end){
		if(fs::is_regular_file(*it) && it->path().extension() == ".txt")
			filePaths.push_back(it->path().string());
		it++;
	}
}
