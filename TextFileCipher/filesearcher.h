#ifndef FILESEARCHER_H_
#define FILESEARCHER_H_

#include <boost/filesystem.hpp>
#include <vector>
#include <string>

namespace fs = ::boost::filesystem;

typedef std::vector<std::string> Vstring;

class FileSearcher{
private:
	Vstring filePaths;
public:
	FileSearcher();
	~FileSearcher();

	Vstring filesPath() { return filePaths; }

	void search(const fs::path &dirRoot);
};


#endif