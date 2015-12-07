#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include "debug.h"
#include <stdlib.h>

#ifdef FLIB_COVERAGE_FUNCTION

#include <string.h>

struct TGfxCoverageReport
{
	TGfxCoverageReport(const char * pPath, int iLine)
	{
		extern void FLIB_COVERAGE_FUNCTION(const char *, int);
		const char * pName = strrchr(pPath, '\\');
		FLIB_COVERAGE_FUNCTION(pName ? (pName + 1) : pPath, iLine);
	}
};

#define FLIB_COVERAGE \
		{ static TGfxCoverageReport tDummy( __FILE__, __LINE__ ); }

#else

#define FLIB_COVERAGE

#endif

struct TGfxBaseType
{
	static TGfxBaseType *& Head()
	{
		FLIB_COVERAGE
			static TGfxBaseType * tFirst;
		return tFirst;
	}

	TGfxBaseType()
		: m_pNext(0)
	{
		FLIB_COVERAGE
			TGfxBaseType ** pNode = &Head();

		while (*pNode != 0)
		{
			pNode = &(*pNode)->m_pNext;
		}

		*pNode = this;
	}

	TGfxBaseType * m_pNext;

	virtual const char * GetName() = 0;
	virtual void ForEachInstance(void(*pCallback) (struct TGfxBase *)) = 0;
};

