
#include "baseObject.h"

unsigned long long int unique_id()
{
	static unsigned long long int n = 0;
	return ++n;
}

namespace CustomStd
{
	STDBaseObject::STDBaseObject(){
		m_iId = unique_id();
	}
	STDBaseObject::~STDBaseObject(){

	}
	bool STDBaseObject::operator==(STDBaseObject &tObject){
		return this->m_iId == tObject.m_iId;
	}

	STDString::STDString(char *pChar){
		if (pChar == NULL)
			m_pChar = NULL;
		else
		{
			m_pChar = (char *)malloc(strlen(pChar) + 1);
			strncpy(m_pChar, pChar, strlen(pChar));
			m_pChar[strlen(pChar)] = 0;
		}
	}
	STDString::~STDString(){
		if (m_pChar != NULL)
			free(m_pChar);
	}
	char *STDString::CharValue()const{
		return m_pChar;
	}

	STDNumber::STDNumber(){
		m_dValue = (double)0;
	}
	STDNumber::STDNumber(int iValue){
		m_dValue = (double)iValue;
	}
	STDNumber::STDNumber(float fValue){
		m_dValue = (double)fValue;
	}
	STDNumber::STDNumber(double dValue){
		m_dValue = dValue;
	}
	STDNumber::~STDNumber(){

	}
	int STDNumber::IntValue()const{
		return (int)m_dValue;
	}
	float STDNumber::FloatValue()const{
		return (float)m_dValue;
	}
	double STDNumber::DoudbleValue()const{
		return (double)m_dValue;
	}
	void STDNumber::SetValue(int iValue){
		m_dValue = (double)iValue;
	}
	void STDNumber::SetValue(float fValue){
		m_dValue = (double)fValue;
	}
	void STDNumber::SetValue(double dValue){
		m_dValue = dValue;
	}
}
