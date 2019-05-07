#include <iostream>
#include "OpCodeParameters.h"
#include "AddressTranslation.h"
#include "Pass1.h"
#include "WriteFile.h"
#include <regex>
using namespace std;

int main()
{
    string s;
    getline(cin,s);
    regex second ("( *pass1 *)([a-zA-Z0-9\.]*(txt))",  std::regex_constants::icase );
    smatch  m;
    regex_search(s, m, second);


    string file_name =   m.str(m.size() -2);
    Pass1 builder(file_name);
    vector<Instruction> v = builder.getInstructions();

    WriteFile p(v,builder.literals_data,builder.pc,builder.symbols,file_name);

    return 0;
}
