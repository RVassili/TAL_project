#ifndef __BASEOBJECT_H_INCLUDED__
#define __BASEOBJECT_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace CustomStd
{
	class STDBaseObject
	{
	private:
		unsigned long long m_iId;

	public:
		STDBaseObject();
		~STDBaseObject();

		virtual bool operator==(STDBaseObject &tObject);
	};

	class STDString : public STDBaseObject
	{
		char *m_pChar;

	public:
		STDString(char *pChar = NULL);
		~STDString();

		char *CharValue()const;
	};

	class STDNumber : public STDBaseObject
	{
		double m_dValue;

	public:
		STDNumber();
		STDNumber(int);
		STDNumber(float);
		STDNumber(double);
		~STDNumber();

		int IntValue()const;
		float FloatValue()const;
		double DoudbleValue()const;

		void SetValue(int);
		void SetValue(float);
		void SetValue(double);
	};
}
#endif
