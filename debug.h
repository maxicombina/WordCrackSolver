/* 
 * File:   debug.h
 * Author: maxlo
 *
 * Created on May 28, 2014, 7:49 PM
 */

#ifndef DEBUG_H
#define	DEBUG_H


#ifdef DEBUG_DONT
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#endif	/* DEBUG_H */

