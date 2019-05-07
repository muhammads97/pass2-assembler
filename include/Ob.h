#ifndef OB_H
#define OB_H
#include<string>
#include <string.h>

class Ob
{
    public:

        int format;
        int opCode;
        Ob(int f, int op);
        int getFormat();
        int getOpCode();
};

#endif // OB_H
