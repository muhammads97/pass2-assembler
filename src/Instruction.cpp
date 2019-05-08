#include "Instruction.h"

Instruction::Instruction(std::string l, std::string op, std::string oper)
{
    label = l;
    operation = op;
    operand = oper;
}
int Instruction:: getAddress(){
    return address;
}
void Instruction:: setAddress(int add){
    address = add;
}
bool Instruction::isWrong(){
    return errorFlag;
}
void Instruction::setErrorFlag(bool f){
    errorFlag = f;
}
std::string Instruction::getErrorMsg(){
    return errorMsg;
}
void Instruction::setErrorMsg(std::string error){
    errorMsg = error;
}
std::string Instruction::getLabel(){
    return label;
}
std::string Instruction::getOperation(){
    return operation;
}
std::string Instruction::getOperand(){
    return operand;
}
int Instruction::getFormat(){
    return format;
}
void Instruction::setFormat(int f){
    format = f;
}
std::string Instruction::getComment(){
    return comment;
}
bool Instruction::isComment(){
    return commetFlag;
}
void Instruction::setCommentflag(bool f){
    commetFlag = f;
}
void Instruction::setCommentMsg(std::string str){
    comment = str;
}
void Instruction::setRelocatable(bool f){
    relocatable = true;
}
bool Instruction::getRelocatable(){
    return relocatable;
}
void Instruction::setOpCode(int opCode){
    this->opCode = opCode;
    hasOpCode = true;
}
bool Instruction::ishasOpCode(){
    return hasOpCode;
}
int Instruction::getOpCode(){
    return opCode;
}

bool Instruction::isPass2Err(){
    return pass2Error;
}
void Instruction::setPass2ErrMsg(std::string msg){
    this->pass2Error = true;
    this->pass2ErrMsg = msg;
}

string Instruction::toString(){
    return label + "    " + operation + "    " + operand + "    " + comment;
}

Instruction::~Instruction()
{
    //dtor
}
