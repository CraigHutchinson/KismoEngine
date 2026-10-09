/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoColour.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoColour.h"

namespace kismo
{
	namespace frx
	{
		
		const Colour Colour::ZERO = Colour( 0.0f, 0.0f, 0.0f, 0.0f );
		const Colour Colour::Black = Colour( 0.0f, 0.0f, 0.0f );
		const Colour Colour::White = Colour( 1.0f, 1.0f, 1.0f );
		const Colour Colour::Red = Colour( 1.0f, 0.0f, 0.0f );
		const Colour Colour::Green = Colour( 0.0f, 1.0f, 0.0f );
		const Colour Colour::Blue = Colour( 0.0f, 0.0f, 1.0f );
		const Colour Colour::Cyan = Colour( 0, 255, 255 ); 
		const Colour Colour::Magenta = Colour( 255, 0, 255 ); 
		const Colour Colour::Yellow = Colour( 255, 255, 0 );

		bool Colour::ms_useRGBAStreamMode( false );
	}
}