#include "OpCodes.h"


ObjectCodes["ADD"] =  new Ob(3 , 0x18);
ObjectCodes["ADDF"] =  new Ob(3 , 0x58);
ObjectCodes["ADDR"] =  new Ob(2 , 0x90);
ObjectCodes["AND"] =  new Ob(3 , 0x40);
ObjectCodes["CLEAR"] =  new Ob(2 , 0xB4);
ObjectCodes["COMP"] =  new Ob(3 , 0x28);
ObjectCodes["COMPF"] =  new Ob(3 , 0x88);
ObjectCodes["COMPR"] =  new Ob(2 , 0xA0);
ObjectCodes["DIV"] =  new Ob(3 , 0x24);
ObjectCodes["DIVF"] =  new Ob(3 , 0x64);
ObjectCodes["DIVR"] =  new Ob(2, 0x9C);
ObjectCodes["FIX"] =  new Ob(1, 0xC4);
ObjectCodes["FLOAT"] =  new Ob(1 , 0xC0);
ObjectCodes["HIO"] =  new Ob(1 , 0xF4);
ObjectCodes["J"] =  new Ob(3 , 0x3C);
ObjectCodes["JEQ"] =  new Ob(3 , 0x30);
ObjectCodes["JLT"] =  new Ob(3 , 0x38);
ObjectCodes["JGT"] =  new Ob(3 , 0x34);
ObjectCodes["JSUB"] =  new Ob(3 , 0x48);
ObjectCodes["LDA"] =  new Ob(3 , 0x00);
ObjectCodes["LDB"] =  new Ob(3 , 0x68);
ObjectCodes["LDCH"] =  new Ob(3 , 0x50);
ObjectCodes["LDF"] =  new Ob(3 , 0x70);
ObjectCodes["LDL"] =  new Ob(3 , 0x08);
ObjectCodes["LDS"] =  new Ob(3 , 0x6C);
ObjectCodes["LDT"] =  new Ob(3 , 0x74);
ObjectCodes["LDX"] =  new Ob(3 , 0x04);
ObjectCodes["LPS"] =  new Ob(3 , 0xD0);
ObjectCodes["MUL"] =  new Ob(3 , 0x20);
ObjectCodes["MULF"] =  new Ob(3 , 0x60);
ObjectCodes["MULR"] =  new Ob(2 , 0x98);
ObjectCodes["NORM"] =  new Ob(1 , 0xCB);
ObjectCodes["OR"] =  new Ob(3 , 0x44);
ObjectCodes["RD"] =  new Ob(3 , 0xD8);
ObjectCodes["RMO"] =  new Ob(2 , 0xAC);
ObjectCodes["RSUB"] =  new Ob(3 , 0x4C);
ObjectCodes["SHIFTL"] =  new Ob(2 , 0xA4);
ObjectCodes["SHIFTR"] =  new Ob(2 , 0xA8);
ObjectCodes["SIO"] =  new Ob(1 , 0xF0);
ObjectCodes["SSK"] =  new Ob(3 , 0xEC);
ObjectCodes["STA"] =  new Ob(3 , 0x0C);
ObjectCodes["STB"] =  new Ob(3 , 0x78);
ObjectCodes["STCH"] =  new Ob(3 , 0x54);
ObjectCodes["STF"] =  new Ob(3 , 0x80);
ObjectCodes["STI"] =  new Ob(3 , 0xD4);
ObjectCodes["STL"] =  new Ob(3 , 0x14);
ObjectCodes["STS"] =  new Ob(3 , 0x7C);
ObjectCodes["STSW"] =  new Ob(3 , 0xE8);
ObjectCodes["STT"] =  new Ob(3 , 0x84);
ObjectCodes["STX"] =  new Ob(3 , 0x10);
ObjectCodes["SUB"] =  new Ob(3 , 0x1C);
ObjectCodes["SUBF"] =  new Ob(3 , 0x5C);
ObjectCodes["SUBR"] =  new Ob(2 , 0x94);
ObjectCodes["SVC"] =  new Ob(2 , 0xB0);
ObjectCodes["TD"] =  new Ob(3 , 0xE0);
ObjectCodes["TIO"] =  new Ob(1 , 0xF8);
ObjectCodes["TIX"] =  new Ob(3 , 0x2C);
ObjectCodes["TIXR"] =  new Ob(2 , 0xB8);
ObjectCodes["WD"] =  new Ob(3 , 0xDC);


int getTheCode(std::string ins){
    transform(ins.begin() , ins.end() ,ins.begin() , ::toupper);
    if (ins[0] == '+'){
        ins.replace(0,1,"");
    }
    if (ObjectCodes.find(ins) == ObjectCodes.end()){
        return -1;
    }
    Ob* temp = ObjectCodes[ins];
    int x = temp -> getOpCode();
    return x;
}

int getTheFormat(std::string ins){
    transform(ins.begin() , ins.end() ,ins.begin() , ::toupper);
    if (ins[0] == '+'){
        ins.replace(0,1,"");
        return 4;
    }

    if (ObjectCodes.find(ins) == ObjectCodes.end()){
        return -1;
    }

    Ob* temp = ObjectCodes[ins];
    int roo = temp -> getFormat();
    return roo;
}
