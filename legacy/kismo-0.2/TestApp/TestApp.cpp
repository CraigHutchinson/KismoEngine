/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\TestApp\TestApp.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "system\KismoCore.h"
#include "framework/KismoDateTime.h"
#include "framework/KismoString.h"
#include "framework/KismoTimer.h"
//using kismo::Core;
//using kismo::gui::Console;
//using kismo::frx::Log;
//using kismo::frx::Timer;
using kismo::frx::Timer;
using kismo::frx::String;
using kismo::frx::Date;
using namespace kismo;

#include "gfx/KismoRenderer.h"


//
//
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>
//#include <iostream>
#include <crtdbg.h>//Include debug heap modifier to show memory leaks

#include "TestWindow1.h"


//class LogTest : public Thread
//{
//public:
//	LogTest( k_String ntxt ) : Thread( ntxt ), txt( ntxt ) { setThreadPriority(Thread::kc_ThreadPriority_Idle);}
//
//	void run()
//	{
//		for ( unsigned int i =0; i < 10000; ++i)
//		{
//			LOG_COMMENT( txt << i << "\t\t\t\t\t\t\t--------------------------------");
//			//sleep(0);
//		}
//	}
//
//	k_String txt;
//
//};

void testString();
void testDateTime();
void testTimers();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//InitCommonControls();


	kismo::frx::Log log( _T("Kismo.log") );
	kismo::gui::Console console;
	kismo::Core core( log );

	//// Leak checking on PC Platform in debug mode only - needs to be set at app entry
	DEBUG_ONLY( _CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF ); )

	testString();
	testDateTime();
	testTimers();

	//return 0;
	

	//LogTest test1( _T("thread 1 :") );
	//LogTest test2( _T("thread 2 :") );
	//LogTest test3( _T("thread 3 :") );
	//LogTest test4( _T("thread 4 :") );
	//LogTest test5( _T("thread 5 :") );


	////Timer timer;
	////timer.start();

	//test1.start();
	//test2.start();
	//test3.start();
	//test4.start();
	//test5.start();

	//DateTime fdyd;


	//LOG_ERROR( _T("***** All thread finished = ") << timer.getApplicationTime() / 1000.0f << _T(" *****") );

	//log.terminate();
	//
	//LOG_ERROR( _T("***** Log output = ") << timer.getApplicationTime() / 1000.0f << _T(" *****") );
	////log.run();

	//LOG_ERROR( "################## END #####################" );
	//
	////log.terminate();

	//log.start();

	if ( !core.loadPlugin( _T("Win32_GUIRenderer") ) )
		return 0;

	if ( !core.loadPlugin( _T("OGL_Renderer") ) )
		return 0;

	gui::GUIRenderer* win32 = core.gui().getRenderer( NAME_SWITCH( Win32 ) );  //Get the Win32 GUI renderer
	gfx::Renderer* ogl = core.gfx().getRenderer( NAME_SWITCH( OpenGL ) ); //Get the openGl renderer

	TestWindow1 testWindow1( win32, ogl );

	////gfx::RenderWindow* renderWindow1 = new RenderFrame( &testWindow1, ogl );

	testWindow1.exec();

	///*test1.waitForThreadTerminate();
	//test2.waitForThreadTerminate();
	//test3.waitForThreadTerminate();
	//test4.waitForThreadTerminate();
	//test5.waitForThreadTerminate();*/


	core.gui().waitForThreadTerminate();
	//
	//log.stop();

	return 0;
}


void testString()
{

	k_Size string8Size = sizeof( frx::String8 );
	k_Size string16Size = sizeof( frx::String16 );

	frx::String8 test8( 17, 'a' );
	frx::String16 test16( 17, L'w' );
	frx::String8 copy8( test8 );
	frx::String16 copy16( test16 );
	frx::String8 ccopy8( test8.c_str() );
	frx::String16 ccopy16( test16.c_str() );

	frx::String8 test8b( "testString" );
	frx::String16 test16b( L"testString" );
	test8b.reserve( 20 );
	test16b.reserve( 20 );
	
	frx::String8 test8c( 15 );
	frx::String16 test16c( 15 );

	bool same8 = (test8 == test8);
	bool same16 = (test16 == test16);
	bool diff8 = (test8 != test8b);
	bool diff6 = (test16 != test16b);


	frx::String8 concat8 = (test8 + test8);
	frx::String16 concat16 = (test16 + test16);
	frx::String8 concat8b = (test8 + test8b);
	frx::String16 concat16b = (test16 + test16b);
	frx::String8 concat8c = (test8 + "_c#-style_#" + test8b);
	frx::String16 concat16c = (test16 + L"#_c-style_#" + test16b);

	frx::String8 copy8c = concat8c;
	frx::String16 copy16c = concat16c;
	copy8c += "#_Concatinated C-Style_";
	copy16c += L"#_Concatinated C-Style_#";

	frx::String8 copy8d = copy8c;
	frx::String16 copy16d = copy16c;
	copy8d += frx::String8("@_Concatinated String_@");
	copy16d += frx::String16( L"@_Concatinated String_@" );

	frx::String stream;
	stream << "Streamed";
	stream << "|partA|" <, "|partB|";
}


