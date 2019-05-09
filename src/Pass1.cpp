#include "Pass1.h"
#include "parser.h"
#include "Instruction.h"
#include<regex>
Pass1::Pass1(std::string path) {
    obectCode.setTheMap();
    readd(path);
    passOne();
    setIInvalidLiterals();
}
std::vector<Instruction> Pass1::getInstructions() {
    return instrucions;
}
std::string HexFromDecimal(int num) {
    int temp = num;
    int i = 0, j, r ;
    std::string hex = "";
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
}
void Pass1::endop(bool* endSt, Instruction* ins) {
    if(!endSt) {
        ins-> setErrorFlag(true);
        valid = false;
        ins-> setErrorMsg("end is happened before");
    } else {
        *endSt = true;
    }
    std::string s = ins-> getOperand();
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    if(isHex(s) && s.size() != 0) {
        ins-> setErrorFlag(true);
        valid = false;
        ins-> setErrorMsg("should be relative end");
    }
    instrucions.push_back(*ins);
}
void Pass1::startop(Instruction* ins) {
    if(firstStart) {
        firstStart = false;
        if( (ins->getOperand()).size() != 0 && (!isHex(ins->getOperand()) || (ins->getOperand()).size() > 6) ) {
            ins-> setErrorFlag(true);
            valid = false;
            ins-> setErrorMsg("start address must be hexadecimal less than 6");
        } else {
            if((ins->getOperand()).size() != 0) {
                std::string st = ins->getOperand();
                pc = std::stoi(st, 0, 16);
            }
            std::string s = ins->getLabel();
            if(s.size() != 0)
                transform(s.begin(), s.end(), s.begin(), ::tolower);
            if(s.size() != 0)
                if(symbols.find(s) == symbols.end()) {
                    symbols[s].address = 0;
                    instrucions.push_back(*ins);
                    return;
                }
        }
    } else {
        ins-> setErrorFlag(true);
        valid = false;
        ins-> setErrorMsg("start happened before");
    }
    instrucions.push_back(*ins);
}
void Pass1::passOne() {

    bool endSt = false;
    for(int i = 0; i < lines.size(); i++) {
        Instruction instr = parse(lines[i]);//get instrucion from parser
        instr.setAddress(pc);
        //wrong from rejex
        if(instr.isWrong()) {
            valid = false;
            instrucions.push_back(instr);
            continue;
        }
        std::string op = instr.getOperation();
        transform(op.begin(), op.end(), op.begin(), ::tolower);
        //comment and does't have instruction
        if(instr.isComment() && instr.getOperation().size() == 0) {
            instrucions.push_back(instr);
            continue;
        }

        if(op == "end") {
            endop(&endSt,&instr);
            continue;
        }
        if(op == "start") {
            startop(&instr);
            continue;
        }
        firstStart = false;
        int format = obectCode.getTheFormat(op);//+ format get format from opcode
        if(format == -1) {
            int pc2 = get_locctr_address(&instr, pc);// get address from special character table
            if(op != "org" && op != "equ")
                instr.setFormat(pc2 - pc);
            if(op == "ltorg") {
                pc = pc2;
                continue;
            }
            pc = pc2;
        } else {
            if(endSt) {
                if(endSt) {
                    instr.setErrorFlag(true);
                    valid = false;
                    instr.setErrorMsg("statement after end statement");
                    instrucions.push_back(instr);
                    continue;
                }
            }
            pc += format;
            instr.setFormat(format);
        }
        handle_literals(instr);

        if(instr.getLabel() != "") {

            std::string s = instr.getLabel();
            std::string s0 = instr.getOperation();
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            transform(s0.begin(), s0.end(), s0.begin(), ::tolower);
            if(s0 == "org" || s0 == "equ") {
                instrucions.push_back(instr);
                continue;
            }
            if(s.size() != 0) {
                if(symbols.find(s) == symbols.end()) {

                    symbols[s].address = instr.getAddress();
                } else {
                    instr.setErrorFlag(true);
                    valid = false;
                    instr.setErrorMsg("redefined symbol");
                }
            }
        }
        instrucions.push_back(instr);

    }

    int prog_count = pc;
    for(int i = 0 ; i < literals_pool.size() ; i++) {
        unordered_map<string,pair<int,string>>::iterator it ;
        for(it = litTable.begin(); it != litTable.end(); it++ ) {
            if( it->second.second == literals_pool[i]) {
                int disp = it->second.first ;
                it->second.first = prog_count;
                prog_count += disp;
            }
        }
    }
}
void Pass1::readd(std::string path) {
    lines = f.read(path);
}

