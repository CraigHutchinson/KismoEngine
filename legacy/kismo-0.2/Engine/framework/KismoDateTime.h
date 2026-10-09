/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoDateTime.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_DATETIME_H
#define	_KISMO_DATETIME_H

#include "framework/KismoCommon.h"

namespace kismo
{
	namespace frx
	{

		class _KismoExport Date
		{
		public:
			enum ke_Months
			{
				kc_January = 1,		kc_Jan = kc_January,	/**< January = 1 */
				kc_February = 2,	kc_Feb = kc_February,	/**< February = 2 */
				kc_March = 3,		kc_Mar = kc_March,		/**< March = 3 */
				kc_April = 4,		kc_Apr = kc_April,		/**< April = 4 */
				kc_May = 5,									/**< May = 5 */
				kc_June = 6,		kc_Jun = kc_June,		/**< June = 6 */
				kc_July = 7,		kc_Jul = kc_July,		/**< July = 7 */
				kc_August = 8,		kc_Aug = kc_August,		/**< August = 8 */
				kc_September = 9,	kc_Sep = kc_September,	/**< September = 9 */
				kc_October = 10,	kc_Oct = kc_October,	/**< October = 10 */
				kc_November = 11,	kc_Nov = kc_November,	/**< November = 11 */
				kc_December = 12,	kc_Dec = kc_December	/**< December = 12 */
			};
		public:

			/** \brief Returns whether the year is a leap year */
			inline static bool isLeapYear( k_UInt16 year ) 
			{ return (year>0) && !(year%4) && ( (year%100) || !(year%400) ); }			
			/** \brief Counts the number of leap days up to January 1st of a given year (From 01/01/1AD)*/
			inline static k_UInt16 countLeapYears( k_UInt16 year )
			{ return (year-1)/4 - (year-1)/100 + (year-1)/400; }
			/** \brief Count the number of days up to January 1st of the specified year (From 01/01/1AD)*/
			inline static k_UInt16 countDaysToYear( k_UInt16 year )
			{ return (year-1)*365 + countLeapYears(year); }
			/** \brief Counts the number of years spanned by a given number of days.
			\remarks Inverse of countDays( year ) */
			inline static k_UInt16 countYears( k_UInt16 day )
			{ return 1 + ( day - countLeapYears( day / 365 ) )/365; }
			/** \brief Adds a number of days to the specified year and returns the resulting year */
			inline static k_UInt16 yearPlusDays( k_UInt16 year, k_Int16 days )
			{ return countYears( countDaysToYear( year ) + days ); }
			/** \brief Counts the number of days from 01/01/YearA to 01/01/YearB */
			inline static k_Int16 daysBetweenYears( k_UInt16 yearA,  k_UInt16 yearB )
			{ return ( yearA != yearB ? countDaysToYear( yearB ) - countDaysToYear( yearA ) : 0 ); }
			/** \brief Returns the number of days in the given year
			\param[in] month The month
			\param[in] leapYear Whether the calculation should assume a leap year*/
			static k_UInt16 daysInMonth( k_UInt8 month, k_Bool leapYear = false );
			/** \brief Returns the number of days in the year leading upto the start of the month from 01/01/xxxx
			\param[in] month The month number to count upto
			\param[in] leapYear Whether the calculation should assume a leap year*/
			static k_UInt16 countDaysToMonth( k_UInt8 month, k_Bool leapYear = false );			
			/** \brief Returns the number of days between two months
			\param[in] monthA The staring month
			\param[in] leapYear Whether the calculation should assume a leap year*/
			inline static k_Int16 daysBetweenMonths( k_UInt8 monthA, k_UInt8 monthB , k_Bool leapYear = false )
			{ return (monthA != monthB ? countDaysToMonth( monthB, leapYear ) - countDaysToMonth( monthA, leapYear ) : 0 ); }
			/** \brief Returns the day of the year ( 1 to 365 )
			\param[in] day The day to count
			\param[in] month The month number to count
			\param[in] leapYear Whether the calculation should assume a leap year
			\remarks Does not check that day is valid for the given month */
			inline static k_UInt16 dayOfYear( k_UInt8 day, k_UInt8 month, k_Bool leapYear = false )
			{ return countDaysToMonth( month, leapYear ) + day; }
			/** \brief Count the number of days between 01/monthA/yearA to 01/monthB/yearB */
			inline static k_Int16 daysBetweenMonthYear( k_UInt8 monthA, k_UInt16 yearA, k_UInt8 monthB, k_UInt16 yearB )
			{ return daysBetweenYears( yearA, yearB ) - (monthA != monthB || yearA != yearB ? countDaysToMonth( monthA, isLeapYear( yearA ) ) + countDaysToMonth( monthB, isLeapYear( yearB ) ) : 0 ); }
			/** \brief Count the number of days between dayA/monthA/yearA to dayB/monthB/yearB */
			inline static k_Int16 daysBetweenDate( k_UInt8 dayA, k_UInt8 monthA, k_UInt16 yearA, k_UInt8 dayB, k_UInt8 monthB, k_UInt16 yearB )
			{ return daysBetweenMonthYear( monthA, yearA, monthB, yearB ) - dayA + dayB; }	
		public:
			Date();
			Date( k_UInt8 day, k_UInt8 month, k_UInt16 year ) : 
				m_day( day ), m_month( month ), m_year( year ) 
			{}
			/** \brief Returns the day of the month ( 1 to 31 ) ...as k_UInt16 as string insertion doesn't like k_Uint8... char ;)  */
			k_UInt16 day() const { return m_day; }
			/** \brief Returns the day of the year ( 1 to 356 ) */
			k_UInt16 dayOfyear() const;
			/** \brief Sets the day of the month ( 1 to 31 ) */
			void setDay( k_UInt8 day );
			/** \brief Month of year ( 1 to 12 ) */
			k_UInt16 month() const { return m_month; }
			/** \brief Returns the name of the month (e.g. February, January etc...) 
			\param[in] shortName Specifies whether the short string name should be returned*/
			const k_String& monthString( k_Bool shortName = true ) const;
			/** \brief Set the month of year ( 1 to 12 ) (as k_UInt16 for string insertion) */
			void setMonth( k_UInt8 month );
			/** \brief Returns the year 0to kc_UInt16_MAX) */
			k_UInt16 year() const { return m_year; }
			/** \brief Set the year (0 to kc_UInt16_MAX) */
			void setYear( k_UInt16 year );
			/** \brief Returns whether the current year is a leap year */
			k_Bool isLeapYear() const { return isLeapYear( year() ); }
			/** \brief Returns the number of days to another date
			\param[in] d The date to find the number of days to */
			k_Int32 daysTo( const Date& d ) const { return daysBetweenDate( (k_UInt8)day(), (k_UInt8)month(), year(), (k_UInt8)d.day(), (k_UInt8)d.month(), d.year() ); }
		private:
			//NOTE: Could be stored more compactly but will still expand to the 4byte barrier
			k_UInt8 m_day; ///< The day of the month
			k_UInt8 m_month; ///< The month
			k_UInt16 m_year; ///< The year
		};

