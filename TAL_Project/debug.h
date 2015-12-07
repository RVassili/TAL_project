#ifndef __DEBUG_H_INCLUDED__
#define __DEBUG_H_INCLUDED__

void GfxDbgAssert(bool bExpectedTrue, const char * pMessage = 0);
int GfxDbgPrintf(const char * pFormat, ...);

#endif
