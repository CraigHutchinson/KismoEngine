/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\DateTime.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/DateTime.h (same name, content differs)
 */
#ifndef FRX_DATETIME_H
#define	FRX_DATETIME_H

#include "framework/Common.h"

namespace frx
{

	/** \brief Holds a date value 
	*/
	class _DllExport Date
	{
	public:

		/** \brief Month enumerator with long and short names 
		*/
		enum eMonth
		{
			January = 1,	Jan = January,	///< January = 1
			February = 2,	Feb = February,	///< February = 2
			March = 3,		Mar = March,	///< March = 3
			April = 4,		Apr = April,	///< April = 4
			May = 5,						///< < May = 5
			June = 6,		Jun = June,		///< June = 6
			July = 7,		Jul = July,		///< July = 7
			August = 8,		Aug = August,	///< August = 8
			September = 9,	Sep = September,///< September = 9
			October = 10,	Oct = October,	///< October = 10
			November = 11,	Nov = November,	///< November = 11
			December = 12,	Dec = December	///< December = 12
		};

	public:

		/** \brief Returns whether the year is a leap year 
		*/
		inline static bool isLeapYear( UInt16 year ) 
		{ return (year>0) && !(year%4) && ( (year%100) || !(year%400) ); }			
		
		/** \brief Counts the number of leap days up to January 1st of a given year (From 01/01/1AD)
		*/
		inline static UInt16 countLeapYears( UInt16 year )
		{ return (year-1)/4 - (year-1)/100 + (year-1)/400; }
		
		/** \brief Count the number of days up to January 1st of the specified year (From 01/01/1AD)
		*/
		inline static UInt16 countDaysToYear( UInt16 year )
		{ return (year-1)*365 + countLeapYears(year); }
		
		/** \brief Counts the number of years spanned by a given number of days.
		\remarks Inverse of countDays( year ) 
		*/
		inline static UInt16 countYears( UInt16 day )
		{ return 1 + ( day - countLeapYears( day / 365 ) )/365; }
		
		/** \brief Adds a number of days to the specified year and returns the resulting year 
		*/
		inline static UInt16 yearPlusDays( UInt16 year, Int16 days )
		{ return countYears( countDaysToYear( year ) + days ); }
		
		/** \brief Counts the number of days from 01/01/YearA to 01/01/YearB 
		*/
		inline static Int16 daysBetweenYears( UInt16 yearA,  UInt16 yearB )
		{ return ( yearA != yearB ? countDaysToYear( yearB ) - countDaysToYear( yearA ) : 0 ); }
		
		/** \brief Returns the number of days in the given year
		\param[in] month The month
		\param[in] leapYear Whether the calculation should assume a leap year
		*/
		static UInt16 daysInMonth( UInt8 month, Bool leapYear = false );
		
		/** \brief Returns the number of days in the year leading upto the start of the month from 01/01/xxxx
		\param[in] month The month number to count upto
		\param[in] leapYear Whether the calculation should assume a leap year
		*/
		static UInt16 countDaysToMonth( UInt8 month, Bool leapYear = false );			
		
		/** \brief Returns the number of days between two months
		\param[in] monthA The staring month
		\param[in] leapYear Whether the calculation should assume a leap year
		*/
		inline static Int16 daysBetweenMonths( UInt8 monthA, UInt8 monthB , Bool leapYear = false )
		{ return (monthA != monthB ? countDaysToMonth( monthB, leapYear ) - countDaysToMonth( monthA, leapYear ) : 0 ); }
		
		/** \brief Returns the day of the year ( 1 to 365 )
		\param[in] day The day to count
		\param[in] month The month number to count
		\param[in] leapYear Whether the calculation should assume a leap year
		\remarks Does not check that day is valid for the given month 
		*/
		inline static UInt16 dayOfYear( UInt8 day, UInt8 month, Bool leapYear = false )
		{ return countDaysToMonth( month, leapYear ) + day; }
		
