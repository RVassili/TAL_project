#ifndef TAL_PARSOR_H_INCLUDED
#define TAL_PARSOR_H_INCLUDED

#include "customString.h"           //string management functions
#include "file.h"                   //file class
#include "linkedList.h"             //list class
#include <vector>                   //should find better way
#include <iostream>                 //must be removed
using namespace CustomStd;
using namespace std;

enum ETALBaliseType{
    ETALBaliseType_Unknow,
    ETALBaliseType_NP,
    ETALBaliseType_VP,
    ETALBaliseType_PP
};
enum ETALWordType{
    ETALWordType_Unknow,
    //
    ETALWordType_NN,
    ETALWordType_NNP,
    ETALWordType_NNS,
    //
    ETALWordType_VB,
    ETALWordType_VBD,
    ETALWordType_VBZ,
    ETALWordType_VBP,
    ETALWordType_VBG,
    //
    ETALWordType_MD,
    ETALWordType_CD,
    ETALWordType_IN,
    ETALWordType_PRP,
    ETALWordType_DT,
    ETALWordType_JJ,
    ETALWordType_PRP$,
    ETALWordType_CC,
    ETALWordType_WDT
};

class TWord : public STDBaseObject{
public:
    char *m_pChar;
    int m_iNbChar;
};

//return balise type for argument
ETALBaliseType GetTalBaliseType(char *);
//return word type for argument
ETALWordType GetWordType(char *);

class TAL_parsor
{
private:
    char *m_pFileContent,               //current file parsed content - alloc
         *m_pCurrentPosition,           //current position in file parsed -no alloc
         *m_pTargetWord;                //target word for parsing - alloc
    ETALWordType m_tTargetWordType;     //target word type
    STDLinkedList *m_pContext,          //last words parssed
        *m_pVectorContext;              //context for target word
    int m_iContextWindow;               //context window size
    vector<int> *m_pFoundWord;

    bool Parse();

    void FoundString(char *);
    void FoundBalise(char *, ETALBaliseType);
    void FoundWord(char *, ETALWordType);

    void FoundTargetWord();
    void FoundTargetWord(char *);

    void AddWordInContext(char *);
    void NewWordForContext();

public:
    TAL_parsor();
    ~TAL_parsor();

    //init parsor with target word specification and size for context window
    void Init(const char *pTargetWord, ETALWordType tWordType, int iContextWindow);

    //parse file - does not reset context already calculated
    bool ParseFile(const char *pFileName);

    //get result - call this after parse to get result
    STDLinkedList  *GetContext()const;
};

#endif
