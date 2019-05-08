#ifndef PASS2_H
#define PASS2_H
#include <unordered_map>
#include <string>
#include <regex>
#include "Symbol.h"
#include "Instruction.h"
#include "objectCodeMap.h"
#include "OpCodeParameters.h"
#include "AddressTranslation.h"
#include <vector>
#include <iomanip>
using namespace std;
using namespace std::regex_constants;

void parseReg(string operand, string &r1, string &r2);
int regValue(string reg);
string HexFromDecimal(int num);
string hexa_format_of_literal(string str);
string getIndexedSymbol(string indexed);
bool isIndexedAddressing(string operand);
string byteHandler(string operand);
class Pass2
{
    public:
        Pass2();
        virtual ~Pass2();
        void setOpTable(vector<Instruction> opTab);
        void setSymTable(unordered_map<string, Symbol> symTab);
        unordered_map<string, Symbol> getSymTable();
        vector<Instruction> getOpTable();
        vector<Instruction> execute();
        vector<pair<int ,std::string>> getLitTab();
        void setLitTab(vector<pair<int ,std::string>> litTab);
    protected:

    private:
        unordered_map<string, Symbol> symTab;
        vector<pair<int ,std::string>> litTab;
        vector<Instruction> opTab;
        objectCodeMap opCodes;
        int base = 16777215;
        void setBaseAddress(string operand);
        int getLiteralValue(string operand);
        int getNumericOperand(string operand);
};

#endif // PASS2_H
