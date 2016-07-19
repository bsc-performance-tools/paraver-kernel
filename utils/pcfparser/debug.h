#ifndef _DEBUG_H
#define _DEBUG_H

#include <iostream>
//#define DEBUG
#ifdef DEBUG
#define LOG(arg) std::cerr << (arg) <<std::endl;
#else
#define LOG(arg)
#endif

#endif
