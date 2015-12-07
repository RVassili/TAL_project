#include <ctype.h>
#include "dictionary.h"
#include "debug.h"
using namespace CustomStd;

enum ETypeBalise{
	ETypeBalise_Ouvrante,
	ETypeBalise_Fermante,
	ETypeBalise_AutoFermante,
	ETypeBalise_DeclarationBalise,
	ETypeBalise_Unknow
};
struct TStructReturnBalise{
	char *pBalise,
		*pEndBalise;

	TStructReturnBalise()
		: pBalise(NULL)
		, pEndBalise(NULL)
	{

	}
};

/*
Balise location
----------------------------------
*/
TStructReturnBalise FindBalise(char * pString, char cStartElement, char cEndElement);

/*
Balise traitment
----------------------------------
*/
ETypeBalise GetBaliseType(char * pBalise);
char *GetName(char * pBalise, ETypeBalise eBaliseType);

/*
Attributs traitment
----------------------------------
*/
STDDictionary *GetAttributs(char *pBalise);

/*
String management
----------------------------------
*/
char * RemoveExternalSpace(char *);
