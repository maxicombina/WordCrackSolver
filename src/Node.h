/* 
 * File:   Node.h
 * Author: Maxi Combina
 *
 * Created on May 27, 2014, 6:38 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <iostream>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

   
    enum ScoreMods {
        NONE,
        DOUBLE_LETTER,
        TRIPLE_LETTER,
        DOUBLE_WORD,
        TRIPLE_WORD
    };
            
    class Node {
        private:
            bool _visited;
            char _letter;
            uint8_t _row;
            uint8_t _col;
            ScoreMods _mod;
            
        public:
            // Constructors, destructor
            Node(char letter, uint8_t row, uint8_t col, ScoreMods mod);
            ~Node();
            
            // Methods
            void set(char letter, uint8_t row, uint8_t col);
            void setMod(ScoreMods mod);
            ScoreMods getMod(void);
            void markVisited();
            bool isVisited() const;
            void clearVisited();
            char letter() const;
            uint8_t row() const;
            uint8_t col() const;
            std::string toString();
            static std::string scoreModToString(ScoreMods mod);
        
    };


#ifdef	__cplusplus
}
#endif

#endif	/* NODE_H */

