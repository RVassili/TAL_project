#ifndef __FILE_DEFINITION_H__
#define __FILE_DEFINITION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace CustomStd
{
	struct TFile;

	bool GfxFileIsEOF(const TFile * pFile);

	void GfxFileClose(struct TFile * pFile);

	int GfxFileSize(const struct TFile * pFile);

	unsigned char GfxFileReadChar(struct TFile * pFile);

	void GfxFileRead(struct TFile * pFile, void * pBuffer, int iByteSize);

	TFile *GfxFileCreate();

	void GfxFileWriteChar(TFile * pFile, const unsigned char cValue);

	void GfxFileWriteString(TFile * pFile, const char * pString);

	int GfxFileTell(const TFile * pFile);

	void GfxFileSeek(TFile * pFile, int iPosition);

	TFile *GfxFileOpenRead(const char * pFilename);

	bool GfxFileSave(TFile * pGfxFile, const char * pFilename);

}

#endif
