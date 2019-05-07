#include "parser.h"
#include <regex>
#include "objectCodeMap.h"
#include <bits/stdc++.h>
using namespace std;
void check(Instruction* i);
bool isHex(std::string s);
Instruction parse(std::string in){
    using namespace std::regex_constants;
    /*in = in + ' ';
    in = ' ' + in;
    transform(in.begin(), in.end(), in.begin(), ::toupper);
    std::regex comment(" *\\. *.*");
    std::string opMatcher = "BYTE|WORD|RESB|RESW|START|END|WD|TIXR|TIO|SVC|SUBR|SUB|STX|STT|STS|LDCH|LDB|HIO|LDA|JGT|JEQ|STB|FLOAT|JLT|COMPF|NORM|SSK|ADDR|FIX|ADD|JSUB|RD|CLEAR|STSW|LDT|COMP|ORG|EQU|DIVR|TIX|LPS|DIVF|AND|LDL|LDS|J|STCH|MUL|MULR|OR|RMO|SUBF|STF|COMPR|ADDF|RSUB|LDX|SHIFTL|LDF|STL|TD|SHIFTR|DIV|SIO|STA|MULF|STI";
    std::regex r2(opMatcher);
    std::regex r ("( *[a-zA-Z][a-zA-Z0-9]*)?( +((\\+)?"+(opMatcher)+"))( +([#=]?[-\\+,a-zA-Z0-9\'\*]+))?( +(.*))? *", icase);
    std::smatch m;
    */

    // check for division
    for(int i = 0; i < in.size(); i++){
        if(in[i] == '/'){
            Instruction i(in, "", "");
            i.setErrorFlag(true);
            i.setErrorMsg("invalid instruction");
            return i;
        }
    }

    // check for expression with spaces
    /*
    std::string testSpace = "";
    for(int i = 0; i < in.size(); i++){
        if(testSpace.size()){
            if(testSpace[testSpace.size()-1] == ',' || testSpace[testSpace.size()-1] == '+' || testSpace[testSpace.size()-1] == '-'){
                if(in[i] == ' ')
                    continue;
            }
        }
        testSpace += in[i];
    }
    in = "";
    for(int i = testSpace.size()-1; i >= 0; i--){
        if(in.size()){
            if(in[0] == ',' || in[0] == '+' || in[0] == '-'){
                if(testSpace[i] == ' ')
                    continue;
            }
        }
        in = testSpace[i] + in;
    }
    */
    // end of expression



    std::string comment = "";
    std::string inString = "";
    bool hasIns = false;
    for(int i = 0; i < in.size(); i++){
        if(in[i] != ' ' && in[i] != '.'){
            hasIns = true;
        }
        if(in[i] == '.'){
            while(i < in.size()){
                comment += in[i];
                i++;
            }
        }else{
            inString += in[i];
        }
    }
    if(!hasIns){
        Instruction i("", "", "");
        if(comment.size()){
            i.setCommentflag(true);
            i.setCommentMsg(comment);
        }
        return i;
    }

    //has Instruction and has comment if comment size more than zero

    unordered_set<std::string>directives;
    // add all directives in capital letter
    directives.insert("START");
    directives.insert("BASE");
    directives.insert("NOBASE");
    directives.insert("LTORG");
    directives.insert("END");
    directives.insert("RESW");
    directives.insert("RESB");
    directives.insert("WORD");
    directives.insert("BYTE");
    directives.insert("EQU");
    directives.insert("ORG");
    //end adding directives

    vector<std::string>types;
    int operandStart = 0;
    objectCodeMap codeMap;
    for(int i = 0; i < inString.size(); i++){
        string x = "";
        if(inString[i] != ' '){
            while(i < inString.size() && inString[i] != ' '){
                x += inString[i];
                i++;
            }
            types.push_back(x);
            //if( codeMap.getTheFormat(x) != -1 || directives.find(x) != directives.end()){
              //  operandStart = i+1;
                //break;
            //}
        }
    }
    string operandString = "";
    for(int i = operandStart; i < inString.size(); i++){
        operandString += inString[i];
    }
     string s;
     int i;
     /*while(i < operandString.size() && operandString[i] == ' '){
        i++;
     }
     int endd = operandString.size()-1;
     while(endd >= 0 && operandString[endd] == ' '){
        endd--;
     }
     for( ; i <= endd; i++){
        s += operandString[i];
     }
     if(s.size())
        types.push_back(s);
    */
    // has no instruction;
    if(types.size() == 0){
        Instruction i("", "", "");
        if(comment.size()){
            i.setCommentflag(true);
            i.setCommentMsg(comment);
        }
        return i;
    }

    // more than three words
    if(types.size() > 3){
        Instruction i(types[0], types[1], types[2]);
        if(comment.size()){
            i.setCommentflag(true);
            i.setCommentMsg(comment);
        }
        i.setErrorFlag(true);
        i.setErrorMsg("does not match the validation");
        return i;
    }

    // valid number of words

    i = 0;
    if(types.size() == 3)
        i = 1;

    for(; i < types.size(); i++){
        string t = types[i];
        transform(t.begin() , t.end() ,t.begin() , ::toupper);
        if( codeMap.getTheFormat(t) != -1 || directives.find(t) != directives.end()){
            std::string label = "", operation = "", operand = "";
            operation = types[i];
            if(i)
                label = types[i-1];
            if(i+1 < types.size())
                operand = types[i+1];

            Instruction ii(label, operation, operand);
            if(comment.size()){
                ii.setCommentflag(true);
                ii.setCommentMsg(comment);
            }
            check(&ii);
            return ii;
        }
    }
        std::string label = types.size() >= 1 ? types[0] : "", operation = types.size() >= 2 ? types[1] : "",
         operand = types.size() >= 3 ? types[2] : "";
        Instruction ii(label, operation, operand);
        if(comment.size()){
            ii.setCommentflag(true);
            ii.setCommentMsg(comment);
        }
        ii.setErrorFlag(true);
        ii.setErrorMsg("instruction doesn't have operation");
        return ii;
}
void check(Instruction* i){

    std::string s = i->getOperation();
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::string er[] = {"a","b","s","sw","t","r","f","x","l","pc"};
    std::unordered_set<std::string>sett;
    for(int i = 0; i < 10; i++){
        sett.insert(er[i]);
    }
    // need two registers
    if(s == "rmo" || s == "compr" || s == "addr" || s == "subr" || s == "divr" || s == "mulr"){

        std::regex r1("(.+)([\\,])(.+)");
        std::string salah = i->getOperand();
        std::smatch m;
        if(std::regex_match(salah,r1)){
            if(std::regex_search(salah, m, r1)){
                std::string s1 = m.str(1);
                std::string s2 = m.str(2);
                std::string s3 = m.str(3);
                std::string operation = i->getOperation();
                transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
                transform(s3.begin(), s3.end(), s3.begin(), ::tolower);
                if(sett.find(s1) == sett.end() || sett.find(s3) == sett.end() || s2 != ","){
                    i->setErrorFlag(true);
                    i->setErrorMsg("invalid register name");
                }else{

                }
            }
        }else{
                i->setErrorFlag(true);
                i->setErrorMsg("should be register");
        }
        return;
    }
    // need one register
    else if(s == "tixr"){
        string s3 = i->getOperand();
        transform(s3.begin(), s3.end(), s3.begin(), ::tolower);
        if(sett.find(s3) == sett.end()){
            i->setErrorFlag(true);
            i->setErrorMsg("invalid register name");
        }
        return;
    }
    //cant have expression
    /*
    if(s.size() >= 1)
    if(s == "add" || s == "sub" || s == "div" || s == "mul" || s == "j" || s == "jeq" || s == "jlt"||
       s == "jgt" || s == "tix" || s == "comp" || s == "compr" || s == "rmo" || ( s.size() >= 2 && s[0] == 's' && s[1] == 't') ||
       s == "ldch" || s[s.size()-1] == 'r' || s[0] == 'j'){
        //std::regex r1("(.+)([\\,])(.+)");
        std::regex r1("(.+)([\\-\\+])(.+)");
        std::string salah = i->getOperand();
        std::smatch m;
        if(std::regex_match(salah,r1)){
            if(std::regex_search(salah, m, r1)){
                i->setErrorFlag(true);
                i->setErrorMsg("cant have expression");
            }
        }
    }
    */
    // rsub shoudn't have label or operand
    if(s == "rsub"){
        if( (i-> getOperand()).size() != 0 || (i-> getLabel()).size() != 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("rsub can not have label or operand");
            return;
        }
    }
    // end must not have label
    if(s == "end"){
        if((i-> getLabel()).size() != 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("end can't have label");
            return;
        }
    }
    // end and rsub only should have operand
    if(s != "end" && s != "rsub"){

        if( (i-> getOperation()) == ""){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction should have operand");
            return;
        }
    }
    if(s == "nobase"){
        if( (i-> getLabel()).size() != 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have label");
            return;
        }
        if( (i-> getOperand()).size() != 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have Operand");
            return;
        }
    }
    if(s == "base"){
        if( (i-> getLabel()).size() != 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction shouldn't have label");
            return;
        }
        if( (i-> getOperand()).size() == 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction should have operand");
            return;
        }
    }
    if(s.size() > 1)
    if(s[0] =='j' || (s[0] == '+' && s[1] == 'j') ){
        if( (i-> getOperand()).size() == 0){
            i-> setErrorFlag(true);
            i-> setErrorMsg("this instruction should have operand2");
        }
    }
    if(s.size() > 0)
    if(s[s.size()-1] == 'r' && s[0] == '+'){
        i-> setErrorFlag(true);
        i-> setErrorMsg("this instruction of format two!");
    }
    // expression can not have constant as the first instruction
    std::string str = i->getOperand();
    std::regex r("(.+)([\\-\\+])(.+)");
    std::smatch m;
    std::string salah = i->getOperand();
    if(std::regex_match(salah,r)){
        if(std::regex_search(salah, m, r)){
            std::string s1 = m.str(1);
            std::string s2 = m.str(2);
            std::string s3 = m.str(3);
            if(isHex(s1)){
                i->setErrorFlag(true);
                i->setErrorMsg("not valid expression");
            }

        }
    }
    std::regex r3("(.+)([\\*\\%])(.+)");
    std::smatch mm;
    salah = i->getOperand();
    if(std::regex_match(salah,r3)){
        if(std::regex_search(salah, mm, r3)){
            std::string s1 = mm.str(1);
            std::string s2 = mm.str(2);
            std::string s3 = mm.str(3);
            i->setErrorFlag(true);
            i->setErrorMsg("not valid expression");

        }
    }


}
bool isHex(std::string s){
    for(char c : s){
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ){
            continue;
        }
        return false;
    }
    return true;
}
