#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <mutex>
#include <utility>
#include <algorithm>
#include <cctype>

namespace wc{
	typedef std::vector<std::string> filesContent;
	typedef std::vector<std::pair<int, int>>counter;

	static counter words;

	std::mutex threadGuard;

	bool readChars = false;

	std::string getFileContent(std::ifstream & file){
		std::string bufor = "\0";
		std::string data = "\0";

		while(file >> bufor)	
			data += bufor + " ";
		
		return data;
	}

	template <class T1, class T2>
	struct sumPair : public std::binary_function<std::pair<T1, T2>, std::pair<T1, T2>, std::pair<T1, T2>>{
		std::pair<T1, T2> operator()(const std::pair<T1, T2>&a, const std::pair<T1, T2>&b){
			return std::pair<T1, T2>(a.first + b.first, a.second + b.second);
		}
	};

	void countWords(const std::string & content){
		std::stringstream sstream(content);
		int numberOfWords = std::distance(std::istream_iterator<std::string>(sstream), 
			std::istream_iterator<std::string>());

		int numberOfChars = std::count_if(content.begin(), content.end(), [](char c){
			return std::isalpha(c);
		});

		threadGuard.lock();
		words.push_back(std::pair<int, int>(numberOfWords, numberOfChars));
		threadGuard.unlock();
	}
}