#define FLIB_COPY_DEF( _NAME_ ) _NAME_( const _NAME_ & ); _NAME_ & operator = ( const _NAME_ & )
#define FLIB_BASE( _CLASS_, _PARENT_ ) \
	virtual const char * GetTypeNameDynamic() { return GetTypeNameStatic(); } \
	static const char * GetTypeNameStatic() { return #_CLASS_; } \
	TGfxBaseNode< _CLASS_ > m_tBaseNode

struct TGfxBase
{
	TGfxBase()
		: m_iRefCount(0)
		, m_bInternal(InternalOverride())
	{
		FLIB_COVERAGE
	}

	virtual ~TGfxBase()
	{
		FLIB_COVERAGE
			GfxDbgAssert(m_iRefCount == 0);
	}

	virtual const char * GetTypeNameDynamic() = 0;

	template< typename T > T * GetAs()
	{
		FLIB_COVERAGE
			if (this != 0 && T::GetTypeNameStatic() == GetTypeNameDynamic())
			{
				return static_cast< T * > (this);
			}

		return 0;
	}

	bool IsInternal() const
	{
		FLIB_COVERAGE
			return m_bInternal;
	}

	static int GenerateID()
	{
		FLIB_COVERAGE
			static int iNextID = 0;
		return iNextID++;
	}

	static bool & InternalOverride()
	{
		FLIB_COVERAGE
			static bool bInternalOverride = false;
		return bInternalOverride;
	}

private:

	FLIB_COPY_DEF(TGfxBase);

	int m_iRefCount;
	bool m_bInternal;
};

template< typename _TYPE_ >
struct TGfxBaseNode
{
	struct TGfxType : TGfxBaseType
	{
		TGfxType()
			: m_pFirstNode(0)
		{
			FLIB_COVERAGE
		}

		TGfxBaseNode * m_pFirstNode;

		virtual const char * GetName()
		{
			return NULL;
		};
		virtual void ForEachInstance(void(*pCallback) (struct TGfxBase *))
		{
			FLIB_COVERAGE
				for (TGfxBaseNode * pNode = TGfxBaseNode::Head(); pNode != 0; pNode = pNode->m_pNext)
				{
					if (pNode->m_pObject->GetTypeNameDynamic() == GetName())
					{
						(*pCallback)(pNode->m_pObject);
					}
				}
		}
	};

	static TGfxType & GetType()
	{
		FLIB_COVERAGE
			static TGfxType tType;
		return tType;
	}

	static TGfxBaseNode *& Head()
	{
		FLIB_COVERAGE
			return GetType().m_pFirstNode;
	}

	_TYPE_ * m_pObject;
	int m_iID;
	TGfxBaseNode * m_pNext;

	TGfxBaseNode(_TYPE_ * pObject)
		: m_pObject(pObject)
		, m_iID(TGfxBase::GenerateID())
		, m_pNext(0)
	{
		FLIB_COVERAGE
			TGfxBaseNode ** pNode = &Head();

		while (*pNode != 0)
		{
			pNode = &(*pNode)->m_pNext;
		}

		*pNode = this;
	}

	~TGfxBaseNode()
	{
		FLIB_COVERAGE
			TGfxBaseNode ** pNode = &Head();

		while (*pNode != this)
		{
			pNode = &(*pNode)->m_pNext;
		}

		*pNode = m_pNext;
	}
};



template<typename T, int _CHUNK_SIZE>
class TGfxUnrolled
{
private:

	struct TNode
	{
		TNode()
			: m_pNext(0)
		{
		}

		T m_pArray[_CHUNK_SIZE];
		TNode * m_pNext;
	};

public:

	static const int CHUNK_SIZE = _CHUNK_SIZE;

	class TIterator
	{
	public:

		TIterator()
			: m_pNode(0)
			, m_pUnrolled(0)
			, m_iIndex(0)
		{
		}

		TIterator(TGfxUnrolled & tUnrolled)
			: m_pNode(tUnrolled.m_pHead)
			, m_pUnrolled(&tUnrolled)
			, m_iIndex(0)
		{
		}

		T & operator*()
		{
			GfxDbgAssert(m_pUnrolled != 0);
			if (m_pUnrolled->GetItemCount() == m_iIndex)
			{
				if (!m_pNode)
				{
					m_pUnrolled->Append(1);
					m_pNode = m_pUnrolled->m_pTail;
				}
				++m_pUnrolled->m_iItemCount;
			}
			return m_pNode->m_pArray[m_iIndex % CHUNK_SIZE];
		}

		void Read(T * pBuffer, int iCount)
		{
		    if (iCount == 0)
                return;
			int iWriteOffset = 0;

			for (;;)
			{
				GfxDbgAssert(this->m_pNode != 0);
				const int iBaseIndex = m_iIndex % CHUNK_SIZE;
				const int iCurChunk = CHUNK_SIZE - iBaseIndex;
				if (iCurChunk >= iCount)
				{
					for (int i = 0; i < iCount; ++i)
					{
						pBuffer[iWriteOffset + i] = m_pNode->m_pArray[iBaseIndex + i];
					}
					m_iIndex += iCount;
					return;
				}
				else
				{
					for (int i = 0; i < iCurChunk; ++i)
					{
						pBuffer[iWriteOffset + i] = m_pNode->m_pArray[iBaseIndex + i];
					}
					iCount -= iCurChunk;
					m_iIndex += iCurChunk;
					iWriteOffset += iCurChunk;
					m_pNode = m_pNode->m_pNext;
				}
			}
		}

		TIterator & operator++()
		{
			GfxDbgAssert(m_pNode != 0);
			if (++m_iIndex % CHUNK_SIZE == 0)
			{
				m_pNode = m_pNode->m_pNext;
			}
			return *this;
		}

		TIterator & operator+=(int iCount)
		{
			if (iCount == 0)
			{
				return *this;
			}

			GfxDbgAssert(m_pNode != 0);
			GfxDbgAssert(iCount > 0);
			GfxDbgAssert(m_iIndex + iCount <= m_pUnrolled->GetItemCount());

			for (;;)
			{
				const int iBaseIndex = m_iIndex % CHUNK_SIZE;
				const int iCurChunk = CHUNK_SIZE - iBaseIndex;
				if (iCurChunk > iCount)
				{
					m_iIndex += iCount;
					break;
				}
				else
				{
					m_iIndex += iCurChunk;
					iCount -= iCurChunk;
					m_pNode = m_pNode->m_pNext;
				}
			}
			return *this;
		}

		int GetIndex() const
		{
			return m_iIndex;
		}

	private:
		TNode * m_pNode;
		TGfxUnrolled * m_pUnrolled;
		int m_iIndex;
	};

	TGfxUnrolled()
		: m_pHead(0)
		, m_pTail(0)
		, m_iItemCount(0)
	{
	}

	~TGfxUnrolled()
	{
		for (TNode * pNode = m_pHead; pNode != 0;)
		{
			TNode * pNext = pNode->m_pNext;
			delete pNode;
			pNode = pNext;
		}
	}

	int GetItemCount() const
	{
		return m_iItemCount;
	}

	T * Append(int iChunkSize)
	{
		GfxDbgAssert(iChunkSize <= CHUNK_SIZE);
		GfxDbgAssert(m_iItemCount % CHUNK_SIZE == 0);

		if (!m_pHead)
		{
			m_pHead = new TNode;
			m_pTail = m_pHead;
		}
		else
		{
			m_pTail->m_pNext = new TNode;
			m_pTail = m_pTail->m_pNext;
		}

		m_iItemCount += iChunkSize;
		return m_pTail->m_pArray;
	}

private:

	TGfxUnrolled(const TGfxUnrolled &);
	TGfxUnrolled & operator=(const TGfxUnrolled &);

	TNode * m_pHead;
	TNode * m_pTail;
	int m_iItemCount;
};

#endif
