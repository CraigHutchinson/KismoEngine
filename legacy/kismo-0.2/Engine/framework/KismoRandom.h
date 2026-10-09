/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoRandom.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RANDOM_H
#define	_KISMO_RANDOM_H

#include "framework/KismoTypes.h"

//@ game coding complete (first edition) by Mike McShaffry, p85

//Period paramaters
#define RAND_N 624

namespace kismo
{
	namespace frx
	{		
		//-----------------------------------------------------
		// Random number generator
		//-----------------------------------------------------
		class Random
		{
		public:

			static const k_UInt32 kc_RandomMax = kc_UInt32_MAX;

		public:
			Random();

			//Returns a number from 0 to n (excluding n)
			k_UInt32 random( k_UInt32 n );	

			//Signed integer within the specified range
			k_Int32 random( k_Int32 minimum, k_Int32 maximum );

			//Returns a float in the range 0 to 1 (excluding 1)
			k_Float32 random();

			//float within a specified range
			k_Float32 random( k_Float32 minimum, k_Float32 maximum);
			k_Float64 random( k_Float64 minimum, k_Float64 maximum);
			
			//Seed the random number generator
			void setSeed( k_UInt32 n );
			k_UInt32 getSeed() const {return m_seed;}

			//Set a random seed (time based seed)
			void randomize();

		private:

			k_Int32 m_seed; //The seed for the generator
			k_UInt32 m_mt[ RAND_N ]; //Random number state vector
			k_Int32 m_mti; //RAND_N + 1 = un-initialised

		};

	} //END: frx
} //END: kismo

#endif