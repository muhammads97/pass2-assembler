#include "AddressTranslation.h"

int twoscomp(int num,int bits) {
    num = abs(num);
    int result = 0,added = 1;
    int t = 0;
    while(num) {
        t++;
        int d = num % 2;
        num /= 2;
        if(!d) {
            result += added;
        }
        added *= 2;
    }
    while( t < bits ) {
        result += added;
        t++;
        added *= 2;
    }
    return result+1;
}

int displacement(int locctr, int operandAddress, int baseAddress, bool e, bool &b, bool &p) {
    if(e) {
        if(operandAddress < 0){
            operandAddress = twoscomp(operandAddress, 20);
        }
        return operandAddress;
    } else {
        int disp = operandAddress - locctr;
        if(disp > 2047 || disp < -2048) {
            if(baseAddress == 16777215){
                b = true;
                p = false;
                return baseAddress;
            }
            disp = operandAddress - baseAddress;
            if(disp > 4095 || disp < 0){
                disp = 16777215;
            }
            b = true;
            p = false;
        } else {
            p = true;
        }
        if(disp < 0){
            disp = twoscomp(disp, 12);
        }
        return disp;
    }
}
