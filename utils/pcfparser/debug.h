#pragma once


#include <iostream>
//#define DEBUG
#ifdef DEBUG
#define LOG(arg) std::cerr << (arg) <<std::endl;
#else
#define LOG(arg)
#endif

#endif