bool Pass1::isInt(std::string s) {
    for(int i = 0 ; i < s.size() ; i++) {
        if(!(s[i] >= '0' && s[i] <= '9')) {
            return false;
        }
    }
    return true;
}
int Pass1::get_locctr_address(Instruction* ins,int loc_ctr) {
    std::string op = ins->getOperation();
    std::string operand = ins->getOperand();
    transform(op.begin(), op.end(),op.begin(), ::tolower);
    if(op == "base" || op == "nobase") {
        return loc_ctr;
    }
    if(op == "byte") {
        if((operand[0] == 'x' || operand[0] == 'X' )&& (operand[1] == '\'' && operand[operand.size()-1] == '\'')) {
            std::string str = operand.substr(2, operand.size() - 3 );
            if(str.size()%2 != 0) {
                ins->setErrorFlag(true);
                ins->setErrorMsg("odd number of half bytes");
                return loc_ctr;
            }
            loc_ctr += str.size() /2;
        } else if ((operand[0] == 'c' || operand[0] == 'C') && (operand[1] == '\'' && operand[operand.size()-1] == '\'')) {
            std::string str = operand.substr(2, operand.size() - 3);
            loc_ctr += str.size() ;
            return loc_ctr;

        } else {
            ins->setErrorFlag(true);
            ins->setErrorMsg("incomplete assembly");
            return loc_ctr;
        }
    } else if(op == "resb") { //done
        if(!isInt(operand)) {
            ins->setErrorFlag(true);
            valid = false;
            ins->setErrorMsg("error size must be number");
            return loc_ctr;
        }
        loc_ctr +=   std::stoi(ins->getOperand());
    } else if(op == "word") { // done
        loc_ctr += 3;
    } else if(op == "resw") { //done
        if(!isInt(operand)) {
            ins->setErrorFlag(true);
            valid = false;
            ins->setErrorMsg("error size must be number");
            return loc_ctr;
        }
        loc_ctr +=   std::stoi(ins->getOperand() )*3;
    } else if(op == "equ" || op == "org") {
        std::regex r("(.+)([\\-\\+])(.+)");
        if(op == "equ" && ins->getLabel().size() == 0) {
            ins->setErrorFlag(true);
            valid = false;
            ins->setErrorMsg("should have labels");
            return loc_ctr;
        }
        if(op == "equ") {
            std::string s = ins->getLabel();
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if(s.size()!=0)
                if(symbols.find(s) != symbols.end()) {
                    ins->setErrorFlag(true);
                    valid = false;
                    ins->setErrorMsg("redefined labels");
                    return loc_ctr;
                }
        }
        if(op == "org" && ins->getLabel().size() != 0) {
            ins->setErrorFlag(true);
            valid = false;
            ins->setErrorMsg("shouldn't have labels");
            return loc_ctr;
        }
        std::smatch m;
        std::string salah = ins->getOperand();
        if(std::regex_match(salah,r)) {
            if(std::regex_search(salah, m, r)) {

                std::string s1 = m.str(1);
                std::string s2 = m.str(2);
                std::string s3 = m.str(3);
                transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
                transform(s3.begin(), s3.end(), s3.begin(), ::tolower);

                if(isHex(s1) && isHex(s3)) {
                    ins->setErrorFlag(true);
                    valid = false;
                    ins->setErrorMsg("Wrong expression");
                    return loc_ctr;
                }
                int n1 = 0,n2 = 0;
                bool f1 = true,f2 = true,reloc = true;
                if(!isHex(s1)) {
                    if(symbols.find(s1) != symbols.end()) {
                        n1 = symbols[s1].address;
                        f1 = symbols[s1].relocatable;
                    } else {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("Undefined variable1");
                        return loc_ctr;
                    }
                } else {
                    n1 = std::stoi(s1, 0, 16);
                    f1 = false;
                }
                if(!isHex(s3)) {
                    if(symbols.find(s3) != symbols.end()) {
                        n2 = symbols[s3].address;
                        f2 = symbols[s3].relocatable;
                    } else {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("Undefined variable2");
                        return loc_ctr;
                    }
                } else {
                    n2 = std::stoi(s3, 0, 16);
                    f2 = false;
                }
                int z = 0;
                if(s2 == "+") {
                    z = n1 + n2;
                    if(f1 && f2) {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("two added relocatable");
                        return loc_ctr;
                    }
                    if(f1 || f2) {
                        reloc = true;
                    } else {
                        reloc = false;
                    }
                } else {
                    z = n1 - n2;
                    if(f1 && f2) {
                        reloc = false;
                    } else if(f1 && !f2) {
                        reloc = true;
                    } else if(!f1 && f2) {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("not valid expression");
                        return loc_ctr;
                    } else {
                        reloc = false;
                    }

                }
                if(op == "org") {
                    if(!reloc) {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("should be relocatable");
                        return loc_ctr;
                    }
                    loc_ctr = z;
                } else {
                    std::string s = ins->getLabel();
                    transform(s.begin(), s.end(), s.begin(), ::tolower);
                    symbols[s].address = z;
                    symbols[s].relocatable = reloc;
                }
                return loc_ctr;
            }
        } else {
            // one word

            if(op == "org") {
                if(isHex(ins->getOperand())) {
                    ins->setErrorFlag(true);
                    valid = false;
                    ins->setErrorMsg("should be relative");
                    return loc_ctr;
                } else {
                    std::string s3 = ins->getOperand();
                    if(symbols.find(s3) != symbols.end()) {
                        if(!symbols[s3].relocatable) {
                            ins->setErrorFlag(true);
                            ins->setErrorMsg("should be relative");
                            return loc_ctr;
                        }
                        loc_ctr = symbols[s3].address;
                        return loc_ctr;
                    } else {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("Undefined variable from equ");
                        return loc_ctr;
                    }
                }
            } else {
                std::string s3 = ins->getOperand();
                transform(s3.begin(), s3.end(), s3.begin(), ::tolower);
                if(isHex(ins->getOperand())) {
                    std::string s = ins->getLabel();
                    transform(s.begin(), s.end(), s.begin(), ::tolower);
                    if(symbols.find(s) != symbols.end()) {
                        ins->setErrorFlag(true);
                        ins->setErrorMsg("redefined label name");
                        return loc_ctr;
                    }
                    symbols[s].address = std::stoi(s3, 0, 16);
                    symbols[s].relocatable = false;
                    return loc_ctr;
                } else {
                    std::string s = ins->getLabel();
                    transform(s.begin(), s.end(), s.begin(), ::tolower);
                    if(symbols.find(s3) != symbols.end()) {
                        symbols[s].address = symbols[s3].address;
                        symbols[s].relocatable = symbols[s3].relocatable;

                    } else {
                        ins->setErrorFlag(true);
                        valid = false;
                        ins->setErrorMsg("Undefined variable  from euq 2");
                        return loc_ctr;
                    }
                }
            }
        }
        return loc_ctr;
    } else if (op == "ltorg" ) {
        std::string label = ins->getLabel() ;
        std::string operand = ins->getOperand();
        if(operand != "" || label != "") {
            valid = false;
            ins->setErrorFlag(true);
            ins->setErrorMsg("LTORG should not have label or operand");
        } else {
            instrucions.push_back(*ins);
            for(int i = 0 ; i < literals_data.size() ; i++) {
                if(find(all_literals_found.begin(),all_literals_found.end(),literals_pool[i]) == all_literals_found.end()) {
                    Instruction lit("*", literals_pool[i],  "");
                    lit.setStringOpCode(literals_data[i].second);
                    all_literals_found.push_back(literals_pool[i]);
                    lit.setAddress(loc_ctr);
                    unordered_map<string,pair<int,string>>::iterator it ;
                    for(it = litTable.begin(); it != litTable.end(); it++ ) {
                        if( (it->second).second == literals_pool[i]) {
                            (it->second).first = loc_ctr;
                        }
                    }
                    instrucions.push_back(lit);
                    loc_ctr += literals_data[i].first;
                }
            }
            literals_data.clear();
            literals_pool.clear();
            return loc_ctr;
        }
        return loc_ctr;
    } else {
        ins->setErrorFlag(true);
        valid = false;
        ins->setErrorMsg("undefined operation");
        return loc_ctr;
    }
    return loc_ctr;
}


