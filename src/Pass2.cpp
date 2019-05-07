#include "Pass2.h"
#include <regex>

Pass2::Pass2() {
    //ctor
}

Pass2::~Pass2() {
    //dtor
}
void Pass2::setOpTable(vector<Instruction> opTab) {
    this->opTab = opTab;
}
void Pass2::setSymTable(unordered_map<string, Symbol> symTab) {
    this->symTab = symTab;
}
unordered_map<string, Symbol> Pass2::getSymTable() {
    return symTab;
}
vector<Instruction> Pass2::getOpTable() {
    return opTab;
}
vector<Instruction> Pass2::execute() {
    bool started = false;
    int j = 0;
    while(j < opTab.size()) {
        bool err = false;
        if(!started) {
            if(opTab[j].getOperation() != string("start")) {
                continue;
            } else {
                started = true;
                continue;
            }
        }
        if(opTab[j].getOperation() == string("")) {
            continue;
        }
        if(opTab[j].getOperation() == string("end")) {
            break;
        }
        if(opTab[j].getOperation() == string("base")) {
            setBaseAddress(opTab[j].getOperand());
            continue;
        }
        int locctr = opTab[j].getAddress();
        locctr += opTab[j].getFormat();
        string op = opTab[j].getOperation();
        string operand = opTab[j].getOperand();
        int opHex = opCodes.getTheCode(op);
        int operandHex = 16777215;
        int disp = 0;
        bool e = false;
        bool i = false;
        bool n = false;
        bool x = false;
        bool b = false;
        bool p = false;
        if(op[0] == '+') {
            e = true;
        }
        if(operand != string("")) {
            if(operand[0] == '#') {
                i = true;
                operand = operand.substr(1, operand.size());
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                }
            } else if (operand[0] == '@') {
                n = true;
                operand = operand.substr(1, operand.size());
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                }
            } else if (operand[0] == '=') {
                operandHex = getLiteralValue(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("literal not found or address exceeded size");
                }
            } else if (operand == string("*")) {
                operandHex = opTab[j].getAddress();
            } else if(isIndexedAddressing(operand)) {
                x = true;
                i = true;
                n = true;
                operand = getIndexedSymbol(operand);
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                }
            } else {
                i = true;
                n = true;
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                }
            }
            disp = displacement(locctr, operandHex, base, e, b, p);
        }
        OpCodeParameters param;
        param.setAddress(disp);
        param.setB(b);
        param.setE(e);
        param.setI(i);
        param.setN(n);
        param.setOperation(opHex);
        param.setP(p);
        param.setX(x);
        opTab[j].setOpCode(param.getObjectCode());
    }
}

bool isIndexedAddressing(string operand) {
    regex r("[a-zA-Z][a-zA-Z0-9]+ *, *[xX]");
    return regex_match(operand, r);
}
string getIndexedSymbol(string indexed) {
    regex r("([a-zA-Z][a-zA-Z0-9]+) *, *[xX]");
    smatch m;
    if(regex_search(indexed, m, r)) {
        return m.str(1);
    }
}

vector<pair<int,std::string>> Pass2::getLitTab() {
    return litTab;
}
void Pass2::setLitTab(vector<pair<int,std::string>> litTab) {
    this->litTab = litTab;
}

void Pass2::setBaseAddress(string operand) {
    try {
        base = stoi(operand);
    } catch (std::invalid_argument arg) {
        unordered_map<string, Symbol>::const_iterator iter = symTab.find(operand);
        if(iter == symTab.end()) {
            base = 16777215;
        } else {
            base = iter->second.address;
        }
    }
}

int Pass2::getNumericOperand(string operand) {
    try {
        return stoi(operand);
    } catch (std::invalid_argument arg) {
        unordered_map<string, Symbol>::const_iterator iter = symTab.find(operand);
        if(iter == symTab.end()) {
            return 16777215;
        } else {
            return iter->second.address;
        }
    }
}

int Pass2::getLiteralValue(string operand) {
    regex r("=(x|c)\'([a-zA-Z0-9]+)\'");
    if(regex_match(operand, r)) {
        smatch m;
        if(regex_search(operand, m, r)) {
            string type = m.str(1);
            string s = m.str(2);
            if(type == string("x")) {
                for(pair<int, string> lit : litTab) {
                    if(lit.second == s) {
                        return lit.first;
                    }
                }
            } else if(type == string("c")) {
                string hex = hexa_format_of_literal(s);
                for(pair<int, string> lit : litTab) {
                    if(lit.second == hex) {
                        return lit.first;
                    }
                }
            }
        }
    }
    return 16777215;
}

/*bool isHex(string s) {
    for(char c : s) {
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ) {
            continue;
        }
        return false;
    }
    return true;
}*/
string hexa_format_of_literal(string str) {
    string hexa_form = "";
    //convert the charachters to thier hexa format
    for(int i = 0 ; i < str.size() ; i++) {
        int x = str[i];
        hexa_form += HexFromDecimal(x);
    }
    return hexa_form;
}

/*string HexFromDecimal(int num) {
    int temp = num;
    int i = 0, j, r ;
    string hex = "";
    while (temp != 0) {
        r = temp % 16;
        if (r < 10) {
            hex += r + 48;
        } else {
            hex += r + 55;
        }
        temp = temp / 16 ;
    }
    reverse(hex.begin(), hex.end());
    return hex;
}*/
