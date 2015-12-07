#include "TAL_parsor.h"

ETALBaliseType GetTalBaliseType(char *pString){
    char *pSpace = strchr(pString, ' ');
    if (pSpace == NULL)
        return ETALBaliseType_Unknow;

    int iSize = pSpace - pString;
    char *pType = (char *)malloc(iSize);
    strncpy(pType, pString + 1, iSize);
    pType[iSize - 1] = 0;

    if (strcmp(pType, "NP") == 0)
        return ETALBaliseType_NP;
    else if (strcmp(pType, "VP") == 0)
        return ETALBaliseType_VP;
    else if (strcmp(pType, "PP") == 0)
        return ETALBaliseType_PP;

    return ETALBaliseType_Unknow;
}
ETALWordType GetWordType(char *pString){
    char *pUnderscore = strchr(pString, '_');
    if (pUnderscore == NULL)
        return ETALWordType_Unknow;

    if (strcmp(pUnderscore + 1, "NN") == 0)
        return ETALWordType_NN;
    else if (strcmp(pUnderscore + 1, "NNP") == 0)
        return ETALWordType_NNP;
    else if (strcmp(pUnderscore + 1, "NNS") == 0)
        return ETALWordType_NNS;
    else if (strcmp(pUnderscore + 1, "VB") == 0)
        return ETALWordType_VB;
    else if (strcmp(pUnderscore + 1, "VBD") == 0)
        return ETALWordType_VBD;
    else if (strcmp(pUnderscore + 1, "VBZ") == 0)
        return ETALWordType_VBZ;
    else if (strcmp(pUnderscore + 1, "VBP") == 0)
        return ETALWordType_VBP;
    else if (strcmp(pUnderscore + 1, "VBG") == 0)
        return ETALWordType_VBG;
    else if (strcmp(pUnderscore + 1, "MD") == 0)
        return ETALWordType_MD;
    else if (strcmp(pUnderscore + 1, "CD") == 0)
        return ETALWordType_CD;
    else if (strcmp(pUnderscore + 1, "IN") == 0)
        return ETALWordType_IN;
    else if (strcmp(pUnderscore + 1, "PRP") == 0)
        return ETALWordType_PRP;
    else if (strcmp(pUnderscore + 1, "DT") == 0)
        return ETALWordType_DT;
    else if (strcmp(pUnderscore + 1, "JJ") == 0)
        return ETALWordType_JJ;
    else if (strcmp(pUnderscore + 1, "PRP$") == 0)
        return ETALWordType_PRP$;
    else if (strcmp(pUnderscore + 1, "CC") == 0)
        return ETALWordType_CC;
    else if (strcmp(pUnderscore + 1, "WDT") == 0)
        return ETALWordType_WDT;

    return ETALWordType_Unknow;
}

TAL_parsor::TAL_parsor(){
    m_pFileContent = NULL;
    m_pCurrentPosition = NULL;
    m_pTargetWord = NULL;
    m_tTargetWordType = ETALWordType_Unknow;
    m_pContext = NULL;
    m_pVectorContext = NULL;
    m_iContextWindow = 0;
    m_pFoundWord = NULL;
}
TAL_parsor::~TAL_parsor(){
    if (m_pFileContent != NULL)
        free(m_pFileContent);
    if (m_pTargetWord != NULL)
        free(m_pTargetWord);

    if (m_pContext != NULL)
        delete m_pContext;
    if (m_pVectorContext != NULL)
        delete m_pVectorContext;
    if (m_pFoundWord != NULL)
        delete m_pFoundWord;
}

STDLinkedList  *TAL_parsor::GetContext()const{
    return m_pVectorContext;
}

