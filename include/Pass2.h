#ifndef PASS2_H
#define PASS2_H
#include <unordered_map>
#include <string>
#include "Symbol.h"
#include "Instrucion.h"
#include "objectCodeMap.h"
#include "OpCodeParameters.h"
#include <vector>
using namespace std;


string getIndexedSymbol(string indexed);
bool isIndexedAddressing(string operand);
class Pass2
{
    public:
        Pass2();
        virtual ~Pass2();
        void setOpTable(vector<Instrucion> opTab);
        void setSymTable(unordered_map<string, Symbol> symTab);
        unordered_map<string, Symbol> getSymTable();
        vector<Instrucion> getOpTable();
        vector<Instrucion> execute();

    protected:

    private:
        unordered_map<string, Symbol> symTab;
        vector<Instrucion> opTab;
        objectCodeMap opCodes;
        int base = 16777215;
        void setBaseAddress(string operand);
};

#endif // PASS2_H
