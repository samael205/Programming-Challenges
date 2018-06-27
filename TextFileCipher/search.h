#ifndef SEARCH_H_
#define SEARCH_H_

#include <boost/filesystem.hpp>
#include <vector>
#include <string>

namespace fs = ::boost::filesystem;

void search(std::vector<std::string>&, const fs::path&);
std::vector<std::string> getTextFiles(const std::string&);

#endif
