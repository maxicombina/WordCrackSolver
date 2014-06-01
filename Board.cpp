/* 
 * File:   Board.cpp
 * Author: maxlo
 * 
 * Created on May 27, 2014, 7:56 PM
 */

#include <iostream>
#include <sstream>
#include <stdint.h>

#include "debug.h"
#include "Board.h"
#include "Node.h"

void Board::init(uint8_t rows, uint8_t cols)
{
    _rows = rows;
    _cols = cols;

    // Build board matrix
    _board.resize(rows);
    DEBUG_MSG("vector size: " << _board.size());
    for (int i = 0; i < _board.size(); i++) {
        _board[i].resize(cols);
        DEBUG_MSG("board" << i << " size: " << _board[i].size());
    }

    for (int r = 0; r <  _board.size(); r++) {
        for (int c = 0; c < _board[r].size(); c++) {
            _board[r][c] = new Node('_', r, r, NONE);
        }
    }

}

Board::Board(uint8_t rows, uint8_t cols)
{
    this->init(rows, cols);

}
Board::Board()
{
    this->init(DEFAULT_BOARD_ROWS, DEFAULT_BOARD_COLS);
}

Board::Board(const Board& orig) {
    DEBUG_MSG("Board CC");
    _rows = orig._rows;
    _cols = orig._cols;
    
    _board.resize(_rows);
    for (int i = 0; i < _board.size(); i++) {
        _board[i].resize(_cols);
    }    
    
    for (int r = 0; r <  _board.size(); r++) {
        for (int c = 0; c < _board[r].size(); c++) {
            _board[r][c] = new Node(orig._board[r][c]->letter(), orig._board[r][c]->row(), orig._board[r][c]->col(), orig._board[r][c]->getMod());
            if (orig._board[r][c]->isVisited()) {
                _board[r][c]->markVisited();
            }
        }
    }
}

Board::~Board() {
    for (int r = 0; r <  _board.size(); r++) {
        for (int c = 0; c < _board[r].size(); c++) {
            //std::cout << r << ", " << c << ": " << _board[r][c] << std::endl;
            //if (_board[r][c]) {
                delete _board[r][c];
                _board[r][c] = 0;
            //}
        }
    }
}


uint8_t Board::getRows() const
{
    return _rows;
}

uint8_t Board::getCols() const
{
    return _cols;
}

Node* Board::getNode(uint8_t row, uint8_t col) const
{
    return _board[row][col];
}

void Board::setNode(uint8_t row, uint8_t col, Node& node)
{
    //delete _board[row][col];
    //_board[row][col] = &node;
    _board[row][col]->set(node.letter(), node.row(), node.col());
}


void Board::setLetters(std::string letters) {
    int base = 0;
    for (int r = 0; r < _rows; r++) {
        for (int c = 0; c < _cols; c++) {
            int offset = c;
            char letter = tolower(letters[base + offset]);
            _board[r][c]->set(letter, r, c);
        }
        base += _cols;
    }
}

void Board::setMods(ScoreMods mod, std::vector<std::pair<uint8_t, uint8_t> > coords)
{
    for (int i = 0; i < coords.size(); i++) {
        // The "-1" is because the coordinateds in the config file start from 1
        _board[coords[i].first-1][coords[i].second-1]->setMod(mod);
    }
}

std::string Board::toString()
{
    std::stringstream s("");
    for (int r = 0; r < _board.size(); r++) {
        for (int c = 0; c < _board[r].size(); c++) {
            s << "  [";
            std::string letter = "_";
            if (_board[r][c]->letter() == 'q') {
                letter = "QU";
            } else if (_board[r][c]->letter() == '#') {
                letter = "Ñ";
            } else {
                letter = toupper(_board[r][c]->letter());
            }
            s << letter;
            s << Node::Node::scoreModToString(_board[r][c]->getMod());
            s << "]";
        }
        s << "\n\n";
    }
    
    return s.str();
}
