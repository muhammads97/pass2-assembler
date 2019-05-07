#include "OpCodeParameters.h"

OpCodeParameters::OpCodeParameters()
{

}

void OpCodeParameters::setOperation(int operation){
    this->operation = operation;
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