void testDateTime()
{
	Date dateA( 1, 1, 2000 );
	Date dateB( 31, 1, 2000 );
	Date dateC( 31, 5, 2000 );
	Date dateD( 31, 12, 2000 );
	Date dateE( 31, 12, 2010 );
	Date dateF( 31, 12, 1999 );
	Date dateG( 31, 11, 1999 );
	int aToB = dateA.daysTo( dateB );
	int aToC = dateA.daysTo( dateC );
	int aToD = dateA.daysTo( dateD );
	int aToE = dateA.daysTo( dateE );
	int aToF = dateA.daysTo( dateF );
	int aToG = dateA.daysTo( dateG );

	
	Date dateNow;
	Date dateBirth( 8, 3, 1985 );
	int ageInDays = dateBirth.daysTo( dateNow );
	int age = Date::countYears( ageInDays );

}

void testTimers()
{
	k_UInt32 ticksPerSecond = _SystemTimer.ticksPerSecond();
	k_Float64 systemSeconds = _SystemTimer.system_secs();
	k_Float64 systemMs = _SystemTimer.system_ms();
	k_Int64 systemTicks = _SystemTimer.system_ticks();

	k_Float32 runningTimeSeconds = _SystemTimer.runningTime_secs();
	k_Float32 runningTimeMs = _SystemTimer.runningTime_ms();
	k_UInt32 runningTimeTicks = _SystemTimer.runningTime_ticks();

	Thread::sleep( 500 );
	
	k_Float32 runningTimeSecondsb = _SystemTimer.runningTime_secs();
	k_Float32 runningTimeMsb = _SystemTimer.runningTime_ms();
	k_UInt32 runningTimeTicksb = _SystemTimer.runningTime_ticks();

	k_Float32 runTimeBSecondsElapsed = runningTimeSecondsb - runningTimeSeconds;
	k_Float32 runTimeBMsElapsed = runningTimeMsb - runningTimeMs;
	k_UInt32 runTimeBTicksElapsed = runningTimeTicksb - runningTimeTicks;

	kismo::frx::SimulationTimer simTimer;

	simTimer.start();
	Thread::sleep( 500 );
	simTimer.setSimulationTimeScale( 0.5f );
	Thread::sleep( 500 );	
	k_Float32 simTimeMsElapsedB = simTimer.simulationTime_ms();
	simTimer.setSimulationTimeScale( -1.5f );
	Thread::sleep( 500 );	
	k_Float32 simTimeMsElapsedC = simTimer.simulationTime_ms();
	simTimer.pause();
	Thread::sleep( 500 );
	simTimer.start();
	k_Float32 simTimeMsElapsedD = simTimer.simulationTime_ms();
	simTimer.stop();
	k_Float32 simTimeMsElapsedE = simTimer.simulationTime_ms();
	simTimer.setSimulationTimeScale( 1.0f );
	simTimer.start();
	Thread::sleep( 500 );
	k_Float32 simTimeMsElapsedF = simTimer.simulationTime_ms();
	simTimer.setSimulationTimeSecs( 1 );
	k_Float32 simTimeMsElapsedG = simTimer.simulationTime_ms();
	Thread::sleep( 500 );
	k_Float32 simTimeMsElapsedH = simTimer.simulationTime_ms();
	simTimer.pause();
	simTimer.setSimulationTimeScale( 10.0f );
	Thread::sleep( 500 );
	simTimer.start();
	k_Float32 simTimeMsElapsedI = simTimer.simulationTime_ms();




}