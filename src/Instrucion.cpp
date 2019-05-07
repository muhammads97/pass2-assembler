#include "Instrucion.h"

Instrucion::Instrucion(std::string l, std::string op, std::string oper)
{
    label = l;
    operation = op;
    operand = oper;
}
int Instrucion:: getAddress(){
    return address;
}
void Instrucion:: setAddress(int add){
    address = add;
}
bool Instrucion::isWrong(){
    return errorFlag;
}
void Instrucion::setErrorFlag(bool f){
    errorFlag = f;
}
std::string Instrucion::getErrorMsg(){
    return errorMsg;
}
void Instrucion::setErrorMsg(std::string error){
    errorMsg = error;
}
std::string Instrucion::getLabel(){
    return label;
}
std::string Instrucion::getOperation(){
    return operation;
}
std::string Instrucion::getOperand(){
    return operand;
}
int Instrucion::getFormat(){
    return format;
}
void Instrucion::setFormat(int f){
    format = f;
}
std::string Instrucion::getComment(){
    return comment;
}
bool Instrucion::isComment(){
    return commetFlag;
}
void Instrucion::setCommentflag(bool f){
    commetFlag = f;
}
void Instrucion::setCommentMsg(std::string str){
    comment = str;
}
void Instrucion::setRelocatable(bool f){
    relocatable = true;
}
bool Instrucion::getRelocatable(){
    return relocatable;
}
void Instrucion::setOpCode(int opCode){
    this->opCode = opCode;
    hasOpCode = true;
}
bool Instrucion::ishasOpCode(){
    return hasOpCode;
}
int Instrucion::getOpCode(){
    return opCode;
}
Instrucion::~Instrucion()
{
    //dtor
}
