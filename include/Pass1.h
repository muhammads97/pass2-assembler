#ifndef INSTRUCTIONBUILDER_H
#define INSTRUCTIONBUILDER_H
#include <bits/stdc++.h>
#include "Instruction.h"
#include <objectCodeMap.h>
#include <ReadFile.h>
#include "Symbol.h"

using namespace std;

class Pass1
{
    public:
        Pass1(string path);
        void passOne();
        unordered_map<string, Symbol> symbols;
        vector<Instruction> getInstructions();
        vector<pair <int ,string> > literals_data;
        vector<string> literals;
        bool valid = true;
        int pc = 0;
        void setIInvalidLiterals();
        vector<int> ltorg;
    protected:

    private:
        vector<string>lines;
        vector<string> found_literals;
        vector<Instruction>instrucions;
        objectCodeMap obectCode;
        void handle_literals(Instruction ins);
        bool firstStart = true;
        ReadFile f;
        void readd(string path);
        bool isHex(string s);
        int startInst(Instruction instr, bool firstStart);
        int get_locctr_address(Instruction* ins, int loc_ctr);
        bool isInt(string s);
        void endop(bool* endSt, Instruction* ins);
        void startop(Instruction* ins);


};

#endif // INSTRUCTIONBUILDER_H
