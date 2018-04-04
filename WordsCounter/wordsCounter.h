#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <mutex>

namespace wc{
	typedef std::vector<std::string> filesContent;
	typedef std::vector<int> counter;

	static counter words;

	std::mutex threadGuard;

	std::string getFileContent(std::ifstream & file){
		std::string bufor = "\0";
		std::string data = "\0";

		while(file >> bufor)	
			data += bufor + " ";
		
		return data;
	}

	void countWords(const std::string & content){
		std::stringstream sstream(content);
		int numberOfWords = std::distance(std::istream_iterator<std::string>(sstream), 
			std::istream_iterator<std::string>());

		threadGuard.lock();
		words.push_back(numberOfWords);
		threadGuard.unlock();
	}

}