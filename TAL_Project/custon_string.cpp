#include "custom_string.h"
/*
char *FindCharExternString(char *pString, char cChar)
{
    if (pString == NULL)
        return NULL;

    char *pBalise = strchr(pString, cChar),
        *pChar = strchr(pString, '\''),
        *pChars = strchr(pString, '"');

    if (pBalise == NULL)
        return NULL;

    if (pChar != NULL && pChars != NULL)
    {
        if (pBalise - pString > pChar - pString && pBalise - pString > pChars - pString)
            return pBalise;
        else if (pString - pChar < pString - pChars)
        {
            //assert missing \'
            char *pChars2 = strchr(pChar + 1, '\'');
            //assert missing \'
            if (strlen(pChars2) == 1)
                return NULL;
            return FindCharExternString(pChars2 + 1, cChar);
        }
        else
        {
            //assert missing \"
            char *pChar2 = strchr(pChars + 1, '"');
            //assert missing \"
            if (strlen(pChar2) == 1)
                return NULL;
            return FindCharExternString(pChar2 + 1, cChar);
        }
    }
    else if (pChar != NULL)
    {
        if (pBalise - pString > pChar - pString)
            return pBalise;
        //assert missing \'
        char *pChar2 = strchr(pChar + 1, '"');
        //assert missing \'
        if (strlen(pChar2) == 1)
            return NULL;
        return FindCharExternString(pChar2 + 1, cChar);
    }
    else if (pChars != NULL)
    {
        if (pBalise - pString > pChars - pString)
            return pBalise;
        //assert missing \"
        char *pChar2 = strchr(pChars + 1, '"');
        //assert missing \"
        if (strlen(pChar2) == 1)
            return NULL;
        return FindCharExternString(pChar2 + 1, cChar);

    }

    return pBalise;
}
TStructReturnBalise FindBalise(char *pFile, char cDelimiterOpen, char cDelimiterClose)
{
    char *pOpen = FindCharExternString(pFile, cDelimiterOpen);
    TStructReturnBalise tReturn;
    if (pOpen == NULL)
        return tReturn;

    char *pClose = FindCharExternString(pOpen, cDelimiterClose);
    if (pClose == NULL)
    //error missing closing balise character
        return tReturn;

        int iSize = pClose - pOpen + 1;
        char *pBalise = (char *)malloc(iSize + 1);
        strncpy(pBalise, pOpen, iSize);
        pBalise[iSize] = 0;

        tReturn.pStartBalise = pBalise;
        if (strlen(pClose) > 1)
            tReturn.pEndBalise = pClose + 1;
        return tReturn;
}

*/