bool Pass1::isHex(std::string s) {
    for(char c : s) {
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ) {
            continue;
        }
        return false;
    }
    return true;
}
void Pass1::handle_literals(Instruction ins ) {
    std::string operand = ins.getOperand();
    std::string operation = ins.getOperation();
    transform(operation.begin(), operation.end(), operation.begin(), ::tolower);
    bool found = false;
    if (ins.getFormat() == 1 || ins.getFormat() == 2) {
        return;
    }
    int ssize = ops_invalid_literals.size();
    for (int i = 0; i < ssize; i++) {
        if (ops_invalid_literals[i] == operation) {
            found = true;
            break;
        }
    }
    if (!found) {
        if(operand[0] == '=') {
            std::string str = operand.substr(3, operand.size() -4);
            if((operand[1] == 'x' || operand[1] == 'X' )&& (operand[2] == '\'' && operand[operand.size()-1] == '\'')) {
                if(str.size()% 2 == 0 &&  str.size() > 0 && str.size() <= 6 && isHex(str)) {
                    if(operation == "ldch" ) {
                        if(str.size() == 2 ) {
                            if(litTable.find(str) == litTable.end()) {
                                literals_data.push_back(make_pair(1,str));
                                literals_pool.push_back(operand);
                                litTable[str] = make_pair(1,operand);
                            }
                        } else {
                            ins.setErrorFlag(true);
                            valid = false;
                            ins.setErrorMsg("incorrect length of literal");
                        }
                    } else {
                        if(litTable.find(str) == litTable.end()) {
                            literals_data.push_back(make_pair(3,str));
                            litTable[str] = make_pair(3,operand);
                            literals_pool.push_back(operand);
                        }
                    }
                } else {
                    ins.setErrorFlag(true);
                    ins.setErrorMsg("incorrect length of literal");
                }
            } else if((operand[1] == 'c' || operand[1] == 'C') && (operand[2] == '\'' && operand[operand.size()-1] == '\'')) {
                if(str.size() == 1 || str.size() == 3) {
                    if(operation == "ldch" && str.size() == 1 ) {
                        int x = str[0];
                        if(litTable.find(HexFromDecimal(x)) == litTable.end()) {
                            literals_data.push_back(make_pair(1,HexFromDecimal(x)));
                            literals_pool.push_back(operand);
                            litTable[HexFromDecimal(x)] = make_pair(1,operand);
                        }
                    } else {
                        std::string ssss = "";
                        for(int i = 0 ; i < str.size() ; i++) {
                            int x = str[i];
                            ssss += HexFromDecimal(x);
                        }
                        int x = str[0];
                        if(litTable.find(ssss) == litTable.end()) {
                            literals_data.push_back(make_pair(3,ssss));
                            literals_pool.push_back(operand);
                            litTable[ssss] = make_pair(3,operand);
                        }
                    }
                } else {
                    ins.setErrorFlag(true);
                    valid = false;
                    ins.setErrorMsg("incorrect length of literal");
                }
            } else { //invalid literal
                ins.setErrorFlag(true);
                valid = false;
                ins.setErrorMsg("invalid literal");
            }

        } else { // not a literal
            return;
        }
    }

}
void Pass1::setIInvalidLiterals() {
    ops_invalid_literals.push_back("sta");
    ops_invalid_literals.push_back("stb");
    ops_invalid_literals.push_back("stf");
    ops_invalid_literals.push_back("sti");
    ops_invalid_literals.push_back("stl");
    ops_invalid_literals.push_back("stx");
    ops_invalid_literals.push_back("stt");
    ops_invalid_literals.push_back("sts");
    ops_invalid_literals.push_back("stch");
    ops_invalid_literals.push_back("rsub");
    ops_invalid_literals.push_back("ssk");
    ops_invalid_literals.push_back("j");
    ops_invalid_literals.push_back("jsub");
    ops_invalid_literals.push_back("jeq");
    ops_invalid_literals.push_back("jlt");
    ops_invalid_literals.push_back("jgt");
}

