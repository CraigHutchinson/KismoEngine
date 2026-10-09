/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\SizeN.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/SizeN.h
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_SIZE_H
#define KISMOFRAMEWORK_SIZE_H

#include "KismoFramework/VectorN.h"

namespace kismo
{
	template <typename _Real, Count _Dim >
	struct SizeNBase
	{
		_Real _vec[_Dim]; //, Data array for number for diemsions of vector
	};
	
#pragma pack(push, 1)
	template <typename _Real> struct SizeNBase<_Real,1> { union { struct { _Real w; };			_Real _vec[1]; }; };
	template <typename _Real> struct SizeNBase<_Real,2> { union { struct { _Real w, h; };		_Real _vec[2]; }; };
	template <typename _Real> struct SizeNBase<_Real,3> { union { struct { _Real w, h, d; };	_Real _vec[3]; }; };
	template <typename _Real> struct SizeNBase<_Real,4> { union { struct { _Real w, h, d, t; };	_Real _vec[4]; }; };
#pragma pack(pop)

	/**
		\brief N dimensional size 
	*/
	template <typename _Real, int _Dim >
	class SizeT : public VectorN< _Real, _Dim, SizeNBase<_Real,_Dim> >
	{
	public:
		SizeT(){}
		SizeT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		operator const VectorN< _Real, _Dim >& () const { return *this; }
		operator VectorN< _Real, _Dim >& () { return *this; }
	};

	
	/**
		\brief 2 dimensional vector 
	*/
	template <typename _Real >
	class SizeT< _Real, 2 > : public VectorN< _Real, 2, SizeNBase<_Real,2> >
	{
	public:
		SizeT(){}
		SizeT( _Real width ) { w = width; }
		SizeT( _Real width, _Real height ) { w = width; h = height; }
		SizeT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		//operator const VectorN< _Real, 2 >& () const { return *this; }
		//operator VectorN< _Real, 2 >& () { return *this; }
	};
	
	/**
		\brief 3 dimensional vector 
	*/
	template <typename _Real >
	class SizeT< _Real, 3 > : public VectorN< _Real, 3, SizeNBase<_Real,3> >
	{
	public:
		SizeT(){}
		SizeT( _Real width ) { w = width; }
		SizeT( _Real width, _Real height ) { w = width; h = height; }
		SizeT( _Real width, _Real height, _Real depth ) { w = width; h = height; d = depth; }
		SizeT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }
	};

	/**
		\brief 3 dimensional vector 
	*/
	template <typename _Real >
	class SizeT< _Real, 4 > : public VectorN< _Real, 4, SizeNBase<_Real,4> >
	{
	public:
		SizeT(){}
		SizeT( _Real width ) { w = width; }
		SizeT( _Real width, _Real height ) { w = width; h = height; }
		SizeT( _Real width, _Real height, _Real depth ) { w = width; h = height; d = depth; }
		SizeT( _Real width, _Real height, _Real depth, _Real ntime ) { w = width; h = height; d = depth; t = ntime; }
		SizeT( const VectorN& rhs ) { ((VectorN&)*this) = rhs; }

		//operator const VectorN< _Real, 4 >& () const { return *this; }
		//operator VectorN< _Real, 4 >& () { return *this; }
	};

	typedef SizeT< Int32, 2 > Size2; ///< Default
	typedef SizeT< Int16, 2 > Size2s;
	typedef SizeT< Int32, 2 > Size2i;
	typedef SizeT< UInt16, 2 > Size2us;
	typedef SizeT< UInt32, 2 > Size2ui;
	typedef SizeT< Real, 2 > Size2r;
	typedef SizeT< Float32, 2 > Size2f;
	typedef SizeT< Float64, 2 > Size2d;
	
	typedef SizeT< Int32, 3 > Size3; ///< Default
	typedef SizeT< Int16, 3 > Size3s;
	typedef SizeT< Int32, 3 > Size3i;
	typedef SizeT< UInt16, 3 > Size3us;
	typedef SizeT< UInt32, 3 > Size3ui;
	typedef SizeT< Real, 3 > Size3r;
	typedef SizeT< Float32, 3 > Size3f;
	typedef SizeT< Float64, 3 > Size3d;
	
} // END: math

#endif