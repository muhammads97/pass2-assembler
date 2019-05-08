#ifndef INSTRUCION_H
#define INSTRUCION_H
#include<bits/stdc++.h>
#include <iomanip>
using namespace std;
class Instruction
{
    private:
        int address;
        bool errorFlag = false; //true if error exist
        bool relocatable = true;
        bool commetFlag = false;
        bool pass2Error = false;
        std::string errorMsg; //error message
        std::string pass2ErrMsg;
        //get from Op code
        int opCode;
        string string_opCode;
        int format;
        bool hasOpCode = false;
        //the instrucion component
        std::string label;
        std::string operation;
        std::string operand;
        std::string comment;
    public:
        string toString();
        void setOpCode(int opCode);
        int getOpCode();
        void setStringOpCode(string string_opCode);
        string getStringOpCode();
        Instruction(std::string l, std::string op, std::string oper);
        virtual ~Instruction();
        int getAddress();
        void setRelocatable(bool f);
        bool getRelocatable();
        void setAddress(int add);
        bool isWrong();
        void setErrorFlag(bool f);
        std::string getErrorMsg();
        void setErrorMsg(std::string error);
        std::string getLabel();
        std::string getOperation();
        std::string getOperand();
        int getFormat();
        void setFormat(int f);
        std::string getComment();
        bool isComment();
        void setCommentflag(bool f);
        void setCommentMsg(std::string str);
        bool ishasOpCode();
        bool isPass2Err();
        void setPass2ErrMsg(std::string msg);
        void setLongObjectCode(string obCode);
        string getLongObjectCode();
        bool extended = false;
        bool isByte = false;
        string longObjectCode;
    protected:

};

#endif // INSTRUCIONPARSER_H
