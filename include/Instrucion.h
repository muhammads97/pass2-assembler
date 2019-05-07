#ifndef INSTRUCION_H
#define INSTRUCION_H
#include<bits/stdc++.h>
class Instrucion
{
    private:
        int address;
        bool errorFlag = false; //true if error exist
        bool relocatable = true;
        bool commetFlag = false;
        std::string errorMsg; //error message
        //get from Op code
        int opCode;
        int format;
        bool hasOpCode = false;
        //the instrucion component
        std::string label;
        std::string operation;
        std::string operand;
        std::string comment;
    public:
        void setOpCode(int opCode);
        int getOpCode();
        Instrucion(std::string l, std::string op, std::string oper);
        virtual ~Instrucion();
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
    protected:

};

#endif // INSTRUCIONPARSER_H
