#ifndef TOPTIONPARSOR_H_INCLUDED
#define TOPTIONPARSOR_H_INCLUDED

#include <iostream>
#include "parsor.h"
using namespace CustomStd;
using namespace std;

class TOptionParsor : public CustomStd::STDParsor
{
private:
    void FoundOpeningBalise(char *sBaliseName, STDDictionary *pAttributDict);
    void FoundString(char *sString);
    void FoundClosingBalise(char *sBaliseName);

public:
    TOptionParsor();
    ~TOptionParsor();
};

#endif // TOPTIONPARSOR_H_INCLUDED
