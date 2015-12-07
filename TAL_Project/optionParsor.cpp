#include "optionParsor.h"

TOptionParsor::TOptionParsor()
{

}
TOptionParsor::~TOptionParsor()
{

}

void TOptionParsor::FoundOpeningBalise(char *sBaliseName, STDDictionary *)
{
    cout << sBaliseName << endl;
}
void TOptionParsor::FoundString(char *sString)
{
    cout << sString << endl;
}
void TOptionParsor::FoundClosingBalise(char *sBaliseName)
{
    cout << sBaliseName << endl;
}
