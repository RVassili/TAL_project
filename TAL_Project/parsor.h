#ifndef PARSOR_H_INCLUDED
#define PARSOR_H_INCLUDED

#include "dictionary.h"
#include "baseObject.h"

namespace CustomStd
{
	class STDParsor : public STDBaseObject
	{
	private:
		char *m_pFileContent,
			*m_pCurrentReadingPosition;

		virtual void FoundOpeningBalise(char *sBaliseName, STDDictionary *pAttributDict) = 0;
		virtual void FoundString(char *sString) = 0;
		virtual void FoundClosingBalise(char *sBaliseName) = 0;

	public:
		STDParsor();
		~STDParsor();

		void InitWithFile(const char *pFileName);

		bool ParseFast();
		bool ParseSlow();
	};
}

#endif
