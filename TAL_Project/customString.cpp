#include "customString.h"

/*
Balise location
----------------------------------
*/
//	todo : remove comentary from xml data
char *FindCharExternString(char *pString, char cChar){
	if (pString == NULL)
		return NULL;

	char *pBalise = strchr(pString, cChar),
		*pChar = strchr(pString, '\''),
		*pChars = strchr(pString, '"');

	if (pBalise == NULL)
		return NULL;

	if (pChar != NULL && pChars != NULL){
		if (pBalise - pString > pChar - pString && pBalise - pString > pChars - pString)
			return pBalise;
		else if (pString - pChar < pString - pChars){
			GfxDbgAssert(strlen(pChars) > 1, "unexpected character '\"'");
			char *pChars2 = strchr(pChars + 1, '"');
			GfxDbgAssert((pChars2 != NULL), "unexpected character '\"'");
			if (strlen(pChars2) == 1)
				return NULL;
			return FindCharExternString(pChars2 + 1, cChar);
		}
		else{
			GfxDbgAssert(strlen(pChar) > 1, "unexpected character '\"'");
			char *pChar2 = strchr(pChar + 1, '"');
			GfxDbgAssert((pChar2 != NULL), "unexpected character '\"'");
			if (strlen(pChar2) == 1)
				return NULL;
			return FindCharExternString(pChar2 + 1, cChar);
		}
	}
	else if (pChar != NULL){
		if (pBalise - pString > pChars - pString)
			return pBalise;
		GfxDbgAssert(strlen(pChar) > 1, "unexpected character '\"'");
		char *pChar2 = strchr(pChar + 1, '"');
		GfxDbgAssert((pChar2 != NULL), "unexpected character '\"'");
		if (strlen(pChar2) == 1)
			return NULL;
		return FindCharExternString(pChar2 + 1, cChar);
	}
	else if (pChars != NULL){
		if (pBalise - pString > pChar - pString)
			return pBalise;
		GfxDbgAssert(strlen(pChars) > 1, "unexpected character '\"'");
		char *pChars2 = strchr(pChars + 1, '"');
		GfxDbgAssert((pChars2 != NULL), "unexpected character '\"'");
		if (strlen(pChars2) == 1)
			return NULL;
		return FindCharExternString(pChars2 + 1, cChar);
	}
	else
		return pBalise;
}
//	return pointer inside the struct, need free
TStructReturnBalise FindBalise(char * pString, char cStartElement, char cEndElement){
	char *pOpen = FindCharExternString(pString, cStartElement);
	TStructReturnBalise tReturn;
	if (pOpen == NULL){
		tReturn.pBalise = NULL;
		tReturn.pEndBalise = NULL;
		return tReturn;
	}

	char *pClose = FindCharExternString(pOpen, cEndElement);
	GfxDbgAssert((pClose != NULL), "missing character '>'");

	int iSize = pClose - pOpen + 1;

	char * cBalise = (char *)malloc(iSize + 1);
	strncpy(cBalise, pOpen, iSize);
	cBalise[iSize] = 0;

	tReturn.pBalise = cBalise;
	if (strlen(pClose) > 1)
		tReturn.pEndBalise = pClose + 1;
	else
		tReturn.pEndBalise = NULL;
	return tReturn;
}

