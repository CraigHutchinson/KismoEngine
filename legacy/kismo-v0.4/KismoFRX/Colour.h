/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Colour.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Colour.h (same name, content differs)
 */
#ifndef KISMOFRX_COLOUR_H
#define KISMOFRX_COLOUR_H

#include "KismoFRX/Common.h"
#include "KismoFRX/Math.h"

namespace kismo
{
	/** \brief Colour3 holds an RGB (red, green, blue ) colour
	*/
	class KISMOFRX_EXPORT Colour3
	{
	public:
			
		//Special Colours
		static const Colour3 ZERO;
		static const Colour3 Black;
		static const Colour3 White;
		static const Colour3 Red;
		static const Colour3 Green;
		static const Colour3 Blue;
		static const Colour3 Cyan;
		static const Colour3 Magenta; 
		static const Colour3 Yellow;

	public:

		Float32 r, g, b;

		Colour3( Float32 red = 1.0f,  Float32 green = 1.0f, Float32 blue = 1.0f) : 
			r( red ), g( green ), b( blue )
		{}

		/** \brief String constructor 
		*/
		Colour3( const String& str )
		{ *this = str; }

		inline Colour3& operator = ( const UInt32 c ) 
		{ 
			setRgba( c ); 
			return *this; 
		}

		// arithmetic operations
		inline Colour3 operator + ( const Colour3& c) const 
		{ return Colour3( r + c.r, g + c.g, b + c.b ); }

		inline Colour3 operator - ( const Colour3& c) const 
		{ return Colour3( r - c.r, g - c.g, b - c.b ); }

		inline Colour3 operator * ( const Colour3& c) const 
		{ return Colour3( r * c.r, g * c.g, b * c.b ); }

		inline Colour3 operator / ( const Colour3& c) const 
		{  return Colour3( c.r / r, c.g / g, c.b / b ); }
  		
		inline Colour3 operator + ( const float f ) const 
		{ return Colour3( r + f, g + f, b + f ); }

		inline Colour3 operator - ( const float f ) const 
		{ return Colour3( r - f, g - f, b - f ); }

		inline Colour3 operator * ( const float f ) const 
		{ return Colour3( r * f, g * f, b * f ); }

		inline Colour3 operator / ( const float f ) const
		{
			assert( f != 0.0f );
			Float32 fInv = 1.0f / f;	
			return Colour3( r * fInv,g * fInv,b * fInv);
		}

		inline Colour3& operator += ( const Colour3& c ) 
		{ r += c.r; g += c.g; b += c.b; return (*this); }

		inline Colour3& operator -= ( const Colour3& c ) 
		{ r -= c.r;  g -= c.g; b -= c.b; return (*this); }

		inline Colour3& operator *= (const float f )
		{ r *= f; g *= f; b *= f; return (*this); }

		inline Colour3& operator /= (const float f )
		{
			assert( f != 0.0f );
			Float32 fInv = 1.0f / f;
			r *= fInv; g *= fInv; b *= fInv;
			return (*this);
		}
		
		//Clamps the Colour3 wihtin the [0 to 1] range
		void saturate(void)
		{
			clamp( r, 0.0f, 1.0f);
			clamp( g, 0.0f, 1.0f);
			clamp( b, 0.0f, 1.0f);
		}

		//Convert the Colour3 into a packed 32bit Colour3
		UInt32 asRgba(void) const
		{
			assert( sizeof( UInt32 ) == sizeof(UInt32 ) ); //Make sure types match!
			UInt32 tVal = 0;	
			tVal += (UInt8)(r * 255) << 24;
			tVal += (UInt8)(g * 255) << 16;
			tVal += (UInt8)(b * 255) << 8;
			return tVal;
		}

		//sets the Colour3 from a packed 32bit Colour3
		void setRgba(const UInt32 c )// Convert from a packed 32bit Colour3
		{
			assert( sizeof( UInt32 ) == sizeof(UInt32 ) ); //Make sure types match!
			const Float32 fInv = 1.0f / 255.0f;
			r = (UInt8)(c >> 24) * fInv;
			g = (UInt8)(c >> 16) * fInv;
			b = (UInt8)(c >> 8) * fInv;	
		}

		/** \brief String assignment
		*/
		Colour3& operator =( const String& str );

		//Stream Operator
		//inline friend OutStream& operator << ( OutStream& outStream, const Colour3& c )
		//{
		//	outStream << c.r << _T("\t") << c.g << _T("\t") << c.b;
		//	return outStream;
		//}
		//inline friend InStream& operator >> ( InStream& inStream, Colour3& c )
		//{
		//	inStream >> (Float32)c.r >> (Float32)c.g >> (Float32)c.b;
		//	return inStream; //Allow cin >> c >> c >> c
		//}

	protected:

	};
	
