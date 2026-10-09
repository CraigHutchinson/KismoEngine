/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoRandom.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoRandom.h"
#include <ctime>

//Period paramaters
#define RAND_M 397
#define RAND_MATRIX_A 0x9908b0df //Constant vector a
#define RAND_UPPER_MASK 0x80000000 //Most significant w-r bits
#define RAND_LOWER_MASK 0x7fffffff //least significant r bits

//Tempering paramaters
#define RAND_TEMPERING_MASK_B 0x9d2c5680
#define RAND_TEMPERING_MASK_C 0xefc60000
#define RAND_TEMPERING_SHIFT_U(y) (y >> 11)
#define RAND_TEMPERING_SHIFT_S(y) (y << 7)
#define RAND_TEMPERING_SHIFT_T(y) (y << 15)
#define RAND_TEMPERING_SHIFT_L(y) (y >> 18)

namespace kismo
{
	namespace frx
	{	
		Random::Random()
		{
			m_seed = 1;
			m_mti = RAND_N + 1;
		}

		//Returns a number from 0 to n (excluding n)
		k_UInt32 Random::random( k_UInt32 n )
		{
			k_UInt32 y;
			static unsigned long mag01[2] = { 0x0, RAND_MATRIX_A };

			if ( n == 0 )
				return 0;

			// mag01[x] = x * MATRIX_A for x = 0,1

			if( m_mti >= RAND_N ) //Generate N words at one time
			{
				k_Int32 kk;

				if ( m_mti == RAND_N+1 )
					setSeed( 4358 ); //Set a default random seed

				for ( kk = 0; kk < RAND_N - RAND_M; ++kk )
				{
					y = (m_mt[kk] & RAND_UPPER_MASK) | (m_mt[kk+1] & RAND_LOWER_MASK );
					m_mt[kk] = m_mt[kk+RAND_M] ^ (y >> 1 ) ^ mag01[y & 0x1];
				}

				for ( ; kk < RAND_N - 1; ++kk ) 
				{
					y = ( m_mt[kk] & RAND_UPPER_MASK ) | (m_mt[kk+1] & RAND_LOWER_MASK);
					m_mt[kk] = m_mt[kk+(RAND_M-RAND_N)] ^ (y >> 1) ^ mag01[y & 0x1];
				}

				y = (m_mt[RAND_N-1] & RAND_UPPER_MASK) | (m_mt[0] & RAND_LOWER_MASK);
				m_mt[RAND_N-1] = m_mt[RAND_M-1] ^ (y >> 1) ^ mag01[y & 0x1];

				m_mti = 0;
			}

			y = m_mt[ m_mti++ ];
			y ^= RAND_TEMPERING_SHIFT_U( y );
			y ^= RAND_TEMPERING_SHIFT_S( y ) & RAND_TEMPERING_MASK_B;
			y ^= RAND_TEMPERING_SHIFT_T( y ) & RAND_TEMPERING_MASK_C;
			y ^= RAND_TEMPERING_SHIFT_L( y );

			return (y%n);
		}	

		//Signed integer within the specified range
		k_Int32 Random::random( k_Int32 minimum, k_Int32 maximum )
		{
			return random( (k_UInt32)maximum + (k_UInt32)abs(minimum) ) - minimum;
		}

		//Returns a float in the range 0 to 1 (excluding 1)
		k_Float32 Random::random()
		{
			return (k_Float32)random( kc_RandomMax ) / (k_Float32)kc_RandomMax;
		}

		//float within a specified range
		k_Float32 Random::random( k_Float32 minimum, k_Float32 maximum )
		{
			return ( random() * (maximum - minimum) ) + minimum; 
		}
	
		//float64 within a specified range
		k_Float64 Random::random( k_Float64 minimum, k_Float64 maximum )
		{
			return ( random() * (maximum - minimum) ) + minimum; 
		}

		//Seed the random number generator
		void Random::setSeed( k_UInt32 n )
		{
			/* setting initial seeds to mt[n] using the generator Line 25 table 1 in
			[KNUTH 1981, The art of COmputer Programming Vol. 2 (snd Ed.), pp102] */
			m_mt[0] = n & 0xffffffff;
			for ( m_mti = 1; m_mti < RAND_N; ++m_mti )
				m_mt[ m_mti] = (69069 * m_mt[ m_mti-1]) & 0xffffffff;

			m_seed = n;
		}

		void Random::randomize() 
		{
			setSeed( (k_UInt32)time(0) ); 
		}

	} //END: frx
} //END: kismo
