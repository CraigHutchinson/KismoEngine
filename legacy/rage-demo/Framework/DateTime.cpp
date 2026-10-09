/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\DateTime.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/DateTime.cpp (same name, content differs)
 */
#include "DateTime.h"

#include <iomanip>
#include <ctime>

namespace frx
{
	//---------------------------------------------------------------------------------------------------
	//Static date functions
	//---------------------------------------------------------------------------------------------------
	struct MonthInfo { const String shortName; const String longName; UInt8 days; UInt16 daysTo; UInt16 leapDays; UInt16 leapDaysTo; };
	const MonthInfo	monthInfo[12] = {
		{_T("Jan"), _T("January"),	31, 0,   31, 0 },
		{_T("Feb"), _T("February"),	28, 31,  29, 31 },
		{_T("Mar"), _T("March"),	31, 59,  31, 60 },
		{_T("Apr"), _T("April"),	30, 89,  30, 90 },
		{_T("May"), _T("May"),		31, 120, 31, 121 },
		{_T("Jun"), _T("June"),		30, 150, 30, 151 },
		{_T("Jul"), _T("July"),		31, 181, 31, 182 },
		{_T("Aug"), _T("August"),	31, 212, 31, 213 },
		{_T("Sep"), _T("September"),30, 242, 30, 243 },
		{_T("Oct"), _T("October"),	31, 273, 31, 274 },
		{_T("Nov"), _T("November"),	30, 303, 30, 305 },
		{_T("Dec"), _T("December"),	31, 334, 31, 335 }	};
	
	UInt16 Date::daysInMonth( UInt8 month, Bool leapYear )
	{
		assert( month > 0 && month <= 12 );
		if ( leapYear )
			return monthInfo[ month - 1 ].leapDays;
		else
			return monthInfo[ month - 1 ].days;
	}

	UInt16 Date::countDaysToMonth( UInt8 month, Bool leapYear )
	{
		assert( month > 0 && month <= 12 );
		if ( leapYear )
			return monthInfo[ month - 1 ].leapDaysTo;
		else
			return monthInfo[ month - 1 ].daysTo;
	}
	
	const String& Date::monthName( UInt8 month, Bool shortName )
	{
		assert( month > 0 && month <= 12 );
		if ( shortName )
			return monthInfo[ month - 1 ].shortName;
		else
			return monthInfo[ month - 1 ].longName ;
	}

	//---------------------------------------------------------------------------------------------------
	//Date impl
	//---------------------------------------------------------------------------------------------------
	Date::Date()
	{
		time_t ctTime; 
		time( &ctTime );
		tm dateTime;
		localtime_s( &dateTime, &ctTime );

		m_day = dateTime.tm_mday; //day of the month - [1,31]
		m_month = dateTime.tm_mon + 1; //months since January - [0,11]
		m_year = dateTime.tm_year + 1900; //years since 1900
	}


	//DateTime::DateTime( ke_DateTimeType dateTimetype, 
	//	Char dateSeperator,
	//	Char timeSeperator,
	//	Char dateTimeSeperator 
	//) : 
	//	m_type( dateTimetype ),
	//	m_dateSeperator( dateSeperator ),
	//	m_timeSeperator( timeSeperator ),
	//	m_dateTimeSeperator( dateTimeSeperator )
	//{
	//	time_t ctTime; 
	//	time( &ctTime );
	//	tm* dateTime = localtime( &ctTime );

	//	m_second = dateTime->tm_sec; //seconds after the minute - [0,59]
	//	m_minute = dateTime->tm_min; //minutes after the hour - [0,59]
	//	m_hour = dateTime->tm_hour; //hours since midnight - [0,23]
	//	m_dayOfMonth = dateTime->tm_mday; //day of the month - [1,31]
	//	m_month = dateTime->tm_mon; //months since January - [0,11]
	//	m_year = dateTime->tm_year; //years since 1900
	//	m_dayOfWeek = dateTime->tm_wday; //days since Sunday - [0,6]
	//	m_dayOfYear = dateTime->tm_yday; //days since January 1 - [0,365]
	//	m_isDaylightSaving = ( dateTime->tm_isdst != 0 ); //daylight savings time flag
	//}

	////Stream operators
	//OStream& operator << ( OStream& outStream, const DateTime& d )
	//{
	//	if ( d.m_type == DateTime::Date || d.m_type == DateTime::DateTime )
	//		outStream
	//			<< std::setw(2) << std::setfill(_T('0')) << d.m_dayOfMonth
	//			<< d.m_dateSeperator
	//			<< std::setw(2) << std::setfill(_T('0')) << d.m_month + 1
	//			<< d.m_dateSeperator
	//			<< std::setw(4) << d.m_year + 1900;
	//	
	//	if ( d.m_type == DateTime::DateTime )
	//		outStream << d.m_dateTimeSeperator;

	//	if ( d.m_type == DateTime::Time || d.m_type == DateTime::DateTime )
	//		outStream
	//			<< std::setw(2) << std::setfill(_T('0')) << d.m_hour
	//			<< d.m_timeSeperator
	//			<< std::setw(2) << std::setfill(_T('0')) << d.m_minute
	//			<< d.m_timeSeperator
	//			<< std::setw(2) << std::setfill(_T('0')) << d.m_second;

	//	return outStream;
	//}

} //END: frx