void TAL_parsor::Init(const char *pTargetWord, ETALWordType tWordType, int iContextWindow){
    if (m_pTargetWord != NULL)
        free(m_pTargetWord);
    GfxDbgAssert(pTargetWord != NULL);
    m_pTargetWord = (char *)malloc(strlen(pTargetWord) + 1);
    strncpy(m_pTargetWord, pTargetWord, strlen(pTargetWord));
    m_pTargetWord[strlen(pTargetWord)] = 0;

    m_tTargetWordType = tWordType;

    m_iContextWindow = iContextWindow;

    if (m_pFoundWord != NULL)
        delete m_pFoundWord;
    m_pFoundWord = new vector<int>();

    if(m_pVectorContext != NULL)
        delete m_pVectorContext;
    m_pVectorContext = new STDLinkedList();
}

bool TAL_parsor::ParseFile(const char *pFileName){
    if (m_pFileContent != NULL)
        free(m_pFileContent);

    TFile * pFile = GfxFileOpenRead(pFileName);
    int iBufferSize = GfxFileSize(pFile) + 1;
    m_pFileContent = (char *)malloc(iBufferSize);
    GfxFileRead(pFile, m_pFileContent, GfxFileSize(pFile));
    m_pFileContent[iBufferSize - 1] = 0;
    GfxFileClose(pFile);
    m_pCurrentPosition = m_pFileContent;

    if (m_pContext != NULL)
        delete m_pContext;
    m_pContext = new STDLinkedList();

    return this->Parse();
}
bool TAL_parsor::Parse(){
    if (m_pCurrentPosition == NULL)
        return true;

    TStructReturnBalise tBalise = FindBalise(m_pCurrentPosition, '[', ']');
    if (tBalise.pBalise == NULL){
        this->FoundString(m_pCurrentPosition);
        return true;
    }

    unsigned int iSizeParcours = 0;
    if (tBalise.pEndBalise != NULL)
        iSizeParcours = tBalise.pEndBalise - m_pCurrentPosition;
    else
        iSizeParcours = strlen(m_pCurrentPosition);

    if (iSizeParcours >= strlen(tBalise.pBalise)){
        int iStringLen = iSizeParcours + 1 - strlen(tBalise.pBalise);
        char *pString = (char *)malloc(iStringLen);
        strncpy(pString, m_pCurrentPosition, iStringLen - 1);
        pString[iStringLen - 1] = 0;
        char *pString2 = RemoveExternalSpace(pString);
        if (pString2 != NULL){
            this->FoundString(pString2);
            free(pString2);
        }
        free(pString);
    }

    ETALBaliseType tType = GetTalBaliseType(tBalise.pBalise);
    switch (tType){
    case ETALBaliseType_NP:{
        char *pType = strchr(tBalise.pBalise, ' '),
            *pBalise;
        pBalise = (char *)malloc(strlen(pType) - 1);
        strncpy(pBalise, pType + 1, strlen(pType) - 2);
        pBalise[strlen(pType) - 2] = 0;
        this->FoundBalise(pBalise, tType);
        free(pBalise);
    }
        break;
    case ETALBaliseType_VP:{
        char *pType = strchr(tBalise.pBalise, ' '),
            *pBalise;
        pBalise = (char *)malloc(strlen(pType) - 4);
        strncpy(pBalise, pType + 1, strlen(pType) - 5);
        pBalise[strlen(pType) - 5] = 0;
        this->FoundBalise(pBalise, tType);
        free(pBalise);
    }
        break;
    case ETALBaliseType_PP:{
        char *pType = strchr(tBalise.pBalise, ' '),
            *pBalise;
        pBalise = (char *)malloc(strlen(pType) - 4);
        strncpy(pBalise, pType + 1, strlen(pType) - 5);
        pBalise[strlen(pType) - 5] = 0;
        this->FoundBalise(pBalise, tType);
        free(pBalise);
    }
        break;
    default:{
        std::cout << "found unknow balise : " << tBalise.pBalise << std::endl;
    }
        break;
    }

    free(tBalise.pBalise);
    if (tBalise.pEndBalise != NULL)
        GfxDbgAssert(strcmp(m_pCurrentPosition, tBalise.pEndBalise) != 0, "Error, xmlReader stop parsed");
    m_pCurrentPosition = tBalise.pEndBalise;

    return this->Parse();
}

