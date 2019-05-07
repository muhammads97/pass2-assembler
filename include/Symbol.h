#ifndef SYMBOL_H
#define SYMBOL_H


class Symbol
{
    public:
        int address = 0;
        bool relocatable = true;
        Symbol();
        virtual ~Symbol();

    protected:

    private:

};

#endif // SYMBOL_H
