#include "Ob.h"
#include "objectCodeMap.h"
#include <iostream>
Ob::Ob(int f, int op)
{
   format = f;
   opCode = op;
}

int Ob::getFormat()
{
    return format;
}

int Ob::getOpCode()
{
    return opCode;
}
