/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\Win32ConsoleListener.cpp
 * Status: current (newest copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "Win32ConsoleListener.h"
#include <iostream>
#include <windows.h>

namespace win32
{
	Win32ConsoleListener::Win32ConsoleListener()
	{
		SMALL_RECT new_win_dims;
		COORD new_size;

		//get the handle to the console
		HANDLE cur_screen = GetStdHandle (STD_OUTPUT_HANDLE);

		//resize the screen buffer
		new_size.X = 256; //columns
		new_size.Y = 2048; //lines 
		SetConsoleScreenBufferSize(cur_screen, new_size);

		//Resize the consoles window
		new_win_dims.Top = 0;
		new_win_dims.Left = 0;
		new_win_dims.Right = 127;
		new_win_dims.Bottom = 48;
		SetConsoleWindowInfo(cur_screen, TRUE, &new_win_dims);

	}

	void Win32ConsoleListener::write( const frx::Log::LogMessage& msg, frx::Log* origin )
	{
		if ( msg.type > frx::Log::Message )
		{
			std::cout << msg.str().c_str() << '\n';
		}
	}


} //END; win32