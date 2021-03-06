#include "OpCodeParameters.h"

OpCodeParameters::OpCodeParameters()
{

}

void OpCodeParameters::setOperation(int operation){
    this->operation = operation;
    if(operation == 0x90 || operation == 0xA0 || operation == 0x9C || operation == 0x98 || operation == 0xAC || operation == 0x94){
        twoReg = true;
    } else if (operation == 0xB4 || operation == 0xB8){
        oneReg = true;
    } else if (operation == 0xA4 || operation == 0xA8){
        shift = true;
    }
}
void OpCodeParameters::setN(bool n){
    this->n = n;
}
void OpCodeParameters::setI(bool i){
    this->i = i;
}
void OpCodeParameters::setX(bool x){
    this->x = x;
}
void OpCodeParameters::setB(bool b){
    this->b = b;
}
void OpCodeParameters::setE(bool e){
    this->e = e;
}
void OpCodeParameters::setP(bool p){
    this->p = p;
}
void OpCodeParameters::setAddress(int address){
    this->address = address;
}
int OpCodeParameters::getObjectCode(){
    if(twoReg || oneReg || shift){
        int obCode = operation << 8;
        obCode = obCode | address;
        return obCode;
    }
    //calculating first byte ( op code + n + i)
    int f2byte;
    if(n){
        f2byte = operation | 2;
    } else {
        f2byte = operation;
    }
    if(i){
        f2byte = f2byte | 1;
    }
    // calculating next half byte
    int beforeAddress = f2byte << 4;
    if(x){
        beforeAddress = beforeAddress | 8;
    }
    if(b){
        beforeAddress = beforeAddress | 4;
    }
    if(p){
        beforeAddress = beforeAddress | 2;
    }
    if(e){
        beforeAddress = beforeAddress | 1;
        opCode = beforeAddress << 20;
        opCode = opCode | address;
    } else {
        opCode = beforeAddress << 12;
        opCode = opCode | address;
    }
    return opCode;
}