void TAL_parsor::FoundString(char *){
}
void TAL_parsor::FoundBalise(char *pString, ETALBaliseType tBaliseType){
    pString = RemoveExternalSpace(pString);
    char *pSpace = strchr(pString, ' ');

    if (pSpace != NULL){
        int iStringSize = pSpace - pString;
        char *pString2 = (char *)malloc(iStringSize + 1);
        strncpy(pString2, pString, iStringSize);
        pString2[iStringSize] = 0;

        ETALWordType tWordType = GetWordType(pString2);
        int iWordSize = strchr(pString2, '_') - pString2;
        char *pWord = (char *)malloc(iWordSize + 1);
        strncpy(pWord, pString2, iWordSize);
        pWord[iWordSize] = 0;
        this->FoundWord(pWord, tWordType);
        free(pWord);

        std::cout << "     ";
        return this->FoundBalise(pSpace + 1, tBaliseType);
    }

    ETALWordType tWordType = GetWordType(pString);
    int iWordSize = strchr(pString, '_') - pString;
    char *pWord = (char *)malloc(iWordSize + 1);
    strncpy(pWord, pString, iWordSize);
    pWord[iWordSize] = 0;
    this->FoundWord(pWord, tWordType);
    free(pWord);
    std::cout << std::endl;
}
void TAL_parsor::FoundWord(char *pWord, ETALWordType tType){
    if (strncmp(m_pTargetWord, pWord, strlen(m_pTargetWord)) == 0 && m_tTargetWordType == tType){
        this->FoundTargetWord();
        this->AddWordInContext(pWord);
        this->NewWordForContext();
        return;
    }

    switch (tType)
    {
        default:
        {
            this->AddWordInContext(pWord);
        }
        break;
    }
}

void TAL_parsor::FoundTargetWord(){
    for (STDLinkedList::STDIterator tIt = m_pContext->Begin(); tIt <= m_pContext->End(); tIt++)
    {
        STDString *pString = static_cast<STDString *>(*tIt);
        char *pWord = pString->CharValue();
        this->FoundTargetWord(pWord);
    }
}
void TAL_parsor::FoundTargetWord(char *pWord){
    bool bFoundWord = false;
    for (STDLinkedList::STDIterator tIt = m_pVectorContext->Begin(); tIt <= m_pVectorContext->End(); tIt++){
        TWord *pWordFound = static_cast<TWord *>(*tIt);
        if (strcmp (pWordFound->m_pChar, pWord) == 0){
            pWordFound->m_iNbChar++;
            bFoundWord = true;
        }
    }

    if (!bFoundWord){
        TWord *pNewWord = new TWord();
        char *pWordStorage = (char *)malloc(strlen(pWord) + 1);
        strncpy(pWordStorage, pWord, strlen(pWord));
        pWordStorage[strlen(pWord)] = 0;
        pNewWord->m_pChar = pWordStorage;
        pNewWord->m_iNbChar = 1;
        m_pVectorContext->Add(pNewWord);
    }
}

void TAL_parsor::AddWordInContext(char *pWord){
    for (vector<int>::iterator tIt = m_pFoundWord->begin(); tIt != m_pFoundWord->end(); ++tIt){
        if (*tIt > 0){
            (*tIt)--;
            this->FoundTargetWord(pWord);
        }
    }

    m_pContext->Add(new STDString(pWord));
    if (m_pContext->GetSize() > m_iContextWindow)
        m_pContext->RemoveHead();
}
void TAL_parsor::NewWordForContext(){
    for (vector<int>::iterator tIt = m_pFoundWord->begin(); tIt != m_pFoundWord->end(); ++tIt){
        if (*tIt == 0){
            *tIt = m_iContextWindow;
            return;
        }
    }

    m_pFoundWord->push_back(m_iContextWindow);
}

