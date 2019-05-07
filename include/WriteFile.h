#ifndef WRITEFILE_H
#define WRITEFILE_H
#include<algorithm>
#include<Symbol.h>
#include<Instruction.h>
#include <stdint.h>
class WriteFile
{
    public:

        WriteFile(std::vector<Instruction> ins, std::vector < std::pair <int ,std::string> > literals_data,int pc,
                  std::unordered_map<std::string, Symbol> symbols ,std::string file_name);
        virtual ~WriteFile();
        std::vector<Instruction> ins;

    protected:

    private:
};

#endif // WRITEFILE_H
