#include "wordscounter.h"

std::string WordsCounter::getFileContent(std::ifstream & file){
    std::string data;
    data.assign(std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>());
    return data;
}
	
std::string WordsCounter::countWords(std::ifstream & file){
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
        summary << std::setw(4)<<numberOfChars;
    }
    if(readNewLines){
        file.clear();
        file.seekg(0, std::ios_base::beg);
        numberOfNewLines = std::count(std::istreambuf_iterator<char>(file), 
                                      std::istreambuf_iterator<char>(),
                                      '\n');
        allNewLines += numberOfNewLines;
        summary<<std::setw(4)<<numberOfNewLines;
    }
    if(readBytes){
        file.clear();
        file.seekg(0, std::ios_base::end);
        numberOfBytes = file.tellg();
        allBytes += numberOfBytes;
        summary<<std::setw(4)<<numberOfBytes;
    }
    return summary.str();
}

const std::string WordsCounter::totalSummary(){
    std::stringstream summaryTotal;

    summaryTotal<<std::setw(4)<<allWords;
    if(readChars) summaryTotal<<std::setw(4)<<allChars;
    if(readNewLines) summaryTotal<<std::setw(4)<<allNewLines;
    if(readBytes) summaryTotal<<std::setw(4)<<allBytes;
    summaryTotal<<"\ttotal\n";

    return summaryTotal.str();
}
