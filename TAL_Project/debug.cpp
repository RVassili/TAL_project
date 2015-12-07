#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <cstring>
//#include <ctype.h>
//#include <malloc.h>
#include "debug.h"

void GfxDbgMessageBox( const char *pMessage );

struct TGfxDbgStrBuffer
{
	TGfxDbgStrBuffer() { string[ 0 ] = '\0'; }
	operator const char * () const { return string; }
	char string[ 1024 ];
};
struct TGfxDbgStrStream
{
	TGfxDbgStrBuffer buffer;

	void Append( int value )
	{
		sprintf( buffer.string, "%s%d", buffer.string, value );
	}
	void Append( const char * value )
	{
		sprintf( buffer.string, "%s%s", buffer.string, value );
	}
	TGfxDbgStrBuffer GetBuffer()
	{
		return buffer;
	}
	static TGfxDbgStrStream & GetInstance()
	{
		static TGfxDbgStrStream str_stream;
		str_stream.buffer = TGfxDbgStrBuffer();
		return str_stream;
	}
};

template<typename T> TGfxDbgStrStream & operator,(TGfxDbgStrStream & stream, const T & value)
{
	stream.Append( value );
	return stream;
}

#define GfxDbgStrFormat( ... ) ( TGfxDbgStrStream::GetInstance(), __VA_ARGS__ ).GetBuffer()

void GfxDbgMsgHelper( const char * prefix, const char * text, const char * expr, const char * file, int line )
{
	TGfxDbgStrBuffer message;

	if( text != 0 && text[ 0 ] != 0 )
	{
		message = GfxDbgStrFormat( prefix, text, "\nFile : ", file, "\nLine : ", line );
	}
	else
	{
		message = GfxDbgStrFormat( prefix, expr, "\nFile : ", file, "\nLine : ", line  );
	}

	GfxDbgPrintf( "%s\n", message.string );
	GfxDbgMessageBox( message.string );
}

#define GfxDbgError() GfxDbgErrorInfo( "" )
#define GfxDbgErrorInfo( ... ) \
{ \
	TGfxDbgStrBuffer tMessage = GfxDbgStrFormat( __VA_ARGS__ ); \
	GfxDbgMsgHelper( "Critical error : ", tMessage.string, "???", __FILE__, __LINE__ ); \
}

#define GfxDbgBreak() if( IsDebuggerPresent() ) { __asm { _emit 0xF1 }; } else DebugBreak()

void GfxDbgMessageBox(const char *pMessage)
{
	//MessageBox(0, pMessage, 0, MB_OK | MB_ICONERROR);
}

int GfxDbgPrintf(const char *pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	int count = vsnprintf(NULL, 0, pFormat, args);
	char * buffer = (char *)malloc(count + 1);
	vsnprintf(buffer, count + 1, pFormat, args);
	std::cerr << buffer;

	//CharToOem(buffer, buffer);
	fputs(buffer, stdout);

	free(buffer);
	return count;
	return 0;
}

#include <iostream>
void GfxDbgAssert( bool bExpectedTrue, const char * pMessage )
{
	if( !bExpectedTrue )
	{
        std::cout << pMessage << std::endl;
		GfxDbgErrorInfo( !pMessage ? "assertion failure" : pMessage );
	}
}
