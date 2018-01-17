#include <iostream>
#include <Python.h>
#include <boost/filesystem.hpp>
#include <vector>
#include <iomanip>

namespace fs = ::boost::filesystem;

#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

typedef PyObject * py;
typedef std::vector<fs::path> vp;

struct search{
	vp files;
	search() : files(0) { }

	void find_files(const std::string & search_file, const fs::path & = fs::current_path());
	void convert_files(py function_to_call, py arguments);
	const void result() const;

};

int main(void){
	setenv("PYTHONPATH", ".", 1);
	Py_Initialize();
	py module = PyImport_ImportModule("image_to_ascii");
	py call = PyModule_GetDict(module);
	py arguments = PyTuple_New(1); 
	py function = PyDict_GetItem(call, PyString_FromString("convert"));
	std::string file_extension;
	std::cout<<"Write file extension: "<<std::flush;
	std::cin>>file_extension;
	if(file_extension[0] != '.')
		file_extension = "." + file_extension;
	search test;
	test.find_files(file_extension);
	test.result();
	test.convert_files(function, arguments);
	Py_Finalize();
}

void search::convert_files(py function_to_call, py arguments){
	FOREACH(i, files){
		PyTuple_SetItem(arguments, 0, PyString_FromString(i->string().c_str()));
		std::cout<<"Converting "<<i->filename();
		PyObject_CallObject(function_to_call, arguments);
		std::cout<<"\tDone\n";
	}
}

void search::find_files(const std::string & search_file, const fs::path & root){
	if(!fs::exists(root) || !fs::is_directory(root))
		return;
	fs::recursive_directory_iterator it(root);
	fs::recursive_directory_iterator end;
	while(it != end){
		if(fs::is_regular_file(*it) && it->path().extension() == search_file)
			files.push_back(it->path());
		it++;
	}
}

const void search::result() const{
	std::cout<<std::setw(15)<<"Found files:\n";
	if(files.size() == 0)
		std::cout<<std::setw(15)<<"None\n";
	FOREACH(i, files)
		std::cout<<std::setw(15)<<i->filename()<<'\n';
	std::cout<<'\n';	
}
