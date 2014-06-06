/* 
 * File:   BoardManager.cpp
 * Author: maxlo
 * 
 * Created on May 28, 2014, 8:21 PM
 */

#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <fstream>
#include <string>
#include <cstdlib>



#include "BoardManager.h"
#include "Node.h"
#include "ProcessedWord.h"
#include "debug.h"

BoardManager::BoardManager() {


}

BoardManager::~BoardManager() {
}

std::vector<Node*> BoardManager::getAvailableNeighbors(uint8_t row, uint8_t col,  const Board &board)
{
    std::vector<Node*> v; //neighbors list
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            bool validTile = row + r >= 0;
            validTile = validTile && (col + c >= 0);
            validTile = validTile && (row + r < board.getRows());
            validTile = validTile && (col + c < board.getCols());
            validTile = validTile && !(r == 0 && c == 0);
            validTile = validTile && !board.getNode(row + r, col + c)->isVisited();
            if (validTile) {
                v.push_back(board.getNode(row + r, col + c));
            }
        }
    }
    return v;
}

std::vector<std::vector<Node*> > BoardManager::getPathsFrom(int length, const Board& board)
{
    std::vector<std::vector<Node*> > paths;
    
    std::stringstream pathFile;
    pathFile << "paths_";
    if (length < 10) {
        pathFile << "0";
    }
    pathFile << length;
    
    std::ifstream myFile (pathFile.str().c_str());
    if (myFile.is_open()) {
        std::string singlePath;
       
        while(getline(myFile, singlePath)) {
            
            std::istringstream is_singlePath (singlePath);
            std::string singleNode;
            
            std::vector<Node*> singlePathNodes;
            
            while(getline(is_singlePath, singleNode, ',')) {
                
                uint8_t row = (uint8_t) singleNode.c_str()[0] - '0';
                uint8_t col = (uint8_t) singleNode.c_str()[1] - '0';
                
                Node* node = new Node(board.getNode(row, col)->letter(), row, col, board.getNode(row, col)->getMod());
                singlePathNodes.push_back(node);
            }
            
            paths.push_back(singlePathNodes);
        }
        
    } else {
        std::cerr << "Error opening file " << pathFile.str() << std::endl;
        exit(EXIT_FAILURE);
    }

    return paths;
}



 