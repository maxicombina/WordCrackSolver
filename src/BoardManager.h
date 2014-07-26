/* 
 * File:   BoardManager.h
 * Author: maxlo
 *
 * Created on May 28, 2014, 8:21 PM
 */

#ifndef BOARDMANAGER_H
#define	BOARDMANAGER_H

#include <vector>
#include <stdint.h>
#include <map>
#include <utility>

#include "ProcessedWord.h"
#include "Node.h"
#include "Board.h"

class BoardManager {
public:
    BoardManager();
    ~BoardManager();
public:
    std::vector<Node* > getAvailableNeighbors(uint8_t row, uint8_t col, const Board &board);
    std::vector<std::vector<Node*> > getPathsFrom(uint8_t row, uint8_t col, int length, const Board &board);
    std::pair<std::string, int> wordFromPath(std::vector<Node* > path);
};

#endif	/* BOARDMANAGER_H */

