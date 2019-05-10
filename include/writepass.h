#ifndef WRITEPASS_H
#define WRITEPASS_H
#include<algorithm>
#include<Symbol.h>
#include<Instruction.h>
#include <stdint.h>

class writepass
{
    public:
        writepass(std::vector<Instruction> ins,std::string file_name);
        virtual ~writepass();
        std::vector<Instruction> ins;

    protected:

    private:
};

#endif // WRITEPASS_H
