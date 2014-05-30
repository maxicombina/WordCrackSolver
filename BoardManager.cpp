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

std::vector<std::vector<Node*> > BoardManager::getPathsFrom(uint8_t row, uint8_t col, int length, const Board &board)
{
    std::vector<std::vector<Node*> > paths;
    Node * currentNode = board.getNode(row, col);
    currentNode->markVisited();
    
    if (length == 1) {
        std::vector<Node*> ngbs = this->getAvailableNeighbors(row, col, board);
        for (int i = 0; i < ngbs.size(); i++) {
            std::vector<Node*> path;
            Node* currentNodeCopy = new Node(currentNode->letter(), currentNode->row(), currentNode->col());
            path.push_back(currentNodeCopy);
            Node* currentNgbCopy = new Node(ngbs[i]->letter(), ngbs[i]->row(), ngbs[i]->col());
            path.push_back(currentNgbCopy);
            paths.push_back(path);
        }
        
    } else {
        std::vector<Node*> ngbs = this->getAvailableNeighbors(row, col, board);
        for (int i = 0; i < ngbs.size(); i++) {
            Board cloneBoard = board;
            std::vector<std::vector<Node*> > subPaths;
            subPaths = this->getPathsFrom(ngbs[i]->row(), ngbs[i]->col(), length-1, cloneBoard);
            for (int j = 0; j < subPaths.size(); j++) {
                std::vector<Node *> currentPath = subPaths[j];
                std::vector<Node*>::iterator it = currentPath.begin();
                Node* currentNodeCopy = new Node(currentNode->letter(), currentNode->row(), currentNode->col());
                currentPath.insert(it, currentNodeCopy);
                paths.push_back(currentPath);
            }
        }
    }
    return paths;
}

std::pair<std::string, int> BoardManager::wordFromPath(std::vector<Node* > path)
{
//    std::stringstream s("");
//    int value = 0;
//    for (int i = 0; i < path.size(); i++){
//        value += _letterValues[path[i]->letter()];
//        if (path[i]->letter() == 'q'){
//            s << "qu";
//        } else {
//            s << path[i]->letter();
//        }
//    }
//    return std::pair<std::string, int>(s.str(), value);
}


 