/*
Balise traitment
----------------------------------
*/
ETypeBalise GetBaliseType(char * pBalise){
	int iLength = strlen(pBalise);

	if (iLength < 2)
		return ETypeBalise_Unknow;

	if (iLength > 4){
		if (strncmp(pBalise, "<?", 2) == 0 && strncmp(pBalise + iLength - 2, "?>", 2) == 0)
			return ETypeBalise_DeclarationBalise;
	}
	if (iLength > 3){
		if (strncmp(pBalise, "<", 1) == 0 && strncmp(pBalise + iLength - 2, "/>", 2) == 0)
			return ETypeBalise_AutoFermante;
		else if (strncmp(pBalise, "</", 2) == 0 && strncmp(pBalise + iLength - 1, ">", 1) == 0)
			return ETypeBalise_Fermante;
	}
	if (strncmp(pBalise, "<", 1) == 0 && strncmp(pBalise + iLength - 1, ">", 1) == 0)
		return ETypeBalise_Ouvrante;

	return ETypeBalise_Unknow;
}
//	if not NULL need free
char *GetName(char * pBalise, ETypeBalise eBaliseType){
	char * cSpace = strchr(pBalise, ' ');
	if (cSpace == NULL){
		if (eBaliseType == ETypeBalise_Ouvrante){
			int iSize = strlen(pBalise);
			char * cResult = (char *)malloc(iSize - 1);
			strncpy(cResult, pBalise + 1, iSize - 2);
			cResult[iSize - 2] = 0;
			return cResult;
		}
		else if (eBaliseType == ETypeBalise_Fermante){
			int iSize = strlen(pBalise);
			char * cResult = (char *)malloc(iSize - 2);
			strncpy(cResult, pBalise + 2, iSize - 3);
			cResult[iSize - 3] = 0;
			return cResult;
		}
		else if (eBaliseType == ETypeBalise_AutoFermante){
			int iSize = strlen(pBalise);
			char * cResult = (char *)malloc(iSize - 2);
			strncpy(cResult, pBalise + 1, iSize - 3);
			cResult[iSize - 3] = 0;
			return cResult;
		}
	}
	int iPos = cSpace - pBalise;
	if (eBaliseType == ETypeBalise_Ouvrante){
		char * cResult = (char *)malloc(iPos);
		strncpy(cResult, pBalise + 1, iPos - 1);
		cResult[iPos - 1] = 0;
		return cResult;
	}
	else if (eBaliseType == ETypeBalise_Fermante){
		char * cResult = (char *)malloc(iPos - 1);
		strncpy(cResult, pBalise + 2, iPos - 2);
		cResult[iPos - 2] = 0;
		return cResult;
	}
	else if (eBaliseType == ETypeBalise_AutoFermante){
		char * cResult = (char *)malloc(iPos);
		strncpy(cResult, pBalise + 1, iPos - 1);
		cResult[iPos - 1] = 0;
		return cResult;
	}

	return NULL;
}

