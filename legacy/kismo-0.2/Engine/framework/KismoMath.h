/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMath.h
 * Status: unique (no other copy of this file name in the archive)
 */
/*
Math -
	Includes accelerated, or close estimate math functions and helpers for mathematical calulcations
*/
#ifndef _KISMO_MATH_H
#define	_KISMO_MATH_H

#define NOMINMAX

#include "framework/KismoTypes.h"
#include <cmath>
//#include <algorithm>

namespace kismo
{
	namespace frx
	{
		namespace math
		{
			//Constants
			const k_Real kc_PI = (k_Real)3.1415926535897932384626433832795;
			const k_Real kc_2PI = kc_PI * 2;
			const k_Real kc_DEG2RAD = kc_PI / (k_Real)180.0;
			const k_Real kc_RAD2DEG = 1.0f / kc_DEG2RAD;

			//------------------------------------------
			// Square & Square root
			//------------------------------------------
			template < class T > inline T square( T a ) { return a*a; }
			template < class T > inline T squareRoot( T a ) { return sqrt(a); }				
			inline float fastSquareRoot( float a ) // http://www.gamedev.net/community/forums/topic.asp?topic_id=391237
			{
				//NOTICE: Accuracy +- 0.01% to 0.1% est. 
				// calculate the 1 / sqrt
				float a_half = a * 0.5f;
				int i = *( int* )&a;
				i = 0x5f3759df - ( i >> 1 );
				a = *( float* )&i;
				return 1.0f / (a * ( 1.5f - a_half * a * a ));
			}

			//------------------------------------------
			// Vector length
			//------------------------------------------
			//2D
			template < class T > inline T lengthSquared( T x, T y) { return ( x * x ) + ( y * y ); }
			template < class T > inline T length( T x, T y ) { return fastSquareRoot( lengthSquared( x, y ) ); }
			template < class T > inline T trueLength( T x, T y) { return squareRoot( lengthSquared( x, y ) ); }
			template < class T > inline T estimatedLength( T x, T y) { return ( x + y ); }	

			//3D
			template < class T > inline T lengthSquared( T x, T y, T z ) { return (x*x) + (y*y) + (z*z); }
			template < class T > inline T length( T x, T y, T z ) { return fastSquareRoot( lengthSquared( x, y, z ) ); }
			template < class T > inline T trueLength( T x, T y, T z ) { return squareRoot( lengthSquared( x, y, z ) ); }
			template < class T > inline T estimatedLength( T x, T y, T z ) { return ( x + y + z ); }		

			//------------------------------------------
			// Min & max
			//------------------------------------------
			#ifndef max
				template < class T > 
					inline T max( T a, T b ) { return ( a > b ) ? a : b; }
			#endif
			#ifndef min
				template < class T > 
					inline T min( T a, T b ) { return ( a < b ) ? a : b; }
			#endif

			template < class T > inline T arrayMax( T* arr, k_Count count ) 
			{ 
				T tmp = arr[0];
				for ( T* curr = arr + count; --curr > arr; )
					if ( (*curr) > tmp ) 
						tmp = (*curr);
				return tmp;
			}

			template < class T > inline T arrayMin( T* arr, k_Count count ) 
			{ 
				T tmp = arr[0];
				for ( T* curr = arr + count; --curr > arr; )
					if ( (*curr) < tmp ) 
						tmp = (*curr);
				return tmp;
			}

			//------------------------------------------
			// Swap values stored at left & right
			//------------------------------------------
			//template< class T > inline void swap( T& _left, T& _right)
			//{ 
			//	T tmp = _left;
			//	_left = _right, _right = tmp;
			//}		
			template < class T > void swap( T& i, T& j ) 
			{
				i -= j;
				j += i; // j gets the original value of i
				i = (j - i); // i gets the original value of j
			}


			//------------------------------------------
			//Gets the sign of a value (-1 is negative, 0 is neuter, & 1 is positive )
			//------------------------------------------
			template <typename T> inline int sign( const T& value ) { return (value < 0 ? -1 : value > 0); }
			
			//------------------------------------------
			// Clamp a value within a low & high boundary
			//------------------------------------------
			template <typename T> inline T clamp( T& value, T low,  T high)
			{
				if ( value < low )
					return ( value = low );
				else
				if ( value > high )
					return ( value = high );
				else
					return value;
			}
			template <typename T> inline T clamped( T value, T low,  T high)
			{
				return (value < low ? low : (value > high ? high : value));
			}

			//-----------------------------------
			// Fast absolute
			//-----------------------------------
			inline float fastAbs( k_Float32 f ) 
			{
				int i=((*(int*)&f)&0x7fffffff);
				return (*(float*)&i);
			}

		} // END: math
	} // END: frx
} // END: kismo


#endif
