/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMD5.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_MD5_H
#define _KISMO_MD5_H

#include "framework/kismoCommon.h"

//@ Derived from the RSA Data Security, Inc. C_MD5 Message-Digest Algorithm

namespace kismo
{
	namespace frx
	{
		class MD5
		{
		public:
			// methods for controlled operation:
			MD5();  // simple initializer
			MD5( const k_String& string ); // digest string, finalise
			MD5( k_UByteIStream& stream ); // digest stream, finalise

			void update( k_UByte* input, k_Count input_length );
			void update( k_UByteIStream& stream );
			void finalise();

			// methods to acquire finalised result
			k_ByteArray raw() const; // digest as a 16-byte binary array
			k_String hex() const; // digest as a 33-byte ascii-hex string
			friend k_OStream& operator << ( k_OStream&, MD5 context );

		private:

			// next, the private data:
			k_UInt32 m_state[4];
			k_UInt32 m_count[2];     // number of *bits*, mod 2^64
			k_UInt8 m_buffer[64];   // input buffer
			k_UInt8 m_digest[16];
			k_UInt8 m_finalised;

			// last, the private methods, mostly static:
			void init(); // called by all constructors
			void transform( k_UInt8* buffer );  // does the real update work.  Note that length is implied to be 64.

			static void encode( k_UInt8 *dest, k_UInt32 *src, k_UInt32 length );
			static void decode( k_UInt32 *dest, k_UInt8 *src, k_UInt32 length );
			static void memcpy( k_UInt8 *dest, k_UInt8 *src, k_UInt32 length );
			static void memset( k_UInt8 *start, k_UInt8 val, k_UInt32 length );

			static inline k_UInt32 rotate_left(k_UInt32 x, k_UInt32 n);
			static inline k_UInt32 F(k_UInt32 x, k_UInt32 y, k_UInt32 z);
			static inline k_UInt32 G(k_UInt32 x, k_UInt32 y, k_UInt32 z);
			static inline k_UInt32 H(k_UInt32 x, k_UInt32 y, k_UInt32 z);
			static inline k_UInt32 I(k_UInt32 x, k_UInt32 y, k_UInt32 z);
			static inline void FF(k_UInt32& a, k_UInt32 b, k_UInt32 c, k_UInt32 d, k_UInt32 x, k_UInt32 s, k_UInt32 ac);
			static inline void GG(k_UInt32& a, k_UInt32 b, k_UInt32 c, k_UInt32 d, k_UInt32 x, k_UInt32 s, k_UInt32 ac);
			static inline void HH(k_UInt32& a, k_UInt32 b, k_UInt32 c, k_UInt32 d, k_UInt32 x, k_UInt32 s, k_UInt32 ac);
			static inline void II(k_UInt32& a, k_UInt32 b, k_UInt32 c, k_UInt32 d, k_UInt32 x, k_UInt32 s, k_UInt32 ac);
		};
	}
}

#endif