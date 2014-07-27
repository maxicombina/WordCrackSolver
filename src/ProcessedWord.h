/* 
 * File:   ProcessedWord.h
 * Author: Maxi Combina
 *
 * Created on May 31, 2014, 12:03 AM
 */

#ifndef PROCESSEDWORD_H
#define	PROCESSEDWORD_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <stdint.h>

#include "Node.h"

class ProcessedWord {
public:
    ProcessedWord(std::vector<Node*> path);
//    ProcessedWord(const ProcessedWord& orig);
    virtual ~ProcessedWord();
    std::string comparableWord();
    std::string prettyWord();
    int value();    
    std::pair<uint8_t, uint8_t> coordOfNodeInPos(uint8_t);
    static bool wordCmp(ProcessedWord, ProcessedWord);
    void deleteWord(); // Yes, ugly to put a deletor here. 
    
private:
    void setupLetterValues(void);
    void setupLengthExtraPoints(void);
    std::string _comparable_word;
    std::string _pretty_word;
    int _value;
    std::vector<Node *> _path;
    std::map<char, int> _letterValues;
    std::map<int, int> _lenExtraPoints;
};

#endif	/* PROCESSEDWORD_H */

