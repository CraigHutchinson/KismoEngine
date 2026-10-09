/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Colour.cpp
 * Status: current (newest copy of this file name in the archive)
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


#include "Colour.h"

namespace kismo
{	

	//Colour3& Colour3::operator =( const String& str )
	//{
	//	_stscanf_s( str.c_str(), _T("%f %f %f"), &r, &g, &b );
	//	return *this;
	//}

	//Colour4& Colour4::operator =( const String& str )
	//{
	//	_stscanf_s( str.c_str(), _T("%f %f %f %f"), &r, &g, &b, &a );
	//	return *this;
	//}

	const Colour3 Colour3::ZERO = Colour3( 0.0f, 0.0f, 0.0f );
	const Colour3 Colour3::Black = Colour3( 0.0f, 0.0f, 0.0f );
	const Colour3 Colour3::White = Colour3( 1.0f, 1.0f, 1.0f );
	const Colour3 Colour3::Red = Colour3( 1.0f, 0.0f, 0.0f );
	const Colour3 Colour3::Green = Colour3( 0.0f, 1.0f, 0.0f );
	const Colour3 Colour3::Blue = Colour3( 0.0f, 0.0f, 1.0f );
	const Colour3 Colour3::Cyan = Colour3( 0, 255, 255 ); 
	const Colour3 Colour3::Magenta = Colour3( 255, 0, 255 ); 
	const Colour3 Colour3::Yellow = Colour3( 255, 255, 0 );

	const Colour4 Colour4::ZERO = Colour4( 0.0f, 0.0f, 0.0f, 0.0f );
	const Colour4 Colour4::Black = Colour4( 0.0f, 0.0f, 0.0f );
	const Colour4 Colour4::White = Colour4( 1.0f, 1.0f, 1.0f );
	const Colour4 Colour4::Red = Colour4( 1.0f, 0.0f, 0.0f );
	const Colour4 Colour4::Green = Colour4( 0.0f, 1.0f, 0.0f );
	const Colour4 Colour4::Blue = Colour4( 0.0f, 0.0f, 1.0f );
	const Colour4 Colour4::Cyan = Colour4( 0, 255, 255 ); 
	const Colour4 Colour4::Magenta = Colour4( 255, 0, 255 ); 
	const Colour4 Colour4::Yellow = Colour4( 255, 255, 0 );

} //END: kismo