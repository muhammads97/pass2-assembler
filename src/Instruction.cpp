#include "Instruction.h"


Instruction::Instruction(std::string l, std::string op, std::string oper)
{
    transform(l.begin(), l.end(), l.begin(), ::tolower);
    transform(op.begin(), op.end(), op.begin(), ::tolower);
    regex r("=([Xx]|[cC])\'([a-zA-Z0-9]+)\'");
    regex r2("([Cc]|[Xx])\'(.+)\'");
    if(!regex_match(oper, r) && !regex_match(oper, r2)){
        transform(oper.begin(), oper.end(), oper.begin(), ::tolower);
    }
    label = l;
    operation = op;
    operand = oper;
}
int Instruction:: getAddress() {
    return address;
}
void Instruction:: setAddress(int add) {
    address = add;
}
bool Instruction::isWrong() {
    return errorFlag;
}
void Instruction::setErrorFlag(bool f) {
    errorFlag = f;
}
std::string Instruction::getErrorMsg() {
    return errorMsg;
}
void Instruction::setErrorMsg(std::string error) {
    errorMsg = error;
}
std::string Instruction::getLabel() {
    return label;
}
std::string Instruction::getOperation() {
    return operation;
}
std::string Instruction::getOperand() {
    return operand;
}
int Instruction::getFormat() {
    return format;
}
void Instruction::setFormat(int f) {
    format = f;
}
std::string Instruction::getComment() {
    return comment;
}
bool Instruction::isComment() {
    return commetFlag;
}
void Instruction::setCommentflag(bool f) {
    commetFlag = f;
}
void Instruction::setCommentMsg(std::string str) {
    comment = str;
}
void Instruction::setRelocatable(bool f) {
    relocatable = true;
}
bool Instruction::getRelocatable() {
    return relocatable;
}
void Instruction::setOpCode(int opCode) {
    this->opCode = opCode;
    hasOpCode = true;
}
bool Instruction::ishasOpCode() {
    return hasOpCode;
}
int Instruction::getOpCode() {
    return opCode;
}

bool Instruction::isPass2Err() {
    return pass2Error;
}
void Instruction::setPass2ErrMsg(std::string msg) {
    this->pass2Error = true;
    this->pass2ErrMsg = msg;
}

void Instruction::setLongObjectCode(string obCode){
    this->longObjectCode = obCode;
    this->hasOpCode = true;
}
string Instruction::getLongObjectCode(){
    return longObjectCode;
}
string Instruction::toString(){
    if(pass2Error){
        return pass2ErrMsg;
    }
    if(hasOpCode){
        stringstream ss;
        ss << hex << address;
        if(isByte){
            return label + "    " + operation + "    " + operand + "    " + comment + "    " + ss.str() + "      " + longObjectCode;
        }
        stringstream ss2;
        ss2 << hex << opCode;
        return label + "    " + operation + "    " + operand + "    " + comment + "    " + ss.str() + "      " + ss2.str();
    }
    stringstream ss;
    ss << hex << address;
    return label + "    " + operation + "    " + operand + "    " + comment + "    " + ss.str();

}
void Instruction::setStringOpCode(string string_opCode) {
    this->string_opCode = string_opCode;
}
string Instruction::getStringOpCode() {
    return string_opCode;
}
Instruction::~Instruction() {
    //dtor
}