/*
Attributs traitment
----------------------------------
*/
// need free
char *GetAttributName(char *pString){
	char *pEqual = strchr(pString, '=');
	GfxDbgAssert(pEqual != NULL, "error, missing '=' for parameter");
	int iNameSize = pEqual - pString + 1;
	GfxDbgAssert(iNameSize > 0, "missing attribut name");
	char * pName = (char *)malloc(iNameSize);
	strncpy(pName, pString, iNameSize - 1);
	pName[iNameSize - 1] = 0;
	return pName;
}
//	need free
//	can be rework
char *GetAttributValue(char *pString){
	char *pEqual = strchr(pString, '=');
	GfxDbgAssert(pEqual != NULL, "error, missing '=' for parameter");
	GfxDbgAssert(strlen(pEqual) > 1, "error missing parameter after '='");
	char * pValue = (char *)malloc(strlen(pEqual));
	strncpy(pValue, pEqual + 1, strlen(pEqual) - 1);
	pValue[strlen(pEqual) - 1] = 0;
	char *pValue2 = RemoveExternalSpace(pValue);
	GfxDbgAssert(strlen(pValue2) >= 2, "error, missing attribut value");
	free(pValue);
	pValue = (char *)malloc(strlen(pValue2) - 1);
	strncpy(pValue, pValue2 + 1, strlen(pValue2) - 2);
	pValue[strlen(pValue2) - 2] = 0;
	free(pValue2);
	return pValue;
}
STDDictionary *GetAttributs(char *pBalise){
	char *pSpace = strchr(pBalise, ' ');
	STDDictionary *pAttributs = new STDDictionary();
	ETypeBalise eType = GetBaliseType(pBalise);

	if (pSpace != NULL)
		pSpace = pSpace + 1;

	while (pSpace != NULL){
		if (strlen(pSpace) <= 1)
			return pAttributs;

		char *pSpace2 = strchr(pSpace + 1, ' ');
		if (pSpace2 == NULL){
			switch (eType){
			case ETypeBalise_AutoFermante:{
				int iSizeAttribut = strlen(pSpace) - 1;
				char *pAttribut = (char *)malloc(iSizeAttribut);
				strncpy(pAttribut, pSpace, iSizeAttribut - 1);
				pAttribut[iSizeAttribut - 1] = 0;
				char *pName = GetAttributName(pAttribut),
					*pValue = GetAttributValue(pAttribut);
				STDString *pCharValue = new STDString(pValue);
				pAttributs->AddElementForKey(pCharValue, pName);
				free(pAttribut);
				free(pName);
				break;
			}
			case ETypeBalise_Ouvrante:{
				int iSizeAttribut = strlen(pSpace);
				char *pAttribut = (char *)malloc(iSizeAttribut);
				strncpy(pAttribut, pSpace, iSizeAttribut - 1);
				pAttribut[iSizeAttribut - 1] = 0;
				char *pName = GetAttributName(pAttribut),
					*pValue = GetAttributValue(pAttribut);
				STDString *pCharValue = new STDString(pValue);
				pAttributs->AddElementForKey(pCharValue, pName);
				free(pAttribut);
				free(pName);
				break;
			}
			default:
				break;
			}
			pSpace = pSpace2;
		}
		else{
			int iSizeAttribut = pSpace2 - pSpace + 1;
			char *pAttribut = (char *)malloc(iSizeAttribut);
			strncpy(pAttribut, pSpace, iSizeAttribut - 1);
			pAttribut[iSizeAttribut - 1] = 0;
			char *pName = GetAttributName(pAttribut),
				*pValue = GetAttributValue(pAttribut);
			STDString *pCharValue = new STDString(pValue);
			pAttributs->AddElementForKey(pCharValue, pName);
			free(pAttribut);
			free(pName);
			if (strlen(pSpace2) <= 1)
				pSpace = NULL;
			else
				pSpace = pSpace2 + 1;
		}
	}
	return pAttributs;
}

/*
String management
----------------------------------
*/
char * RemoveFirstSpace(char *cString){
	if (cString == NULL)
		return NULL;

	int iLen = strlen(cString);

	if (iLen == 0)
		return NULL;

	if (isspace(cString[0])){
		if (iLen == 1){
			free(cString);
			return NULL;
		}

		char *pString = (char *)malloc(iLen);
		strncpy(pString, cString + 1, iLen - 1);
		pString[iLen - 1] = 0;
		free(cString);
		return RemoveFirstSpace(pString);
	}

	return cString;
}
char * RemoveLastSpace(char * cString){
	if (cString == NULL)
		return NULL;

	int iLen = strlen(cString);

	if (iLen == 0)
		return NULL;

	if (isspace(cString[iLen - 1])){
		if (iLen == 1){
			free(cString);
			return NULL;
		}

		char *pString = (char *)malloc(iLen);
		strncpy(pString, cString, iLen - 1);
		pString[iLen - 1] = 0;
		free(cString);
		return RemoveLastSpace(pString);
	}

	return cString;
}
char * RemoveExternalSpace(char * cString){
	if (cString == NULL)
		return NULL;
	if (strlen(cString) == 0)
		return NULL;

	char * pResult = (char *)malloc(strlen(cString) + 1);
	strncpy(pResult, cString, strlen(cString));
	pResult[strlen(cString)] = 0;

	pResult = RemoveFirstSpace(pResult);
	if (pResult == NULL)
		return NULL;

	pResult = RemoveLastSpace(pResult);
	if (pResult == NULL)
		return NULL;

	return pResult;
}