		/** \brief Count the number of days between 01/monthA/yearA to 01/monthB/yearB 
		*/
		inline static Int16 daysBetweenMonthYear( UInt8 monthA, UInt16 yearA, UInt8 monthB, UInt16 yearB )
		{ return daysBetweenYears( yearA, yearB ) - (monthA != monthB || yearA != yearB ? countDaysToMonth( monthA, isLeapYear( yearA ) ) + countDaysToMonth( monthB, isLeapYear( yearB ) ) : 0 ); }
		
		/** \brief Count the number of days between dayA/monthA/yearA to dayB/monthB/yearB 
		*/
		inline static Int16 daysBetweenDate( UInt8 dayA, UInt8 monthA, UInt16 yearA, UInt8 dayB, UInt8 monthB, UInt16 yearB )
		{ return daysBetweenMonthYear( monthA, yearA, monthB, yearB ) - dayA + dayB; }	

		/** \brief Returns a month as a string
		\param[in] month The number of the month (1 to 12)
		\param[in] shortName Specifies whether the short name should be returned or the full name
		\remarks
			This function will assert if an invalid month number is supplied */
		static const String& monthName( UInt8 month, Bool shortName = false );

	public:

		/** \brief Constructor creates a new date structure set to the current date */
		Date();

		/** \brief Constructor takes a date
		\param[in] day The day part of the date ( 1 to 31 ) 
		\param[in] month the month part of the date ( 1 to 12 )
		\param[in] year The year part of the date 
		*/
		Date( UInt8 day, UInt8 month, UInt16 year ) : 
			m_day( day ), m_month( month ), m_year( year ) 
		{}

		/** \brief Returns the day of the month ( 1 to 31 ) 
		\returns Day as UInt16 as string insertion to a stream doesn't like Uint8 which is basically a char ;)  
		*/
		UInt16 day() const 
		{ return m_day; }

		/** \brief Returns the day of the year ( 1 to 366 ) 
		*/
		UInt16 dayOfyear() const
		{ return dayOfYear( m_day, m_month, isLeapYear() ); }

		/** \brief Sets the day of the month ( 1 to 31 ) 
		*/
		void setDay( UInt8 day )
		{ m_day = day; }

		/** \brief Month of year ( 1 to 12 ) */
		UInt16 month() const 
		{ return m_month; }

		/** \brief Returns the name of the month (e.g. February, January etc...) 
		\param[in] shortName Specifies whether the short string name should be returned
		*/
		const String& monthName( Bool shortName = false ) const
		{ return monthName( m_month, shortName ); }

		/** \brief Set the month of year ( 1 to 12 ) (as UInt16 for string insertion) 
		*/
		void setMonth( UInt8 month )
		{ m_month = month; }

		/** \brief Returns the year 0 to UInt16_MAX) 
		*/
		UInt16 year() const 
		{ return m_year; }

		/** \brief Set the year (0 to UInt16_MAX) 
		*/
		void setYear( UInt16 year )
		{ m_year = year; }

		/** \brief Returns whether the current year is a leap year 
		*/
		Bool isLeapYear() const 
		{ return isLeapYear( year() ); }

		/** \brief Returns the number of days to another date
		\param[in] d The date to find the number of days to 
		*/
		Int32 daysTo( const Date& d ) const 
		{ return daysBetweenDate( m_day, m_month, m_year, (UInt8)d.day(), (UInt8)d.month(), (UInt16)d.year() ); }

	private:
		//NOTE: Could be stored more compactly but will still expand to the 4byte barrier anyhow
		UInt8 m_day; ///< The day of the month
		UInt8 m_month; ///< The month
		UInt16 m_year; ///< The year
	};

	class _DllExport Time
	{
	public:

	};

	/** \brief DateTime contanes a date time pair */
	class _DllExport DateTime
	{
	public:


	protected:

		Date m_date; ///< The date
		Time m_time; ///< The time

	};

} //END: frx

#endif