#ifndef IMAGE_H_
#define IMAGE_H_

#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <Python.h>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

namespace fs = ::boost::filesystem;

typedef std::vector<fs::path> vp;
typedef std::vector<std::string> vs;
typedef PyObject * py;

class Images;

class ConvertImages{
private:
	py module;
	py moduleDict;
	py function;
	py arguments;
public:
	ConvertImages();
	~ConvertImages();

	friend void Call(const ConvertImages &, const Images &);

protected:

};

class Images{
private:
	vp filesPath;
	vs extensionsToConvert;
	vs extensionsOrNamesToSearch;
public:
	Images();
	~Images();

	void get_data(std::string &);
	friend void Call(const ConvertImages &, const Images &);
protected:
	vs split(const std::string);
	void search(const vs &,  const fs::path & rootDirPath = fs::current_path());
	std::string DotFormat(std::string);
};


#endif