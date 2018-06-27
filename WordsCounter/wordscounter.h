#ifndef WORDSCOUNTER_H_
#define WORDSCOUNTER_H_

#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

typedef struct{
	int allWords = 0;
	int allChars = 0;
	int allBytes = 0;
	int allNewLines = 0;

	bool readChars = false;
	bool readBytes = false;
	bool readNewLines = false;

	int numberOfNewLines, numberOfWords, numberOfChars, numberOfBytes;
	std::vector<std::string> result;

	std::string getFileContent(std::ifstream&);
	std::string countWords(std::ifstream&);
	const std::string totalSummary();
} WordsCounter;

#endif
