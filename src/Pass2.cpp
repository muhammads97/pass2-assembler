#include "Pass2.h"
#include <regex>

Pass2::Pass2()
{
    //ctor
}

Pass2::~Pass2()
{
    //dtor
}
void Pass2::setOpTable(vector<Instrucion> opTab){
    this->opTab = opTab;
}
void Pass2::setSymTable(unordered_map<string, Symbol> symTab){
    this->symTab = symTab;
}
unordered_map<string, Symbol> Pass2::getSymTable(){
    return symTab;
}
vector<Instrucion> Pass2::getOpTable(){
    return opTab;
}
vector<Instrucion> Pass2::execute(){
    bool started = false;
    int j = 0;
    while(j < opTab.size()){
        bool err = false;
        if(!started){
            if(opTab[j].getOperation() != string("start")){
                continue;
            }
        }
        if(opTab[j].getOperation() == string("end")){
                break;
        }
        if(opTab[j].getOperation() == string("base")){
                setBaseAddress(opTab[j].getOperand());
                continue;
        }
        int locctr = opTab[j + 1].getAddress(); // need to be modified to find the address correctly since it may not be the next line
        string op = opTab[j].getOperation();
        string operand = opTab[j].getOperand();
        int opHex = opCodes.getTheCode(op);
        int operandHex = 16777215;
        bool e = false;
        bool i = false;
        bool n = false;
        bool x = false;
        bool b = false;
        bool p = false;
        if(op[0] == '+'){
            e = true;
        }
        if(operand[0] == '#'){
            i = true;
            //immediate addressing
            operand = operand.substr(1, operand.size());

        } else if (operand[0] == '@'){
            //Indirect addressing
            n = true;
            operand = operand.substr(1, operand.size());
        } else if (operand[0] == '='){
            //deal with literals
        } else if (operand == string("*")){

        } else if(isIndexedAddressing(operand)){
            x = true;
            i = true;
            n = true;
            operand = getIndexedSymbol(operand);
        } else {
            i = true;
            n = true;
        }
        unordered_map<string, Symbol>::const_iterator iter = symTab.find(operand);
        if(iter == symTab.end()){
            err = true;
        } else {
            operandHex = iter->second.address;
        }
        OpCodeParameters param;
        int disp = displacement(locctr, operandHex, base, e, b, p);
        param.setAddress(disp);
        param.setB(b);
        param.setE(e);
        param.setI(i);
        param.setN(n);
        param.setOperation(opHex);
        param.setP(p);
        param.setX(x);
        opTab[j].setOpCode(param.getObjectCode());
        //to be continued
    }
}

bool isIndexedAddressing(string operand){
    regex r("[a-zA-Z][a-zA-Z0-9]+ *, *[xX]");
    return regex_match(operand, r);
}
string getIndexedSymbol(string indexed){
    regex r("([a-zA-Z][a-zA-Z0-9]+) *, *[xX]");
    smatch m;
    if(regex_search(indexed, m, r)){
        return m.str(1);
    }
}
void Pass2::setBaseAddress(string operand){
    try{
        return stoi(operand);
    } catch (std::invalid_argument arg){
        unordered_map<string, Symbol>::const_iterator iter = symTab.find(operand);
        if(iter == symTab.end()){
            return 16777215;
        } else {
            return iter->second.address;
        }
    }
}
