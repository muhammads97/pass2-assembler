#include "AddressTranslation.h"

int displacement(int locctr, int operandAddress, int baseAddress, bool e, bool &b, bool &p){
    if(e){
        return operandAddress;
    } else {
        int disp = operandAddress - locctr;
        if(disp > 2047 || disp < -2048){
            disp = operandAddress - baseAddress;
            b = true;
            p = false;
        } else {
            p = true;
        }
        return disp;
    }
}
