#include "Pass2.h"

int getHex(string hexstr) {
    return (int)strtol(hexstr.c_str(), 0, 16);
}

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
    regex skipOperations("(\\+)?(START|EQU|ORG|BASE|NOBASE|LTORG|END|RESW|RESW|RESB|WORD|BYTE)", icase);
    bool started = false;
    int j = 0;
    while(j < opTab.size()) {
        bool err = false;
        //cout << "                                                >>>>>>>" << opTab[j].toString() <<endl;
        if(!started) {
            if(opTab[j].getOperation() != string("start")) {
                cout<<opTab[j].getOperation() << endl;
                j++;
                continue;
            } else {
                started = true;
                j++;

                continue;
            }
        }
        if(opTab[j].getOperation() == string("")) {
            j++;
            continue;
        }
        if(opTab[j].getOperation() == string("end")) {
            break;
        }
        if(opTab[j].getOperation() == string("base")) {
            setBaseAddress(opTab[j].getOperand());
            j++;
            continue;
        }
        if(opTab[j].getLabel() == string("*")){
            int objectCode = getLiteralValue(opTab[j].getOperation());
            opTab[j].setOpCode(getHex(opTab[j].getStringOpCode()));
            cout << opTab[j].toString() <<endl;
            j++;
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
        if(regex_match(op, skipOperations)){
            if(op == string("byte")){
                string objectCode = byteHandler(operand);
                opTab[j].setLongObjectCode(objectCode);
                cout << opTab[j].toString() <<endl;
                j++;
                continue;
            } else if(op == string("word")){
                try {
                    int objectCode = stoi(operand);
                    if(objectCode > 4095){
                        opTab[j].setPass2ErrMsg("operand overflow");
                        opTab[j].setOpCode(0);
                continue;
                    } else {
                        opTab[j].setOpCode(objectCode);
                    }
                    //cout << opTab[j].toString() <<endl;
                    j++;
                    continue;
                } catch(invalid_argument arg){
                    opTab[j].setPass2ErrMsg("illegal operand");
                    opTab[j].setOpCode(0);
                //continue;
                    //cout << opTab[j].toString() <<endl;
                    j++;
                    continue;
                }
            } else {
                j++;
                continue;
            }
        }
        if(operand != string("")) {
            if(operand[0] == '#') {
                i = true;
                operand = operand.substr(1, operand.size());
                //operandHex = getNumericOperand(operand);
                try{
                    operandHex = stoi(operand);
                    if((operandHex < -2048 || operandHex > 4095) && !e){
                        opTab[j].setPass2ErrMsg("address exceeds size try to use extended format");
                        opTab[j].setOpCode(0);
                continue;
                    } else if (operandHex < 0){
                        if(e){
                            operandHex = twoscomp(operandHex, 20);
                        } else {
                            operandHex = twoscomp(operandHex, 12);
                        }
                    }
                    disp = operandHex;
                    //cout << disp << endl;
                } catch (invalid_argument arg) {
                    unordered_map<string, Symbol>::const_iterator iter = symTab.find(operand);
                    if(iter == symTab.end()) {
                        opTab[j].setPass2ErrMsg("using undefined symbol");
                        opTab[j].setOpCode(0);
                continue;
                    } else {
                        operandHex = iter->second.address;
                        disp = displacement(locctr, operandHex, base, e, b, p);
                    }
                }
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                    opTab[j].setOpCode(0);
                continue;
                }
                //disp = displacement(locctr, operandHex, base, e, b, p);
            } else if (operand[0] == '@') {
                n = true;
                operand = operand.substr(1, operand.size());
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                    opTab[j].setOpCode(0);
                continue;
                }
                disp = displacement(locctr, operandHex, base, e, b, p);
            } else if (operand[0] == '=') {
                operandHex = getLiteralValue(operand);
                //cout << hex << operandHex << endl;
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("literal not found or address exceeded size");
                    opTab[j].setOpCode(0);
                continue;
                }
                //disp = displacement(locctr, operandHex, base, e, b, p);
                disp = operandHex;
                i = true;
                n = true;
            } else if (operand == string("*")) {
                operandHex = opTab[j].getAddress();
                disp = displacement(locctr, operandHex, base, e, b, p);
            } else if(isIndexedAddressing(operand)) {
                x = true;
                i = true;
                n = true;
                operand = getIndexedSymbol(operand);
                operandHex = getNumericOperand(operand);
                if(operandHex == 16777215) {
                    opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                    opTab[j].setOpCode(0);
                continue;
                }
                disp = displacement(locctr, operandHex, base, e, b, p);
            } else if (isExpression(operand)){
                //cout << "expression" <<endl;
                if(operand[0] == '#'){
                    i = true;
                    n = false;
                } else if (operand[0] == '@'){
                    n = true;
                    i = false;
                } else {
                    n = true;
                    i = true;
                }
                operandHex = getExpressionValue(operand);
                //cout << hex <<operandHex <<endl;
                if(operandHex == 16777215 || operandHex < 0){
                    opTab[j].setPass2ErrMsg("invalid expression or negative value address");
                    opTab[j].setOpCode(0);
                continue;
                }
                disp = displacement(locctr, operandHex, base, e, b, p);
            } else {
                if(opHex == 0x90 || opHex == 0xA0 || opHex == 0x9C || opHex == 0x98 || opHex == 0xAC || opHex == 0x94){
                    string r1;
                    string r2;
                    parseReg(operand, r1, r2);
                    disp = regValue(r1) << 4;
                    disp = disp | regValue(r2);
                } else if (opHex == 0xB4 || opHex == 0xB8){
                    disp = regValue(operand) << 4;
                } else if (opHex == 0xA4 || opHex == 0xA8){
                    string r1;
                    string n;
                    parseReg(operand, r1, n);
                    int bits = stoi(n) - 1;
                    disp = regValue(r1) << 4;
                    disp = disp | bits;
                } else {
                    i = true;
                    n = true;
                    operandHex = getNumericOperand(operand);
                    if(operandHex == 16777215) {
                        opTab[j].setPass2ErrMsg("symbol not found or address exceeded size");
                        opTab[j].setOpCode(0);
                continue;
                    }
                    disp = displacement(locctr, operandHex, base, e, b, p);
                }

            }

            if(b && disp == 16777215){
                opTab[j].setPass2ErrMsg("base relative while base is not set");
                opTab[j].setOpCode(0);
                continue;
            } else if (disp == 16777215){
                opTab[j].setPass2ErrMsg("address overflow");
                opTab[j].setOpCode(0);
                continue;
            }
        } else {
            n = true;
            i = true;
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
        cout << opTab[j].toString() <<endl;
        j++;
    }
    return opTab;
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

