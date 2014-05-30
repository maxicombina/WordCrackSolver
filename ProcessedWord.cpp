/* 
 * File:   ProcessedWord.cpp
 * Author: maxlo
 * 
 * Created on May 31, 2014, 12:03 AM
 */

#include <string>
#include <utility>
#include <stdint.h>
#include <sstream>

#include "ProcessedWord.h"

void ProcessedWord::setupLetterValues(void)
{
    _letterValues.insert(std::pair<char, int>('a', 1));
    _letterValues.insert(std::pair<char, int>('b', 3));
    _letterValues.insert(std::pair<char, int>('c', 3));
    _letterValues.insert(std::pair<char, int>('d', 2));
    _letterValues.insert(std::pair<char, int>('e', 1));
    _letterValues.insert(std::pair<char, int>('f', 4));
    _letterValues.insert(std::pair<char, int>('g', 2));
    _letterValues.insert(std::pair<char, int>('h', 4));
    _letterValues.insert(std::pair<char, int>('i', 1));
    _letterValues.insert(std::pair<char, int>('j', 1)); // @todo: recheck
    _letterValues.insert(std::pair<char, int>('k', 1)); // MISSING?
    _letterValues.insert(std::pair<char, int>('l', 1));
    _letterValues.insert(std::pair<char, int>('m', 3));
    _letterValues.insert(std::pair<char, int>('n', 1));
    _letterValues.insert(std::pair<char, int>('#', 8)); //  ñ
    _letterValues.insert(std::pair<char, int>('o', 1));
    _letterValues.insert(std::pair<char, int>('p', 3));
    _letterValues.insert(std::pair<char, int>('q', 5));
    _letterValues.insert(std::pair<char, int>('r', 1));
    _letterValues.insert(std::pair<char, int>('s', 1));
    _letterValues.insert(std::pair<char, int>('t', 1));
    _letterValues.insert(std::pair<char, int>('u', 1));
    _letterValues.insert(std::pair<char, int>('v', 4));
    _letterValues.insert(std::pair<char, int>('w', 1)); // MISSING?
    _letterValues.insert(std::pair<char, int>('x', 8));
    _letterValues.insert(std::pair<char, int>('y', 4));
    _letterValues.insert(std::pair<char, int>('z', 10));
}
ProcessedWord::ProcessedWord(std::vector<Node*> path) {
    this->setupLetterValues();
    
    _path = path;

    // Init internal word and value
    std::stringstream comparable("");
    std::stringstream pretty("");
    int value = 0;
    for (int i = 0; i < path.size(); i++){
        value += _letterValues[path[i]->letter()];
        if (path[i]->letter() == 'q'){
            comparable << "qu";
            pretty << "qu";
        } else if (path[i]->letter() == '#') {
            comparable << "#";
            pretty << "ñ";
            
        } else {
            comparable << path[i]->letter();
            pretty << path[i]->letter();
        }
    }

    _comparable_word = comparable.str();
    _pretty_word = pretty.str();
    _value = value;
}

//ProcessedWord::ProcessedWord(const ProcessedWord& orig) {
//}

ProcessedWord::~ProcessedWord() {
}

std::string ProcessedWord::comparableWord()
{
    return _comparable_word;
}

std::string ProcessedWord::prettyWord()
{
    return _pretty_word;
}

int ProcessedWord::value()
{
    return _value;
}

std::pair<uint8_t, uint8_t> ProcessedWord::coordOfNodeInPos(uint8_t pos)
{
    return std::pair<uint8_t, uint8_t>(_path[pos]->row(), _path[pos]->col());
}

 bool ProcessedWord::wordCmp(ProcessedWord firstWord, ProcessedWord secondWord)
 {
     // Order higer -> lower
     return firstWord.value() > secondWord.value();
 }
 