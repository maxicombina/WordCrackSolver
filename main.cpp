/* 
 * File:   main.cpp
 * Author: maxlo
 *
 * Created on May 27, 2014, 6:34 PM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <strings.h>

#include "Node.h"
#include "Board.h"
#include "BoardManager.h"
#include "ProcessedWord.h"
#include "debug.h"
using namespace std;

// Board Size
#define BOARD_ROWS 4
#define BOARD_COLS 4


std::string ngbsToString(std::vector<Node*> v)
{
    if (v.size() == 0) {
        return "[]";
    }
    stringstream s("");
    s << "[";
    for (int i = 0; i < v.size()-1; i++) {
        s << v[i]->letter();
        s << ", ";
    }
    s << v[v.size()-1]->letter();
    s << "]";
    return s.str();
}
std::string pathsToString(std::vector<std::vector<Node*> > v)
{
    if (v.size() == 0) {
        return "<<>>";
    }
    stringstream s("");
    s << "<<";
    for (int i = 0; i < v.size()-1; i++) {
        s << ngbsToString(v[i]);
        s << ", ";
    }
    s << ngbsToString(v[v.size()-1]);
    
    s << ">>";
    return s.str();
}

unordered_map<string, int> loadDictionary(const string& wordListFile)
{
    string line;
    unordered_map<string, int> dictionary;

    ifstream myfile (wordListFile.c_str());
    //ifstream myfile ("example.txt");
    if (myfile.is_open()) {
        while ( getline (myfile, line) ) {
            //cout << "["<< line << "]" << endl;
            dictionary.insert(pair<string, int>(line, 1)); // Dummy value
        }
        myfile.close();
    } else {
        cerr << "error opening " << wordListFile << endl;
    }
    
    return dictionary;

}

vector<pair<uint8_t, uint8_t>> parseCoordinateList(const string& list)
{
    vector<pair<uint8_t, uint8_t>> v;
    istringstream isList(list);
    string coordPair;
    while(getline(isList, coordPair, ',')) {
        stringstream trimmer;
        // Trim coordinate pair
        trimmer.str(coordPair);
        trimmer >> coordPair;
        
        uint8_t row = (uint8_t) coordPair.c_str()[0] - '0';
        uint8_t col = (uint8_t) coordPair.c_str()[1] - '0';
        if (   row >= 0
            && row <= BOARD_ROWS
            && col >= 0
            && col <= BOARD_COLS
            )
        {
            v.push_back(pair<int, int>(row, col));
        }
    }
            
    return v;
}

bool loadConfig(const string& configFile,
                string& letters,
                int& minLen,
                int& maxLen,
                vector<pair<uint8_t, uint8_t>>& doubleLetter,
                vector<pair<uint8_t, uint8_t>>& tripleLetter,        
                vector<pair<uint8_t, uint8_t>>& doubleWord,
                vector<pair<uint8_t, uint8_t>>& tripleWord)
{
    doubleLetter.clear();
    tripleLetter.clear();
    doubleWord.clear();
    tripleWord.clear();

    ifstream myFile(configFile);
    if (myFile.is_open()) {
        string line;
        while(getline(myFile, line)) {
            istringstream is_line(line);
            string key;
            if( getline(is_line, key, '=') ) {
                string value;
                stringstream trimmer;
                
                // Trimm the key
                trimmer.str(key);
                trimmer >> key;
                if (key[0] ==  '#') {
                    continue; // Skip comments
                }
                if( std::getline(is_line, value) ) {
                    if (key == "letters") {
                        // Trim the value
                        trimmer.str(value);
                        trimmer >> value;
                        letters = value;
                    } else if (key == "minLen") {
                        // Trim the value
                        trimmer.str(value);
                        trimmer >> value;
                        minLen = atoi(value.c_str());
                    } else if (key == "maxLen") {
                        // Trim the value
                        trimmer.str(value);
                        trimmer >> value;
                        maxLen = atoi(value.c_str());
                    } else if (strcasecmp(key.c_str(), "dl") == 0) {
                        // No trimming here, done inside parseCoordinateList
                        // Double letter
                        doubleLetter = parseCoordinateList(value);
                    } else if (strcasecmp(key.c_str(), "tl") == 0) {
                        // Triple letter
                        tripleLetter = parseCoordinateList(value);
                    } else if (strcasecmp(key.c_str(), "dw") == 0) {
                        // Double word
                        doubleWord = parseCoordinateList(value);
                    } else if (strcasecmp(key.c_str(), "tw") == 0) {
                        // Triple word
                        tripleWord = parseCoordinateList(value);
                    }
                    
                }
            }
        }
    } else {
        cerr << "Error opening " << configFile << endl;
        return false;
    }
    
    return true;
}

int main(int argc, char** argv) {

    /* CONFIG */
    string letters = "________________";
    int minPathLength = 1;
    int maxPathLength = 5;
    vector<pair<uint8_t, uint8_t>> doubleLetter;
    vector<pair<uint8_t, uint8_t>> tripleLetter;
    vector<pair<uint8_t, uint8_t>> doubleWord;
    vector<pair<uint8_t, uint8_t>> tripleWord;
    
    const string wordListFile = "es_words_no_accents_lowercase_sorted_uniqued.txt";
    const string configFile = "config";
    
    clock_t program_start = clock();

    cout << "Start Memory: " << endl;
    cout << "Start Real memory: "  << endl;

    // Load config
    if (!loadConfig(configFile, letters, minPathLength, maxPathLength,
                    doubleLetter, tripleLetter, doubleWord, tripleWord)) {
        cout << "Failure loading config, using defaults" << endl;
    }
    cout << "Min path: " << minPathLength << endl;
    cout << "Max path: " << maxPathLength << endl;
    
    // Build board
    if (BOARD_ROWS * BOARD_COLS != letters.size()) {
        cerr << "I need " << BOARD_ROWS * BOARD_COLS << " letters, " << letters.size() << " provided" << endl;
        exit(EXIT_FAILURE);
    }
    Board *b =  new Board(BOARD_ROWS, BOARD_COLS);
    b->setLetters(letters);
    b->setMods(DOUBLE_LETTER, doubleLetter);
    b->setMods(TRIPLE_LETTER, tripleLetter);
    b->setMods(DOUBLE_WORD, doubleWord);
    b->setMods(TRIPLE_WORD, tripleWord);
    
    cout << "Using board: " << endl;
    cout << endl << b->toString() << endl;
    
    // And the board manager
    BoardManager bm;

    // Load dictionary
    cout << "Loading word list" << endl;
    unordered_map<string, int> wordList = loadDictionary(wordListFile);

    vector<vector<Node*> > allPaths;
    cout << "Creating possible paths:" << endl;
    for (int len = minPathLength; len <= maxPathLength; len++ ) {
        cout << "\tProcessing path length " << len << endl;
        for (int r = 0; r < BOARD_ROWS; r++) {
            for (int c = 0; c < BOARD_COLS; c++ ) {
                Board cloneBoard = *b;
                vector<vector<Node*> > p = bm.getPathsFrom(r, c, len, cloneBoard);
                allPaths.insert(allPaths.begin(), p.begin(), p.end());
            }
        }
    }

    // Get processed word, which include the word string and its value
    vector<ProcessedWord> allWords;
    for (int i = 0; i < allPaths.size(); i++){
        ProcessedWord pw(allPaths[i]);
        allWords.push_back(pw);
    }
    // Sort them
    
  
    cout << "Checking words" << endl;
    // Print results
    set<string> alreadyPrintedWords;
    vector<ProcessedWord> validWords;
    for (int i = 0; i < allPaths.size(); i++){
        ProcessedWord processedWord (allPaths[i]);
        string word = processedWord.comparableWord();
        if (    wordList.find(word) != wordList.end() /* word in dictionary*/
            &&  alreadyPrintedWords.find(word) == alreadyPrintedWords.end() /* word NOT in printed words */  )
        {
            alreadyPrintedWords.insert(word);
            validWords.push_back(processedWord);
        }
    }

    cout << "Sorting words" << endl;
    sort (validWords.begin(), validWords.end(), ProcessedWord::wordCmp);        

    assert( alreadyPrintedWords.size() == validWords.size());
    cout << "Word list:" << endl;
    for (int i = 0; i < validWords.size(); i++) {
        cout << "(" << (1 + (int)validWords[i].coordOfNodeInPos(0).first);
        cout << ", " << (1 + (int)validWords[i].coordOfNodeInPos(0).second) << ") ";
        cout << validWords[i].prettyWord() << " : " << validWords[i].value() << endl;
    }

    cout << "Done!" << endl;
    cout << "Total results: " << allPaths.size() << endl;
    cout << "Total valid words: " << alreadyPrintedWords.size() << endl;
    
    // Free stuff. 
    /**
     * @todo correctly free.
     */
//    for (int i = 0; i < paths.size(); i++) {
//        for (int j = 0; j < paths[i].size(); j++ ){
//            delete paths[i][j];
//        }
//    }
    delete b;
    
    clock_t program_end = clock() - program_start;
    cout << "Time spent: " << ((float)program_end/CLOCKS_PER_SEC) << " secs" << endl;
    return 0;
}

