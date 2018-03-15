#include "Image.h"

Images::Images() : filesPath(0), extensionsToConvert(0), extensionsOrNamesToSearch(0){}

Images::~Images(){}

void Images::search(const vs & dataToSearch, const fs::path & rootDirPath){
	if(!fs::exists(rootDirPath) || !fs::is_directory(rootDirPath))
		return;
	fs::recursive_directory_iterator it(rootDirPath);
	fs::recursive_directory_iterator end;
	while(it != end){
		FOREACH(searchImage, dataToSearch)
			if( fs::is_regular_file(*it) && (it->path().stem() == *searchImage 
				|| it->path().extension() == *searchImage || it->path().filename() == *searchImage))
				filesPath.push_back(it->path())	;		
		it++;
	}
	std::sort(filesPath.begin(), filesPath.end());
	filesPath.erase(std::unique(filesPath.begin(), filesPath.end()), filesPath.end());
}

void Images::get_data(std::string & data){
	extensionsOrNamesToSearch = split(data.substr(0, data.find_first_of(">")-1));
	extensionsToConvert = split(data.substr(data.find_first_of(">")+1, data.length()));
	FOREACH(extension, extensionsToConvert){
		*extension = DotFormat(*extension);
		std::transform(extension->begin(), extension->end(), extension->begin(), ::tolower);
	}	
	search(extensionsOrNamesToSearch);
}

vs Images::split(const std::string data){
	std::stringstream sstream(data);
	std::string temp;
	vs token;
	while(std::getline(sstream, temp, ' '))
		token.push_back(temp);
	return token;
}

std::string Images::DotFormat(std::string dotInExtension){
	if(dotInExtension[0] == '.')
		return dotInExtension.substr(1, dotInExtension.length());
	return dotInExtension;
}

ConvertImages::ConvertImages(){
	Py_Initialize();
	module = PyImport_Import(PyString_FromString("image_convert"));
	moduleDict = PyModule_GetDict(module);
	function = PyDict_GetItem(moduleDict, PyString_FromString("changeExtension"));
	arguments = PyTuple_New(2); 
}

ConvertImages::~ConvertImages(){
	Py_Finalize();
}

void Convert(const ConvertImages & convert, const Images & imagesToConvert){
	FOREACH(imageToConvert, imagesToConvert.filesPath){
		PyTuple_SetItem(convert.arguments, 0, PyString_FromString(imageToConvert->filename().string().c_str()));
		FOREACH(extensionToSet, imagesToConvert.extensionsToConvert){
			PyTuple_SetItem(convert.arguments, 1, PyString_FromString(extensionToSet->c_str()));
			PyObject_CallObject(convert.function, convert.arguments);
		}
	}	
}

