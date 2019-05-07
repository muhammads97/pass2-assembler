#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED
#include "Instruction.h"
#include "Record.h"


string HexFromDecimal(int num);
string getName(vector<Instruction>ins);
int getStartLoc(vector<Instruction>ins);
void printObjectCode(vector<Instruction>ins);

#endif // PRINT_H_INCLUDED