	/** \brief Colour4 holds an RGBA (red, green, blue, alpha ) colour
	*/
	class KISMOFRX_EXPORT Colour4
	{
	public:
			
		//Special Colours
		static const Colour4 ZERO;
		static const Colour4 Black;
		static const Colour4 White;
		static const Colour4 Red;
		static const Colour4 Green;
		static const Colour4 Blue;
		static const Colour4 Cyan;
		static const Colour4 Magenta; 
		static const Colour4 Yellow;

	public:

		Float32 r, g, b, a;

		Colour4( Float32 red = 1.0f,  Float32 green = 1.0f, Float32 blue = 1.0f, Float32 alpha = 1.0f ) : 
			r( red ), g( green ), b( blue ), a( alpha )
		{}

		/** \brief String constructor 
		*/
		Colour4( const String& str )
		{ *this = str; }

		operator const Float32* () const
		{ return &r; }

		inline Colour4& operator = ( const UInt32 c ) 
		{ 
			setRgba( c ); 
			return *this; 
		}

		// arithmetic operations
		inline Colour4 operator + ( const Colour4& c) const 
		{ return Colour4( r + c.r, g + c.g, b + c.b, a + c.a ); }

		inline Colour4 operator - ( const Colour4& c) const 
		{ return Colour4( r - c.r, g - c.g, b - c.b, a - c.a ); }

		inline Colour4 operator * ( const Colour4& c) const 
		{ return Colour4( r * c.r, g * c.g, b * c.b, a * c.a ); }

		inline Colour4 operator / ( const Colour4& c) const 
		{  return Colour4( c.r / r, c.g / g, c.b / b, c.a / a ); }
  		
		inline Colour4 operator + ( const float f ) const 
		{ return Colour4( r + f, g + f, b + f, a + f ); }

		inline Colour4 operator - ( const float f ) const 
		{ return Colour4( r - f, g - f, b - f, a - f ); }

		inline Colour4 operator * ( const float f ) const 
		{ return Colour4( r * f, g * f, b * f, a * f ); }

		inline Colour4 operator / ( const float f ) const
		{
			assert( f != 0.0f );
			Float32 fInv = 1.0f / f;	
			return Colour4( r * fInv,g * fInv,b * fInv,a * fInv);
		}

		inline Colour4& operator += ( const Colour4& c ) 
		{ r += c.r; g += c.g; b += c.b; a += c.a; return (*this); }

		inline Colour4& operator -= ( const Colour4& c ) 
		{ r -= c.r;  g -= c.g; b -= c.b; a -= c.a; return (*this); }

		inline Colour4& operator *= (const float f )
		{ r *= f; g *= f; b *= f; a *= f; return (*this); }

		inline Colour4& operator /= (const float f )
		{
			assert( f != 0.0f );
			Float32 fInv = 1.0f / f;
			r *= fInv; g *= fInv; b *= fInv; a *= fInv;
			return (*this);
		}
		
		//Clamps the Colour4 wihtin the [0 to 1] range
		void saturate(void)
		{
			clamp( r, 0.0f, 1.0f);
			clamp( g, 0.0f, 1.0f);
			clamp( b, 0.0f, 1.0f);
			clamp( a, 0.0f, 1.0f);
		}

		//Convert the Colour4 into a packed 32bit Colour4
		UInt32 asRgba(void) const
		{
			assert( sizeof( UInt32 ) == sizeof(UInt32 ) ); //Make sure types match!
			UInt32 tVal = 0;	
			tVal += (UInt8)(r * 255) << 24;
			tVal += (UInt8)(g * 255) << 16;
			tVal += (UInt8)(b * 255) << 8;
			tVal += (UInt8)(a * 255);
			return tVal;
		}

		//sets the Colour4 from a packed 32bit Colour4
		void setRgba(const UInt32 c )// Convert from a packed 32bit Colour4
		{
			assert( sizeof( UInt32 ) == sizeof(UInt32 ) ); //Make sure types match!
			const Float32 fInv = 1.0f / 255.0f;
			r = (UInt8)(c >> 24) * fInv;
			g = (UInt8)(c >> 16) * fInv;
			b = (UInt8)(c >> 8) * fInv;	
			a = (UInt8)(c) * fInv;
		}

		/** \brief String assignment
		*/
		Colour4& operator =( const String& str );

		//Stream Operator
		/*inline friend OutStream& operator << ( OutStream& outStream, const Colour4& c )
		{
			outStream << c.r << _T("\t") << c.g << _T("\t") << c.b << _T("\t") << c.a;
			return outStream;
		}
		inline friend InStream& operator >> ( InStream& inStream, Colour4& c )
		{
			inStream >> (Float32)c.r >> (Float32)c.g >> (Float32)c.b >> (Float32)c.a;
			return inStream; //Allow cin >> c >> c >> c
		}*/

	protected:

	};

} // END: frx


#endif
