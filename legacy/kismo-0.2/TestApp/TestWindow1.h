/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\TestApp\TestWindow1.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _TESTAPP_TESTWINDOW1_H
#define	_TESTAPP_TESTWINDOW1_H

#include "gui/KismoWindow.h"
#include "gui/KismoButton.h"
#include "gui/KismoRenderFrame.h"
#include <hash_set>
using namespace kismo;

class TestButton : public gui::Button
{
public:
	TestButton( Widget* w ) : gui::Button( w ) {}

	virtual void clicked()
	{ 
		LOG_MESSAGE( getDefinition().text << _T(" ") << _T(" - Button Clicked") ); 
	}
};


class TestWindow1 : public gui::Window
{
public:
	TestWindow1( gui::GUIRenderer* renderer, gfx::Renderer* gfxRenderer1 ) : gui::Window( 0, renderer )
	{		
		setText( _T("Test Window 1 ; Wooooo New text") );
		setPosition( Vector2r( 100,100) );
		setSize( Vector2r( 640,480) );
//		setEnabled( kc_TRUE );

		button1 = new TestButton( this );
		button1->setText( _T("moooo Button1") );	

		button2 = new TestButton( this );
		button2->setText( _T("moooo Button2") );
		button2->setPosition( Vector2r( 120, 0 ) );

		btnExit = new gui::Button( this );
		btnExit->setText( _T("Exit!!") );
		btnExit->setPosition( Vector2r( 30, 70 ) );

		renderWindow1 = new gui::RenderFrame( this, gfxRenderer1 );
		renderWindow1->setText( _T("RenderWindow") );
		renderWindow1->setPosition( Vector2r( 150, 150 ) );
		renderWindow1->setSize( Vector2r( 600, 400 ) );

		window1 = new gui::Window( this );
		window1->setText( _T("Child Window") );
		window1->setPosition( Vector2r( 50, 50 ) );
		window1->setSize( Vector2r( 100, 100 ) );

		btnExit->bind( gui::Button::kc_Slot_clicked, this, kc_Slot_term );
		
		button1->bind( gui::Button::kc_Slot_clicked, window1, kc_Slot_hide );		
		button2->bind( gui::Button::kc_Slot_clicked, window1, kc_Slot_show );

		//window1->bind( kc_Slot_hide, renderWindow1, kc_Slot_hide );	
		button1->bind( gui::Button::kc_Slot_clicked, this, kc_Slot_moo );


	}

	~TestWindow1()
	{
		safeDelete( btnExit );
		safeDelete( button1 );
		safeDelete( button2 );
		safeDelete( window1 );
	}

protected:

	DEFINE_SLOT( moo );
	DEFINE_SLOT( term );

	RECIEVE_SLOT_FUNC
	{
		EXEC_SLOT( moo );
		EXEC_SLOT( term );
		return EXEC_PARENT( Window );
	}


	gui::Button* btnExit;
	gui::Button* button1;
	gui::Button* button2;
	gui::Window* window1;
	gui::RenderFrame* renderWindow1;
};

#endif

