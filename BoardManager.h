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
    std::vector<std::vector<Node*> > getPathsFrom(int length, const Board& boad);
};

#endif	/* BOARDMANAGER_H */

