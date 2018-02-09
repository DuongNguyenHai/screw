// Nguyen Hai Duong
// Nov 2016

#ifndef BONE_CONFIG_H
#define BONE_CONFIG_H

#include <iostream>
#include <inttypes.h>	// int32_t int64_t

namespace BONE {

#define RET_MISS -1
#define RET_SUCCESS 0
#define RET_FAILURE 1

extern std::string MASTER_IP;

#define StringToChar(str) str.c_str() 
// Define port for ...
#define BRANCH_PORT 8888
#define CONSOLE_PORT 8885


extern const char *usage;

}	// end of namespace TREE


#endif