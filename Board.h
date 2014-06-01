/* 
 * File:   Board.h
 * Author: maxlo
 *
 * Created on May 27, 2014, 7:56 PM
 */

#ifndef BOARD_H
#define	BOARD_H

#include <vector>
#include <memory>

#include <stdint.h>

#include "Node.h"

// Board Size
#define DEFAULT_BOARD_ROWS 4
#define DEFAULT_BOARD_COLS 4

class Board {
public:
    Board(uint8_t rows, uint8_t cols);
    Board();
    Board(const Board& orig);
    virtual ~Board();
    
    uint8_t getRows() const;
    uint8_t getCols() const;
    Node* getNode(uint8_t row, uint8_t col) const;
    void setNode(uint8_t row, uint8_t col, Node& node);
    void setLetters(std::string letters);
    void setMods(ScoreMods, std::vector<std::pair<uint8_t, uint8_t> > );
    std::string toString();
    
private:
    uint8_t _rows;
    uint8_t _cols;
    std::vector< std::vector<Node*> > _board;
    void init(uint8_t rows, uint8_t cols);
};

#endif	/* BOARD_H */
