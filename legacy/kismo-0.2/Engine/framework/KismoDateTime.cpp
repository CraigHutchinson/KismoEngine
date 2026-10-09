/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoDateTime.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoDateTime.h"

#include <iomanip>
#include <ctime>

namespace kismo
{
	namespace frx
	{
		struct Monthinfo { const k_Char* shortName; const k_Char* longName; k_UInt8 days; k_UInt16 daysTo; };
		const Monthinfo	kc_Monthinfo[12] = {
			{_T("Jan"), _T("January"),	31, 0 },
			{_T("Feb"), _T("February"),	28, 31 },
			{_T("Mar"), _T("March"),	31, 59 },
			{_T("Apr"), _T("April"),	30, 89 },
			{_T("May"), _T("May"),		31, 120 },
			{_T("Jun"), _T("June"),		30, 150 },
			{_T("Jul"), _T("July"),		31, 181 },
			{_T("Aug"), _T("August"),	31, 212 },
			{_T("Sep"), _T("September"),30, 242 },
			{_T("Oct"), _T("October"),	31, 273 },
			{_T("Nov"), _T("November"),	30, 303 },
			{_T("Dec"), _T("December"),	31, 334 }	};
		
		k_UInt16 Date::daysInMonth( k_UInt8 month, k_Bool leapYear )
		{
			assert( month > 0 && month <= 12 );
			return kc_Monthinfo[ month - 1 ].days + (leapYear && (month == kc_Feb)? 1 : 0 );
		}

		k_UInt16 Date::countDaysToMonth( k_UInt8 month, k_Bool leapYear )
		{
			assert( month > 0 && month <= 12 );
			return kc_Monthinfo[ month - 1 ].daysTo + (leapYear && (month > kc_Feb)? 1 : 0 );
		}

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


		DateTime::DateTime( ke_DateTimeType dateTimetype, 
			k_Char dateSeperator,
			k_Char timeSeperator,
			k_Char dateTimeSeperator 
		) : 
			m_type( dateTimetype ),
			m_dateSeperator( dateSeperator ),
			m_timeSeperator( timeSeperator ),
			m_dateTimeSeperator( dateTimeSeperator )
		{
			time_t ctTime; 
			time( &ctTime );
			tm* dateTime = localtime( &ctTime );

			m_second = dateTime->tm_sec; //seconds after the minute - [0,59]
			m_minute = dateTime->tm_min; //minutes after the hour - [0,59]
			m_hour = dateTime->tm_hour; //hours since midnight - [0,23]
			m_dayOfMonth = dateTime->tm_mday; //day of the month - [1,31]
			m_month = dateTime->tm_mon; //months since January - [0,11]
			m_year = dateTime->tm_year; //years since 1900
			m_dayOfWeek = dateTime->tm_wday; //days since Sunday - [0,6]
			m_dayOfYear = dateTime->tm_yday; //days since January 1 - [0,365]
			m_isDaylightSaving = ( dateTime->tm_isdst != 0 ); //daylight savings time flag
		}

		//Stream operators
		k_OStream& operator << ( k_OStream& outStream, const DateTime& d )
		{
			if ( d.m_type == DateTime::kc_Date || d.m_type == DateTime::kc_DateTime )
				outStream
					<< std::setw(2) << std::setfill(_T('0')) << d.m_dayOfMonth
					<< d.m_dateSeperator
					<< std::setw(2) << std::setfill(_T('0')) << d.m_month + 1
					<< d.m_dateSeperator
					<< std::setw(4) << d.m_year + 1900;
			
			if ( d.m_type == DateTime::kc_DateTime )
				outStream << d.m_dateTimeSeperator;

			if ( d.m_type == DateTime::kc_Time || d.m_type == DateTime::kc_DateTime )
				outStream
					<< std::setw(2) << std::setfill(_T('0')) << d.m_hour
					<< d.m_timeSeperator
					<< std::setw(2) << std::setfill(_T('0')) << d.m_minute
					<< d.m_timeSeperator
					<< std::setw(2) << std::setfill(_T('0')) << d.m_second;

			return outStream;
		}

	} //END: frx

} //END: kismo
