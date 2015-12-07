#include "file.h"
#include "base.h"
using namespace CustomStd;

namespace CustomStd
{
	struct TFile : TGfxBase
	{
		TFile()
			: m_tBaseNode(this)
		{

		}

		typedef TGfxUnrolled<unsigned char, 4096> TData;
		TData m_tUnrolled;
		TData::TIterator m_tCursor;

		FLIB_BASE(TFile, TGfxBase);
	};
}

bool CustomStd::GfxFileIsEOF(const TFile * pFile)
{
	GfxDbgAssert(pFile != 0);
	return (pFile->m_tCursor.GetIndex() == pFile->m_tUnrolled.GetItemCount());
}

void CustomStd::GfxFileClose(struct TFile * pFile)
{
	GfxDbgAssert(pFile != 0);
	delete pFile;
}

int CustomStd::GfxFileSize(const struct TFile * pFile)
{
	GfxDbgAssert(pFile != 0);
	return pFile->m_tUnrolled.GetItemCount();
}

unsigned char CustomStd::GfxFileReadChar(struct TFile * pFile)
{
	GfxDbgAssert(pFile != 0);
	GfxDbgAssert(!GfxFileIsEOF(pFile), "reading passed EOF");
	const unsigned char cByte = *pFile->m_tCursor;
	++pFile->m_tCursor;
	return cByte;
}

void CustomStd::GfxFileRead(struct TFile * pFile, void * pBuffer, int iByteSize)
{
	GfxDbgAssert(pFile != 0);
	GfxDbgAssert(pBuffer != 0);
	pFile->m_tCursor.Read(static_cast<unsigned char *>(pBuffer), iByteSize);
}

TFile *CustomStd::GfxFileCreate()
{
	TFile * pGfxFile = new TFile();
	pGfxFile->m_tCursor = TFile::TData::TIterator(pGfxFile->m_tUnrolled);
	return pGfxFile;
}

void CustomStd::GfxFileWriteChar(TFile * pFile, const unsigned char cValue)
{
	GfxDbgAssert(pFile != 0);
	*pFile->m_tCursor = cValue;
	++pFile->m_tCursor;
}

void CustomStd::GfxFileWriteString(TFile * pFile, const char * pString)
{
    GfxDbgAssert(pString != NULL);
    int iStringLength = strlen(pString);

    for (int iK = 0; iK < iStringLength; iK++)
        GfxFileWriteChar(pFile, pString[iK]);
}

int CustomStd::GfxFileTell(const TFile * pFile)
{
	GfxDbgAssert(pFile != 0);
	return pFile->m_tCursor.GetIndex();
}

void CustomStd::GfxFileSeek(TFile * pFile, int iPosition)
{
	GfxDbgAssert(pFile != 0);
	pFile->m_tCursor = TFile::TData::TIterator(pFile->m_tUnrolled);
	pFile->m_tCursor += iPosition;
}

TFile *CustomStd::GfxFileOpenRead(const char * pFilename)
{
	GfxDbgAssert(pFilename != 0);

	TFile * pGfxFile = new TFile;
	char pPath[1024];

	sprintf(pPath, "%s", pFilename);
	FILE *pFile = fopen(pPath, "r");

	if (pFile != 0)
	{
	    fseek(pFile, 0, SEEK_END);
	    int iByteCount = ftell(pFile);
	    fseek(pFile, 0, SEEK_SET);

	    while (iByteCount > 0)
	    {
	        const int iChunkByteCount = (iByteCount < TFile::TData::CHUNK_SIZE) ? iByteCount : TFile::TData::CHUNK_SIZE;
	        unsigned char *pBuffer = pGfxFile->m_tUnrolled.Append(iChunkByteCount);
	        const int iCountRead = fread(pBuffer, iChunkByteCount, 1, pFile);
            GfxDbgAssert(iCountRead == 1);
	        iByteCount -= iChunkByteCount;
	    }
	    fclose(pFile);
	    pGfxFile->m_tCursor = TFile::TData::TIterator(pGfxFile->m_tUnrolled);
	}

	return pGfxFile;
}

bool CustomStd::GfxFileSave(TFile * pGfxFile, const char * pFilename)
{
	GfxDbgAssert(pGfxFile != 0);
	GfxDbgAssert(pFilename != 0);

	FILE *pFile = fopen(pFilename, "w");

	if (pFile != 0)
	{
		int iByteCount = GfxFileSize(pGfxFile);
		TFile::TData::TIterator it(pGfxFile->m_tUnrolled);

		while (iByteCount > 0)
		{
			const int iChunkByteCount = (iByteCount < TFile::TData::CHUNK_SIZE) ? iByteCount : TFile::TData::CHUNK_SIZE;
			fputs((const char *)&*it, pFile);
			it += iChunkByteCount;
			iByteCount -= iChunkByteCount;
		}

		fclose(pFile);
		return true;
	}

    fclose(pFile);
	return false;
}
