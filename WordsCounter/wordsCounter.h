#include <bits/stdc++.h>

namespace wc{
	typedef std::vector<std::string> result;

	static int allWords = 0;
	static int allChars = 0;
	static int allNewLines = 0;

	bool readChars = false;
	bool readNewLines = false;

	int numberOfNewLines;
	int numberOfWords;
	int numberOfChars;

	std::string getFileContent(std::ifstream & file){
		std::string bufor = "\0";
		std::string data = "\0";

		while(file >> bufor){	
			data += bufor + " ";
		}
		
		return data;
	}

	std::string countWords(std::ifstream & file){
		auto content = getFileContent(file);

		std::stringstream sstream(content);
		numberOfWords = std::distance(std::istream_iterator<std::string>(sstream), 
			std::istream_iterator<std::string>());
		allWords += numberOfWords;

		std::stringstream summary;
		summary <<numberOfWords;

		if(readChars){
			numberOfChars = std::count_if(content.begin(), content.end(), [=](char c){
				return std::isalpha(c);	
			});
			allChars += numberOfChars;
			summary << " "<<numberOfChars;
		}

		if(readNewLines){
			file.clear();
			file.seekg(0, std::ios_base::beg);

			numberOfNewLines = std::count(std::istreambuf_iterator<char>(file), 
					std::istreambuf_iterator<char>(), '\n');
			allNewLines += numberOfNewLines;

			summary<<" "<<numberOfNewLines;
		}

		return summary.str();
	}

	std::string totalSummary(){
		std::stringstream summaryTotal;

		summaryTotal << allWords;
		if(readChars)
			summaryTotal << " "<<allChars;
		if(readNewLines)
			summaryTotal << " "<<allNewLines;
		summaryTotal<<" total\n";

		return summaryTotal.str();
	}

}
