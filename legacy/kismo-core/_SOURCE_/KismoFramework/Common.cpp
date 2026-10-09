/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Common.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "Common.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cAssert>

namespace kismo
{

	//TODO; Imeplemnt platform specific Assert
	//http://www.flipcode.com/archives/An_Assert_Replacement.shtml
	bool kismoAssertFunction( const Char* expression, const Char* description, UInt32 line, const Char* file, bool* alwaysIgnore )
	{
		int ret = MessageBox( 0, description, expression, alwaysIgnore ? MB_ABORTRETRYIGNORE : MB_ABORTRETRYIGNORE );
		switch ( ret )
		{
		case IDIGNORE:
			if ( alwaysIgnore )
				*alwaysIgnore = true;
			return false;
		case IDABORT:
			exit(-1); //, Abort! probably not he best idea to do this really!
			return false;
		default:
			__debugbreak();
			return true; //, break into code
		}
		//if( OpenClipboard( NULL ) )
		//{
		//   HGLOBAL hMem;
		//   char szAssert[256];
		//   char *pMem;

		//   sprintf( szAssert, "Put Assert info here" );
		//   hMem = GlobalAlloc( GHND|GMEM_DDESHARE, strlen( szAssert )+1 );

		//   if( hMem ) {
		//	  pMem = (char*)GlobalLock( hMem );
		//	  strcpy( pMem, szAssert );
		//	  GlobalUnlock( hMem );
		//	  EmptyClipboard();
		//	  SetClipboardData( CF_TEXT, hMem );
		//   }

		//   CloseClipboard();
		//}
		
		return true; //, break into code
	}

} //END: kismo