		class _KismoExport Time
		{
		public:

		};

		class _KismoExport DateTime
		{
		public:

			enum ke_DateTimeType
			{
				kc_Date,
				kc_Time,
				kc_DateTime,
			};

		public:

			DateTime( ke_DateTimeType dateTimetype = kc_DateTime, 
				k_Char dateSeperator = _T('/'),
				k_Char timeSeperator = _T(':'),
				k_Char dateTimeSeperator = _T(' ') );
					
			ke_DateTimeType type() { return m_type; }//get the dateTime type
			void setType( ke_DateTimeType dateTimetype ) { m_type = dateTimetype; } //Set the type

			//Stream operators			
			friend k_OStream& operator << ( k_OStream& outStream, const DateTime& d );

			bool isDaylightSavings() { return m_isDaylightSaving; }

		protected:
			ke_DateTimeType m_type;//The type of date/time format

			k_UInt8 m_second;     /* seconds after the minute - [0,59] */
			k_UInt8 m_minute;     /* minutes after the hour - [0,59] */
			k_UInt8 m_hour;    /* hours since midnight - [0,23] */
			k_UInt8 m_dayOfMonth;    /* day of the month - [1,31] */
			k_UInt8 m_month;     /* months since January - [0,11] */
			k_UInt8 m_year;    /* years since 1900 */
			k_UInt8 m_dayOfWeek;    /* days since Sunday - [0,6] */
			k_UInt16 m_dayOfYear;    /* days since January 1 - [0,365] */
			k_Bool m_isDaylightSaving;   /* daylight savings time flag */
		
			k_Char m_dateSeperator;
			k_Char m_timeSeperator;
			k_Char m_dateTimeSeperator;

		};

	} //END: frx

} //END: kismo


#endif