#ifndef OBJECTCODEMAP_H
#define OBJECTCODEMAP_H
#include <unordered_map>
#include "Ob.h"

class objectCodeMap
{
    public:
        std::unordered_map<std::string, Ob*> ObjectCodes;
        objectCodeMap();
        int roo;
        std::unordered_map<std::string, Ob*> getObjectCodes();
        void setTheMap();
        int getTheCode(std::string ins);
        int getTheFormat(std::string ins);
        int getet ();

};

#endif // OBJECTCODEMAP_H
