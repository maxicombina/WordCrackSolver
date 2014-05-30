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


#include "Node.h"
#include "Board.h"
#include "BoardManager.h"
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
    }
    
    return dictionary;

}

void loadConfig(const string& configFile, string& letters, int &minLen, int&maxLen)
{
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
                    // Trim the value
                    trimmer.str(value);
                    trimmer >> value;

                    if (key == "letters") {
                        letters = value;
                    } else if (key == "minLen") {
                        minLen = atoi(value.c_str());
                    } else if (key == "maxLen") {
                        maxLen = atoi(value.c_str());
                    } 
                }
            }
        }
    } else {
        cerr << "error opening " << configFile << endl;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    /* CONFIG */
    string letters = "________________";
    int minPathLength = 3;
    int maxPathLength = 8;
    int boardRows = 4;
    int boardCols = 4;
    const string wordListFile = "es_words_no_accents_lowercase_sorted_uniqued.txt";
    const string configFile = "config";
    

    cout << "Start Memory: " << endl;
    cout << "Start Real memory: " << endl;

    // Load config
    loadConfig(configFile, letters, minPathLength, maxPathLength);
    cout << "Min path: " << minPathLength << endl;
    cout << "Max path: " << maxPathLength << endl;
    
    // Build board
    if (boardRows * boardCols != letters.size()) {
        cerr << "I need " << boardRows * boardCols << " letters, " << letters.size() << " provided" << endl;
        exit(EXIT_FAILURE);
    }
    Board *b =  new Board(boardRows, boardCols);
    b->setLetters(letters);
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
        for (int r = 0; r < boardRows; r++) {
            for (int c = 0; c < boardCols; c++ ) {
                Board cloneBoard = *b;
                vector<vector<Node*> > p = bm.getPathsFrom(r, c, len, cloneBoard);
                allPaths.insert(allPaths.begin(), p.begin(), p.end());
            }
        }
    }
    
    //cout << pathsToString(allPaths);
    cout << "Checking words" << endl;
    // Print results
    set<string> alreadyPrintedWords;
    for (int i = 0; i < allPaths.size(); i++){
        string word = bm.wordFromPath(allPaths[i]);
        if (    wordList.find(word) != wordList.end() /* word in dictionary*/
            &&  alreadyPrintedWords.find(word) == alreadyPrintedWords.end() /* word NOT in printed words */  )
        {
            alreadyPrintedWords.insert(word);
            cout << "(" << (1+allPaths[i][0]->row());
            cout << "," << (1+allPaths[i][0]->col()) << ") " ;
            cout << word << endl;
        }
    }

    cout << "Done!" << endl;
    cout << "Total results: " << allPaths.size() << endl;
    cout << "Total valid words: " << alreadyPrintedWords.size() << endl;
    
    // Free stuff.
//    for (int i = 0; i < paths.size(); i++) {
//        for (int j = 0; j < paths[i].size(); j++ ){
//            delete paths[i][j];
//        }
//    }
    delete b;
    
    
    return 0;
}

