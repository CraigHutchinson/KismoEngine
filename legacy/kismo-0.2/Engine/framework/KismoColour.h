/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoColour.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_Colour_H
#define	_KISMO_Colour_H

#include "framework/KismoCommon.h"

namespace kismo
{
	namespace frx
	{
		class _KismoExport Colour
		{
		public:
		
			//Specifies whether reading/writing to stream is done in packed 32bit integers
			static void Stream_AS_RGBA( k_Bool b = kc_TRUE ) { ms_useRGBAStreamMode = b; }
			
			//Special Colours
			static const Colour ZERO;
			static const Colour Black;
			static const Colour White;
			static const Colour Red;
			static const Colour Green;
			static const Colour Blue;
			static const Colour Cyan;
			static const Colour Magenta; 
			static const Colour Yellow;

		public:

			k_Float32 r, g, b, a;

			Colour( k_Float32 red = 1.0f,  k_Float32 green = 1.0f, k_Float32 blue = 1.0f, k_Float32 alpha = 1.0f ) : 
				r( red ), g( green ), b( blue ), a( alpha )
			{}

			inline Colour& operator = ( const k_RGBA32 c ) { setRGBA( c ); return *this; }

			// arithmetic operations
			inline Colour operator + ( const Colour& c) const { return Colour( r + c.r, g + c.g, b + c.b, a + c.a ); }
			inline Colour operator - ( const Colour& c) const { return Colour( r - c.r, g - c.g, b - c.b, a - c.a ); }
			inline Colour operator * ( const Colour& c) const { return Colour( r * c.r, g * c.g, b * c.b, a * c.a ); }
			inline Colour operator / ( const Colour& c) const {  return Colour( c.r / r, c.g / g, c.b / b, c.a / a ); }
	  		
			inline Colour operator + ( const float f ) const { return Colour( r + f, g + f, b + f, a + f ); }    
			inline Colour operator - ( const float f ) const { return Colour( r - f, g - f, b - f, a - f ); }
			inline Colour operator * ( const float f ) const { return Colour( r * f, g * f, b * f, a * f ); }
			inline Colour operator / ( const float f ) const
			{
				assert( f != 0.0f );
				k_Float32 fInv = 1.0f / f;	
				return Colour( r * fInv,g * fInv,b * fInv,a * fInv);
			}

			inline Colour& operator += ( const Colour& c ) { r += c.r; g += c.g; b += c.b; a += c.a; return (*this); }
			inline Colour& operator -= ( const Colour& c ) { r -= c.r;  g -= c.g; b -= c.b; a -= c.a; return (*this); }
			inline Colour& operator *= (const float f ) { r *= f; g *= f; b *= f; a *= f; return (*this); }
			inline Colour& operator /= (const float f )
			{
				assert( f != 0.0f );
				k_Float32 fInv = 1.0f / f;
				r *= fInv; g *= fInv; b *= fInv; a *= fInv;
				return (*this);
			}
			
			//Clamps the Colour wihtin the [0 to 1] range
			void saturate(void)
			{
				math::clamp( r, 0.0f, 1.0f);
				math::clamp( g, 0.0f, 1.0f);
				math::clamp( b, 0.0f, 1.0f);
				math::clamp( a, 0.0f, 1.0f);
			}

			//Convert the Colour into a packed 32bit Colour
			k_RGBA32 asRGBA(void) const
			{
				assert( sizeof( k_UInt32 ) == sizeof(k_RGBA32 ) ); //Make sure types match!
				k_UInt32 tVal = 0;	
				tVal += (k_UInt8)(r * 255) << 24;
				tVal += (k_UInt8)(g * 255) << 16;
				tVal += (k_UInt8)(b * 255) << 8;
				tVal += (k_UInt8)(a * 255);
				return tVal;
			}

			//sets the Colour from a packed 32bit Colour
			void setRGBA(const k_RGBA32 c )// Convert from a packed 32bit Colour
			{
				assert( sizeof( k_UInt32 ) == sizeof(k_RGBA32 ) ); //Make sure types match!
				const k_Float32 fInv = 1.0f / 255.0f;
				r = (k_UInt8)(c >> 24) * fInv;
				g = (k_UInt8)(c >> 16) * fInv;
				b = (k_UInt8)(c >> 8) * fInv;	
				a = (k_UInt8)(c) * fInv;
			}

			//Stream Operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Colour& c )
			{
				if ( ms_useRGBAStreamMode )
					outStream << "ColourValue[ " << c.asRGBA() << " ]";
				else
					outStream << "ColourValue[ " << c.r << " " << c.g << " " << c.b << " " << c.a << " ]";
				return outStream;
			}
			inline friend k_IStream& operator >> ( k_IStream& inStream, Colour& c )
			{
				if ( ms_useRGBAStreamMode )
					inStream >> c;
				else
					inStream >> (k_Float32)c.r >> (k_Float32)c.g >> (k_Float32)c.b >> (k_Float32)c.a;

				return inStream; //Allow cin >> c >> c >> c
			}

		protected:
			//Static args
			static k_Bool ms_useRGBAStreamMode; //Specifies whether reading/writing to stream is done in packed 32bits
		};
	
	} // END: frx

} // END: kismo

#endif
