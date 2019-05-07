#include <iostream>
#include "OpCodeParameters.h"
#include "AddressTranslation.h"
#include <regex>
using namespace std;

int main()
{
    OpCodeParameters p;
    //p.setOperation("STL");
    p.setAddress(45);
    p.setN(1);
    p.setI(1);
    p.setP(1);
    //printf("%x\n", p.getObjectCode());
    bool b;

    //printf("%x\n", displacement(4182, 51, 51, false, b));
    int g = 16777215;
    regex r("[a-zA-Z][a-zA-Z0-9]+ *, *[xX]");
    try{
        printf("%d", stoi("584"));
    } catch(std::invalid_argument error){
        printf("error");
    }

    return 0;
}
