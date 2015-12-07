#include <iostream>
#include "TAL_parsor.h"

using namespace std;
using namespace CustomStd;

int main()
{
    TAL_parsor *pPar = new TAL_parsor();
    pPar->Init("Mark", ETALWordType_NNP, 4);
    pPar->ParseFile("data/article/test_chk.txt");
    CustomStd::STDLinkedList *pContext = pPar->GetContext();

    for (STDLinkedList::STDIterator tIt = pContext->Begin(); tIt <= pContext->End(); tIt++){
        TWord *pWordFound = static_cast<TWord *>(*tIt);
        std::cout << "word : " << pWordFound->m_pChar << " * " << pWordFound->m_iNbChar << std::endl;
    }

    delete pPar;

    return 0;
}
