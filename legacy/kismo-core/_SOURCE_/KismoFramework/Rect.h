/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Rect.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_RECT_H
#define KISMOFRAMEWORK_RECT_H

#include "KismoFramework/VectorN.h"
#include "KismoFramework/SizeN.h"

namespace kismo
{

	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a topLeft
	*/
	template< typename _Real > 
	class Rect2
	{
	public:
		typedef std::vector< Rect2 > Array;
		typedef SizeT<_Real,2> Size;
		typedef VectorT<_Real,2> Position;

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect2() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect2( const _Real left, const _Real top, const _Real width, const _Real height );

		/** \brief Constructor takes a size and topLeft 
		*/
		Rect2( const Position& topLeft, const Size& size ) :
			m_topLeft( topLeft ), m_size( size ) {}

		/** \brief Gets the topLeft of the rect 
		*/
		const Position& getTopLeft() const
		{ return m_topLeft; }

		/** \brief Sets the central position of the rect 
		*/
		void setCenter( const Position& center )
		{ m_topLeft = center; m_topLeft -= m_size/2; }

		/** \brief Sets the top left corner position of the rect 
		*/
		void setTopLeft( const Position& pos )
		{ m_topLeft = pos; }

		/** \brief Gets the size of the rect 
		*/
		const Size& getSize() const
		{ return m_size; }
	
		/** \brief Sets the size of the rect 
		*/
		void setSize( const Size& size )
		{ m_size = size; }

		/** \brief Gets the w of the rect 
		*/
		_Real width() const
		{ return m_size.w; }
	
		/** \brief Gets the h of the rect 
		*/
		_Real height() const
		{ return m_size.h; }

		/** \brief Gets the coordinate of the left
		*/
		_Real left() const
		{ return m_topLeft.x; }

		/** \brief Gets the coordinate of the top
		*/
		_Real top() const
		{ return m_topLeft.y; }

		/** \brief Gets the coordinate of the right
		*/
		_Real right() const
		{ return m_topLeft.x + m_size.w; }

		/** \brief Gets the coordinate of the bottom
		*/
		_Real bottom() const
		{ return m_topLeft.y + m_size.h; }

	private:	
		Position m_topLeft; ///< Center of the rectangle
		Size m_size; ///< Size extents of the rectangle
	};
	
	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a topLeft */
	template< typename _Real >
	class Rect3
	{
	public:

		typedef std::vector< Rect3 > Array;
		typedef SizeT< _Real, 2 > Size;
		typedef VectorT< _Real, 3 > Position;

	public:

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect3() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect3( const _Real left, const _Real top, const _Real right, const _Real bottom ) :
			m_topLeft( left, top ),
			m_size( right-left, bottom-top ) {}

		/** \brief Constructor takes a size and topLeft 
		*/
		Rect3( const Position& topLeft, const Size& size ) :
			m_topLeft( topLeft ), m_size( size ) {}

		/** \brief Gets the topLeft of the rect 
		*/
		inline const Position& topLeft() const
		{ return m_topLeft; }

		/** \brief Gets the size of the rect 
		*/
		const Size& size() const
		{ return m_size; }
	
		/** \brief Gets the w of the rect 
		*/
		_Real width() const
		{ return m_size.w; }
	
		/** \brief Gets the h of the rect 
		*/
		_Real height() const
		{ return m_size.h; }

	private:		
		Position m_topLeft; ///< Center of the rectangle
		Position m_axis[2]; ///< Unit vectors determinitng the local x and y axes for the rectangle
		Size m_size; ///< Size extents of the rectangle along the axes
	};

	typedef Rect2< Int16 > Rect2s;
	typedef Rect2< Int32 > Rect2i;
	typedef Rect2< UInt16 > Rect2us;
	typedef Rect2< UInt32 > Rect2ui;
	typedef Rect2< Real > Rect2r;
	typedef Rect2< Float32 > Rect2f;
	typedef Rect2< Float64 > Rect2d;

	typedef Rect3< Int16 > Rect3s;
	typedef Rect3< Int32 > Rect3i;
	typedef Rect3< UInt16 > Rect3us;
	typedef Rect3< UInt32 > Rect3ui;
	typedef Rect3< Real > Rect3r;
	typedef Rect3< Float32 > Rect3f;
	typedef Rect3< Float64 > Rect3d;


} //END: kismo
#endif