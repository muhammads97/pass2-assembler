#include "parser.h"
#include <regex>
#include "objectCodeMap.h"
#include <bits/stdc++.h>
using namespace std;
void check(Instruction* i);
bool isHex(std::string s);
Instruction parse(std::string in) {
    //transform(in.begin(), in.end(), in.begin(), ::tolower);
    using namespace std::regex_constants;
    regex operation_one_operand(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(\\+)?(ADD|ADDF|AND|COMP|COMPF|DIV|DIVF|J|JEQ|JGT|JLT|JSUB|LDA|LDB|LDCH|LDF|LDL|LDS|LDT|LDX|LPS|MUL|MULF|OR|RD|SSK|STA|STB|STCH|STF|STI|STL|STS|STSW|STT|STX|SUB|SUBF|TD|TIX|WD) *( +([#@\\*a-zA-Z0-9\'\\+\\-=]+)) *(\\..*)?", icase);
    regex operation_one_register(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(CLEAR|TIXR) *( +([AXLBSTFaxlbstf])) *(\\..*)?", icase);
    regex operation_two_register(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(ADDR|COMPR|DIVR|MULR|RMO|SUBR) *( +([AXLBSTFaxlbstf] *, *[AXLBSTFaxlbstf])) *(\\..*)?", icase);
    regex shift_operations(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(SHIFTL|SHIFTR|SVC) *( +([AXLBSTFaxlbstf] *, *[0-9]+)) *(\\..*)?", icase);
    regex operation_no_operand(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(\\+)?(FIX|FLOAT|HIO|NORM|RSUB|SIO|TIO) *(\\..*)?", icase);
    regex commentRegex(" *\\..*", icase);
    regex directivesWithOperand(" *((([a-zA-Z][a-zA-Z0-9]*) +)?) *(\\+)?(START|ORG|BASE|RESW|RESW|RESB|WORD|BYTE|EQU) *( +([#@\\*a-zA-Z0-9\'\\+\\-=]+)) *(\\..*)?", icase);
    regex endDirective(" *(END) *( +([#@\\*a-zA-Z0-9\'\\+\\-=]+))? *(\\..*)?", icase);
    regex NOBASELTORGDirective(" *(NOBASE|LTORG) *(\\..*)?", icase);
    if(regex_match(in, operation_one_operand)) {
        smatch m;
        if(regex_search(in, m, operation_one_operand)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(5);
            operand = m.str(7);
            comment = m.str(8);
            if(m.str(4) == string("+")) {
                e = true;
                operation = "+"+operation;
            }
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }
    } else if(regex_match(in, operation_one_register)) {
        smatch m;
        if(regex_search(in, m, operation_one_register)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(4);
            operand = m.str(6);
            comment = m.str(7);
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }
    } else if(regex_match(in, operation_two_register)) {
        smatch m;
        if(regex_search(in, m, operation_two_register)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(4);
            operand = m.str(6);
            comment = m.str(7);
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }
    } else if(regex_match(in, shift_operations)) {
        smatch m;
        if(regex_search(in, m, shift_operations)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(4);
            operand = m.str(6);
            comment = m.str(7);
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }
    } else if(regex_match(in, operation_no_operand)) {
        smatch m;
        if(regex_search(in, m, operation_no_operand)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(5);
            operand = "";
            comment = m.str(6);
            if(m.str(4) == string("+")) {
                e = true;
                operation = "+"+operation;
            }
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }
    } else if(regex_match(in, commentRegex)) {
        smatch m;
        if(regex_search(in, m, commentRegex)) {
            string symbol = "";
            string operation = "";
            string operand = "";
            string comment = in;
            Instruction i(symbol, operation, operand);
            i.setCommentflag(true);
            i.setCommentMsg(comment);
            //cout << i.toString() << endl;
            return i;

        }
    } else if(regex_match(in, directivesWithOperand)) {
        smatch m;
        if(regex_search(in, m, directivesWithOperand)) {
            string symbol;
            string operation;
            string operand;
            string comment;
            bool e = false;
            symbol = m.str(3);
            operation = m.str(5);
            operand = m.str(7);
            comment = m.str(8);
            if(m.str(4) == string("+")) {
                e = true;
                operation = "+"+operation;
            }
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            if(operation == string("byte") || operation == string("BYTE"))
                i.isByte = true;
            check(&i);
            return i;
        }
    } else if(regex_match(in, endDirective)) {
        smatch m;
        if(regex_search(in, m, endDirective)) {
            string symbol = "";
            string operation;
            string operand;
            string comment;
            bool e = false;
            //symbol = m.str(3);
            operation = m.str(1);
            operand = m.str(3);
            comment = m.str(4);
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }

    } else if(regex_match(in, NOBASELTORGDirective)) {
        smatch m;
        if(regex_search(in, m, NOBASELTORGDirective)) {
            string symbol = "";
            string operation;
            string operand = "";
            string comment;
            bool e = false;
            //symbol = m.str(3);
            operation = m.str(1);
            //operand = m.str(3);
            comment = m.str(2);
            Instruction i(symbol, operation, operand);
            i.extended = e;
            if(comment.size() > 0) {
                i.setCommentflag(true);
                i.setCommentMsg(comment);
            }
            check(&i);
            return i;
        }

    } else {
        Instruction i("", "", "");
        i.setErrorFlag(true);
        i.setErrorMsg("didn't match any valid instruction");
        return i;
    }
}
void check(Instruction* i) {

    std::string s = i->getOperation();
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::string er[] = {"a","b","s","sw","t","r","f","x","l","pc"};
    std::unordered_set<std::string>sett;
    for(int i = 0; i < 10; i++) {
        sett.insert(er[i]);
    }
    // need two registers
    if(s == "rmo" || s == "compr" || s == "addr" || s == "subr" || s == "divr" || s == "mulr") {

        std::regex r1("(.+)([\\,])(.+)");
        std::string salah = i->getOperand();
        std::smatch m;
        if(std::regex_match(salah,r1)) {
            if(std::regex_search(salah, m, r1)) {
                std::string s1 = m.str(1);
                std::string s2 = m.str(2);
                std::string s3 = m.str(3);
                std::string operation = i->getOperation();
                transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
                transform(s3.begin(), s3.end(), s3.begin(), ::tolower);
                if(sett.find(s1) == sett.end() || sett.find(s3) == sett.end() || s2 != ",") {
                    i->setErrorFlag(true);
                    i->setErrorMsg("invalid register name");
                } else {

                }
            }
        } else {
            i->setErrorFlag(true);
            i->setErrorMsg("should be register");
        }
        return;
    }
    // need one register
    else if(s == "tixr") {
        string s3 = i->getOperand();
        transform(s3.begin(), s3.end(), s3.begin(), ::tolower);
        if(sett.find(s3) == sett.end()) {
            i->setErrorFlag(true);
            i->setErrorMsg("invalid register name");
        }
        return;
    }
    //cant have expression
    // rsub shoudn't have label or operand
    if(s == "rsub") {
        if( (i-> getOperand()).size() != 0 || (i-> getLabel()).size() != 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("rsub can not have label or operand");
            return;
        }
    }
    // end must not have label
    if(s == "end") {
        if((i-> getLabel()).size() != 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("end can't have label");
            return;
        }
    }
    // end and rsub only should have operand
    if(s != "end" && s != "rsub") {

        if( (i-> getOperation()) == "") {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction should have operand");
            return;
        }
    }
    if(s == "nobase") {
        if( (i-> getLabel()).size() != 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have label");
            return;
        }
        if( (i-> getOperand()).size() != 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have Operand");
            return;
        }
    }
    if(s == "base") {
        if( (i-> getLabel()).size() != 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have label");
            return;
        }
        if( (i-> getOperand()).size() == 0) {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction should have operand");
            return;
        }
    }
    if(s.size() > 1)
        if(s[0] =='j' || (s[0] == '+' && s[1] == 'j') ) {
            if( (i-> getOperand()).size() == 0) {
                i-> setErrorFlag(true);
                i-> setErrorMsg("this instruction should have operand2");
            }
        }
    if(s.size() > 0)
        if(s[s.size()-1] == 'r' && s[0] == '+') {
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction of format two!");
        }
    // expression can not have constant as the first instruction
    std::string str = i->getOperand();
    std::regex r("(.+)([\\-\\+])(.+)");
    std::smatch m;
    std::string salah = i->getOperand();
    if(std::regex_match(salah,r)) {
        if(std::regex_search(salah, m, r)) {
            std::string s1 = m.str(1);
            std::string s2 = m.str(2);
            std::string s3 = m.str(3);
            if(isHex(s1)) {
                i->setErrorFlag(true);
                i->setErrorMsg("not valid expression");
            }

        }
    }
    std::regex r3("(.+)([\\*\\%])(.+)");
    std::smatch mm;
    salah = i->getOperand();
    if(std::regex_match(salah,r3)) {
        if(std::regex_search(salah, mm, r3)) {
            std::string s1 = mm.str(1);
            std::string s2 = mm.str(2);
            std::string s3 = mm.str(3);
            i->setErrorFlag(true);
            i->setErrorMsg("not valid expression");

        }
    }


}
bool isHex(std::string s) {
    for(char c : s) {
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ) {
            continue;
        }
        return false;
    }
    return true;
}