void parseReg(string operand, string &r1, string &r2){
    regex r("([AXLBSTFaxlbstf]) *, *([AXLBSTFaxlbstf])");
    if(regex_match(operand, r)){
        smatch m;
        if(regex_search(operand, m, r)){
            r1 = m.str(1);
            r2 = m.str(2);
        }
    }
}

int regValue(string reg){
    if(reg == string("a") || reg == string("A")){
        return 0;
    } else if (reg == string("x") || reg == string("X")){
        return 1;
    } else if (reg == string("l") || reg == string("L")){
        return 2;
    } else if (reg == string("b") || reg == string("B")){
        return 3;
    } else if (reg == string("s") || reg == string("S")){
        return 4;
    } else if (reg == string("t") || reg == string("T")){
        return 5;
    } else if (reg == string("f") || reg == string("F")){
        return 6;
    }
}
unordered_map<string, pair<int ,string>> Pass2::getLitTab() {
    return litTab;
}
void Pass2::setLitTab(unordered_map<string, pair<int ,string>> litTab) {
    this->litTab = litTab;
    unordered_map<string, pair<int ,string>>::const_iterator iter = litTab.begin();
    /*while(iter != litTab.end()){
        cout << "                                    >>>>>>"<<iter->first << "    " << iter->second.first << "     " << iter->second.second << endl;
        iter++;
    }*/
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
    regex r("=([Xx]|[cC])\'([a-zA-Z0-9]+)\'");
    if(regex_match(operand, r)) {
        smatch m;
        if(regex_search(operand, m, r)) {
            string type = m.str(1);
            string s = m.str(2);
            if(type == string("x") || type == string("X")) {
                unordered_map<string, pair<int, string>>::const_iterator iter = litTab.find(s);
                if(iter == litTab.end()){
                    return 16777215;
                } else {
                    return iter->second.first;
                }
                /*for(pair<int, string> lit : litTab) {
                    if(lit.second == s) {
                        return lit.first;
                    }
                }*/
            } else if(type == string("c") || type == string("C")) {
                string hex = hexa_format_of_literal(s);
                transform(hex.begin(), hex.end(), hex.begin(), ::toupper);
                unordered_map<string, pair<int, string>>::const_iterator iter = litTab.find(hex);
                if(iter == litTab.end()){
                    cout << "ml2aaaash" << endl;
                    return 16777215;
                } else {
                    return iter->second.first;
                }
                /*for(pair<int, string> lit : litTab) {
                    if(lit.second == hex) {
                        return lit.first;
                    }
                }*/
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
        stringstream ss;
        ss << hex << x;
        hexa_form += ss.str();
    }
    return hexa_form;
}

string byteHandler(string operand){
    regex r("([Cc]|[Xx])\'(.+)\'");
    if(regex_match(operand, r)){
        smatch m;
        if(regex_search(operand, m, r)){
            string type = m.str(1);
            string value = m.str(2);
            if(type == string("x") || type == string("X")){
                return value;
            } else if (type == string("c") || type == string("C")){
                return hexa_format_of_literal(value);
            }
        }
    }
    return string("");
}

bool isExpression(string operand){
    regex r("((#|@)?([a-zA-Z0-9]+))(\\+|\\-|\\*|\\/)([a-zA-Z0-9]+)");
    //cout << regex_match(operand, r);
    return regex_match(operand, r);
}

int Pass2::getExpressionValue(string operand){
    regex r("([a-zA-Z0-9]+)(\\+|\\-|\\*|\\/)([a-zA-Z0-9]+)");
    smatch m;
    if(regex_search(operand, m, r)){
        string sym1 = m.str(1);
        string op = m.str(2);
        string sym2 = m.str(3);
        int v1 = getNumericOperand(sym1);
        int v2 = getNumericOperand(sym2);
        if(v1 == 16777215 || v2 == 16777215){
            return 16777215;
        }
        if(op == string("+")){
            return v1+v2;
        } else if(op == string("-")){
            return v1 - v2;
        } else if(op == string("*")){
            return v1*v2;
        } else if(op == string("/")){
            return v1/v2;
        }
    }
}
