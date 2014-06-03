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
#include <thread>

#include "Node.h"
#include "Board.h"
#include "BoardManager.h"
#include "ProcessedWord.h"
#include "debug.h"
using namespace std;


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
        exit(EXIT_FAILURE);
    }
    
    return dictionary;

}

vector<pair<uint8_t, uint8_t>> parseCoordinateList(const string& list, uint8_t rowNum, uint8_t colNum)
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
            && row <= rowNum
            && col >= 0
            && col <= colNum
            )
        {
            v.push_back(pair<int, int>(row, col));
        }
    }
            
    return v;
}

bool loadConfig(const string& configFile, uint8_t rowNum, uint8_t colNum,
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
                        doubleLetter = parseCoordinateList(value, rowNum, colNum);
                    } else if (strcasecmp(key.c_str(), "tl") == 0) {
                        // Triple letter
                        tripleLetter = parseCoordinateList(value, rowNum, colNum);
                    } else if (strcasecmp(key.c_str(), "dw") == 0) {
                        // Double word
                        doubleWord = parseCoordinateList(value, rowNum, colNum);
                    } else if (strcasecmp(key.c_str(), "tw") == 0) {
                        // Triple word
                        tripleWord = parseCoordinateList(value, rowNum, colNum);
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

void doNothting()
{
    cout << "Thread here";
}
void computeValidWords( /* In */
                        int len, const Board& board, BoardManager& bm, const unordered_map<string, int> &wordList,
                        /* Out */
                        int *totalPaths,
                        vector<ProcessedWord> *validWords)
{
    set<string> alreadyDetectedWords;
    // Only for stats
    int totalPathsTmp = 0;

    for (int r = 0; r < board.getRows(); r++) {
        for (int c = 0; c < board.getCols(); c++ ) {
            Board cloneBoard = board;
            // All possible paths from pos (`r`, `c`) of length `len`
            vector<vector<Node*> > paths = bm.getPathsFrom(r, c, len, cloneBoard);

            totalPathsTmp += paths.size();

            // Verify all the paths and keep only the valid words
            for (int i = 0; i < paths.size(); i++) {
                // Get current word
                ProcessedWord processedWord(paths[i]);
                string word = processedWord.comparableWord();

                if (   wordList.find(word) != wordList.end() /* Valid ... */
                    && alreadyDetectedWords.find (word) == alreadyDetectedWords.end()) /* ... but not already stored */
                {
                    alreadyDetectedWords.insert(word);
                    validWords->push_back(processedWord);
                } else {
                    // This path is not a valid word. Free it from memory
                    for (int j = 0; j < paths[i].size(); j++ ){
                        delete paths[i][j];
                    }
                }
            }
        }
    }

    
    *totalPaths = totalPathsTmp;
}

int main(int argc, char** argv) {

    /* Default CONFIG */
    string letters = "________________";
    int minPathLength = 1;
    int maxPathLength = 5;
    vector<pair<uint8_t, uint8_t>> doubleLetter;
    vector<pair<uint8_t, uint8_t>> tripleLetter;
    vector<pair<uint8_t, uint8_t>> doubleWord;
    vector<pair<uint8_t, uint8_t>> tripleWord;
   
    const string wordListFile = "dict.txt";
    const string configFile = "config";
    
    clock_t program_start = clock();

    cout << "Start Memory: " << endl;
    cout << "Start Real memory: "  << endl;

    // Load config
    if (!loadConfig(configFile, DEFAULT_BOARD_ROWS, DEFAULT_BOARD_ROWS, letters, minPathLength, maxPathLength,
                    doubleLetter, tripleLetter, doubleWord, tripleWord)) {
        cout << "Failure loading config, using defaults" << endl;
    }
    cout << "Min path: " << minPathLength << endl;
    cout << "Max path: " << maxPathLength << endl;

    // Every length will be computed by a thread
    const int numThreads = maxPathLength - minPathLength + 1;

    
    // Build board
    if (DEFAULT_BOARD_ROWS * DEFAULT_BOARD_COLS != letters.size()) {
        cerr << "I need " << DEFAULT_BOARD_ROWS * DEFAULT_BOARD_ROWS << " letters, " << letters.size() << " provided" << endl;
        exit(EXIT_FAILURE);
    }
    Board board(DEFAULT_BOARD_ROWS, DEFAULT_BOARD_COLS);
    board.setLetters(letters);
    board.setMods(DOUBLE_LETTER, doubleLetter);
    board.setMods(TRIPLE_LETTER, tripleLetter);
    board.setMods(DOUBLE_WORD, doubleWord);
    board.setMods(TRIPLE_WORD, tripleWord);
  
    // And the board manager
    BoardManager bm;

    cout << "Using board: " << endl;
    cout << endl << board.toString() << endl;

    // Load dictionary
    cout << "Loading word list" << endl;
    unordered_map<string, int> wordList = loadDictionary(wordListFile);

    cout << "Creating possible paths and checking words with " << numThreads << " threads" << endl;
    
    vector<thread> workers;
    vector<vector<ProcessedWord> > validWords;
    vector<int> totalPathsCount;
    
    workers.resize(numThreads);
    
    validWords.resize(numThreads);
    totalPathsCount.resize(numThreads);

    int currentLen = minPathLength;
    for (int i = 0 ; i < numThreads; i++) {
        currentLen = minPathLength + i;
        // Put thread to work
        Board cloneBoard = board;
        cout  << "\tProcessing path length " << currentLen << endl;
        workers[i] = thread (computeValidWords, currentLen, board, bm, wordList, &totalPathsCount[i], &validWords[i]);
        //workers[i]=  thread (doNothting);
        cout << "Thread creation done " << currentLen << endl;
    }

    cout << "Waiting for workers" << flush;
    for (int i = 0 ; i < numThreads; i++) {
        // Wait for every worker
        currentLen = minPathLength + i;
        workers[i].join();
        cout << " " << currentLen << flush;
    }
    cout << " done!" << endl;
    
    int totalPaths = 0;
    vector<ProcessedWord> allValidWords;
    for (int i = 0; i < workers.size(); i++) {
        // Sum up total Paths
        totalPaths += totalPathsCount[i];
        cout << "valid words " << i << ": " << validWords[i].size() << endl;
        // Convert validWords to a single vector
        allValidWords.insert(allValidWords.end(), validWords[i].begin(), validWords[i].end());
    }
    
    cout << "Sorting words" << endl;
    sort (allValidWords.begin(), allValidWords.end(), ProcessedWord::wordCmp);  

    cout << "Total paths: " << totalPaths << endl;
    cout << "\nWord list:" << endl;
    for (int i = 0; i < allValidWords.size(); i++) {
        cout << "(" << (1 + (int)allValidWords[i].coordOfNodeInPos(0).first); 
        cout << ", " << (1 + (int)allValidWords[i].coordOfNodeInPos(0).second) << ") ";
        cout << allValidWords[i].prettyWord() << " : " << allValidWords[i].value() << endl;
    }

    cout << "\nDone!" << endl;
    cout << "Total results: " << totalPaths << endl;
    cout << "Total valid words: " << allValidWords.size() << endl;
    
    // Free stuff. 
    // Free validWords
    for (int i = 0; i < validWords.size(); i++) {
            allValidWords[i].deleteWord();
 
    }
    // And free the board
    //delete board;
    
    clock_t program_end = clock() - program_start;
    cout << "Time spent: " << ((float)program_end/CLOCKS_PER_SEC) << " secs" << endl;

    return 0;
}

