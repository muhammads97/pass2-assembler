#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include "Ob.h"

std::unordered_map<std::string, Ob*> ObjectCodes;

void init_opCodes();
int getTheCode(std::string ins);
int getTheFormat(std::string ins);

#endif // OPCODES_H_INCLUDED
