#include "parsor.h"
#include "file.h"
#include "customString.h"

namespace CustomStd
{
	STDParsor::STDParsor(){
		m_pFileContent = NULL;
		m_pCurrentReadingPosition = NULL;
	}
	STDParsor::~STDParsor(){
		if (m_pFileContent != NULL)
			free(m_pFileContent);
	}

	void STDParsor::InitWithFile(const char *pFileName){
		if (m_pFileContent != NULL)
			free(m_pFileContent);

		TFile * pFile = GfxFileOpenRead(pFileName);
		int iBufferSize = GfxFileSize(pFile) + 1;
		m_pFileContent = (char *)malloc(iBufferSize);
		GfxFileRead(pFile, m_pFileContent, GfxFileSize(pFile));
		m_pFileContent[iBufferSize - 1] = 0;
		GfxFileClose(pFile);
		m_pCurrentReadingPosition = m_pFileContent;
	}

	bool STDParsor::ParseFast(){
		if (m_pCurrentReadingPosition == NULL)
			return true;

		TStructReturnBalise tBalise = FindBalise(m_pCurrentReadingPosition, '<', '>');
		GfxDbgAssert(tBalise.pBalise != NULL, "error, NULL balise in remaining content");

		unsigned int iSizeParcours = 0;
		if (tBalise.pEndBalise != NULL)
			iSizeParcours = tBalise.pEndBalise - m_pCurrentReadingPosition;
		else
			iSizeParcours = strlen(m_pCurrentReadingPosition);

		if (iSizeParcours >= strlen(tBalise.pBalise)){
			int iStringLen = iSizeParcours + 1 - strlen(tBalise.pBalise);
			char *pString = (char *)malloc(iStringLen);
			strncpy(pString, m_pCurrentReadingPosition, iStringLen - 1);
			pString[iStringLen - 1] = 0;
			char *pString2 = RemoveExternalSpace(pString);
			if (pString2 != NULL){
				this->FoundString(pString2);
				free(pString2);
			}
			free(pString);
		}

		ETypeBalise eType = GetBaliseType(tBalise.pBalise);
		switch (eType){
		case ETypeBalise_AutoFermante:{
			char *pName = GetName(tBalise.pBalise, eType);
			STDDictionary *pAttr = GetAttributs(tBalise.pBalise);
			this->FoundOpeningBalise(pName, pAttr);
			this->FoundClosingBalise(pName);
			free(pName);
			delete pAttr;
			break;
		}
		case ETypeBalise_Ouvrante:{
			char  *pName = GetName(tBalise.pBalise, eType);
			STDDictionary *pAttr = GetAttributs(tBalise.pBalise);
			this->FoundOpeningBalise(pName, pAttr);
			free(pName);
			delete pAttr;
			break;
		}
		case ETypeBalise_Fermante:{
			char *pName = GetName(tBalise.pBalise, eType);
			this->FoundClosingBalise(pName);
			free(pName);
			break;
		}
		default:
			break;
		}
		free(tBalise.pBalise);
		if (tBalise.pEndBalise != NULL)
			GfxDbgAssert(strcmp(m_pCurrentReadingPosition, tBalise.pEndBalise) != 0, "Error, xmlReader stop parsed");
		m_pCurrentReadingPosition = tBalise.pEndBalise;

		return this->ParseFast();
	}
	bool STDParsor::ParseSlow(){
		if (m_pCurrentReadingPosition == NULL)
			return true;

		TStructReturnBalise tBalise = FindBalise(m_pCurrentReadingPosition, '<', '>');
		GfxDbgAssert(tBalise.pBalise != NULL, "error, NULL balise in remaining content");

		unsigned int iSizeParcours = 0;
		if (tBalise.pEndBalise != NULL)
			iSizeParcours = tBalise.pEndBalise - m_pCurrentReadingPosition;
		else
			iSizeParcours = strlen(m_pCurrentReadingPosition);

		if (iSizeParcours >= strlen(tBalise.pBalise)){
			int iStringLen = iSizeParcours + 1 - strlen(tBalise.pBalise);
			char *pString = (char *)malloc(iStringLen);
			strncpy(pString, m_pCurrentReadingPosition, iStringLen - 1);
			pString[iStringLen - 1] = 0;
			char *pString2 = RemoveExternalSpace(pString);
			if (pString2 != NULL){
				this->FoundString(pString2);
				free(pString2);
			}
			free(pString);
		}

		ETypeBalise eType = GetBaliseType(tBalise.pBalise);
		switch (eType){
		case ETypeBalise_AutoFermante:{
			char *pName = GetName(tBalise.pBalise, eType);
			STDDictionary *pAttr = GetAttributs(tBalise.pBalise);
			this->FoundOpeningBalise(pName, pAttr);
			this->FoundClosingBalise(pName);
			free(pName);
			delete pAttr;
			break;
		}
		case ETypeBalise_Ouvrante:{
			char *pName = GetName(tBalise.pBalise, eType);
			STDDictionary *pAttr = GetAttributs(tBalise.pBalise);
			this->FoundOpeningBalise(pName, pAttr);
			free(pName);
			delete pAttr;
			break;
		}
		case ETypeBalise_Fermante:{
			char *pName = GetName(tBalise.pBalise, eType);
			this->FoundClosingBalise(pName);
			free(pName);
			break;
		}
		default:
			break;
		}
		free(tBalise.pBalise);
		if (tBalise.pEndBalise != NULL)
			GfxDbgAssert(strcmp(m_pCurrentReadingPosition, tBalise.pEndBalise) != 0, "Error, xmlReader stop parsed");
		m_pCurrentReadingPosition = tBalise.pEndBalise;

		return false;
	}
}
