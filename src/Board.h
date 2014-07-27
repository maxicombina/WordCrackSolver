/*
 * File:   Board.h
 * Author: Maxi Combina
 *
 * Created on May 27, 2014, 7:56 PM
 */

#ifndef BOARD_H
#define	BOARD_H

#include <vector>
#include <memory>

#include <stdint.h>

#include "Node.h"

// Default Board Sizes
#define DEFAULT_BOARD_ROWS 4
#define DEFAULT_BOARD_COLS 4

/**
 * Represents a WordCrack Board.
 */
class Board {

public:
    /**
     * Builds a Board of the specified size.
     * The Board holds a matrix of Nodes
     *
     * @param rows Number of rows
     * @param cols Number of columns
     */
    Board(uint8_t rows, uint8_t cols);

    /**
     * Builds a Board using default dimensions: DEFAULT_BOARD_ROWS x DEFAULT_BOARD_COLS
     */
    Board();

    /**
     * Copy constructor
     * @param orig
     */
    Board(const Board& orig);


    virtual ~Board();

    /**
     * Gets the number of rows of this board
     * @return
     */
    uint8_t getRows() const;
    /**
     * Gets the number of columns of this board
     * @return
     */
    uint8_t getCols() const;

    /**
     * Gets the Node at a given position
     * @param row. Valid range is [0 .. getRows()-1]
     * @param col. Valid range is [0 .. getCols()-1]
     * @return
     */
    Node* getNode(uint8_t row, uint8_t col) const;

    /**
     * Sets a specific node
     * @param row
     * @param col
     * @param node Node from where to copy the values (letter, modifier)
     */
    void setNode(uint8_t row, uint8_t col, Node& node);

    /**
     * Sets the letters of this Board
     * @param letters The letters to use. The length of string must
     *                be equal to getRows()*getCols()
     */
    void setLetters(std::string letters);

    /**
     * Sets the modifiers of this Board
     *
     * @param mod The modifier
     * @param coords Vector with coordinates of this modifier
     */
    void setMods(ScoreMods mod, std::vector<std::pair<uint8_t, uint8_t> > coords);

    /**
     * Obtains a nice string representation
     * 
     * @return
     */
    std::string toString();

private:
    uint8_t _rows;
    uint8_t _cols;
    std::vector< std::vector<Node*> > _board;
    void init(uint8_t rows, uint8_t cols);
};

#endif	/* BOARD_H */
