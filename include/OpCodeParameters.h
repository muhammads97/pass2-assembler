#ifndef OPCODEPARAMETERS_H
#define OPCODEPARAMETERS_H
#include <string>
using namespace std;

class OpCodeParameters
{
    public:
        OpCodeParameters();
        void setOperation(int operation);
        void setN(bool n);
        void setI(bool i);
        void setX(bool x);
        void setB(bool b);
        void setE(bool e);
        void setP(bool p);
        void setAddress(int address);
        int getObjectCode();

    protected:

    private:
        bool n = false;
        bool i = false;
        bool x = false;
        bool b = false;
        bool p = false;
        bool e = false;
        int operation = 0;
        int address = 0;
        int opCode = 0;
};

#endif // OPCODEPARAMETERS_H
