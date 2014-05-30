/* 
 * File:   Node.cpp
 * Author: maxlo
 * 
 * Created on May 27, 2014, 6:51 PM
 */
#include <iostream>
#include <stdint.h>
#include <sstream>
#include "Node.h"
#include "debug.h"

Node::Node(char letter, uint8_t row, uint8_t col)
    : _letter(letter), _row(row), _col(col), _visited(false)
{
    DEBUG_MSG("build node " << int(row) << ", "<< int(_col));
}

Node::~Node()
{
    //std::cout << "delete node" << std::endl;
}

void Node::set(char letter, uint8_t row, uint8_t col)\
{
    _letter = letter;
    _row = row;
    _col = col;
}
void Node::markVisited()
{
    this->_visited = true;
}

bool Node::isVisited() const{
    return this->_visited;
}

char Node::letter() const
{
    return this->_letter;
}
uint8_t Node::row() const
{
    return this->_row;
}

uint8_t Node::col() const
{
    return this->_col;
}
            
std::string Node::toString()
{
    std::stringstream s("");
    s << "Visited: " << (_visited?"true":"false");
    s << ", letter: " << _letter;
    s << ", row: " << int(_row);
    s << ", col: " << int(_col);
    return s.str();
}