/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\TestApp\TestWindow1.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "TestWindow1.h"


IMPL_SLOT( TestWindow1, moo )
{
	LOG_MESSAGE( getDefinition().text << _T(" ") << _T(" - Went Mooooo!") ); 
	button2->setPosition( button2->getDefinition().position + Vector2r( 10,10 ) );

	
	window1 = new gui::Window( this );
	window1->setText( _T("New Window") );
	window1->setSize( Vector2r( 100, 100 ) );
	window1->exec();


}

IMPL_SLOT( TestWindow1, term )
{
	_Gui.stop();

}