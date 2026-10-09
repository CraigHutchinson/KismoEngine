/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Rect.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Rect.h (same name, content differs)
 */
#ifndef KISMOFRX_RECT_H
#define KISMOFRX_RECT_H

#include "KismoFRX/VectorN.h"
#include "KismoFRX/SizeN.h"

namespace kismo
{

	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center
	*/
	template< typename _TReal > 
	class KISMOFRX_EXPORT Rect2
	{
	public:
		typedef Vector< Rect2 > Array;
		typedef SizeT<_TReal,2> Size;
		typedef VectorT<_TReal,2> Position;

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect2() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect2( const _TReal left, const _TReal top, const _TReal width, const _TReal height );

		/** \brief Constructor takes a size and center 
		*/
		Rect2( const Position& center, const Size& size ) :
			m_center( center ), m_size( size ) {}

		/** \brief Gets the center of the rect 
		*/
		const Position& getCenter() const
		{ return m_center; }

		/** \brief Sets the central position of the rect 
		*/
		void setCenter( const Position& center )
		{ m_center = center; }

		/** \brief Sets the top left corner position of the rect 
		*/
		void setTopLeft( const Position& pos )
		{ m_center = pos; m_center.x -= (m_size.w * (_TReal)0.5); m_center.y -= (m_size.h * (_TReal)0.5); }

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
		_TReal width() const
		{ return m_size.w; }
	
		/** \brief Gets the h of the rect 
		*/
		_TReal height() const
		{ return m_size.h; }

		/** \brief Gets the coordinate of the left
		*/
		_TReal left() const
		{ return m_center.x - (m_size.w * (_TReal)0.5); }

		/** \brief Gets the coordinate of the top
		*/
		_TReal top() const
		{ return m_center.y - (m_size.h * (_TReal)0.5); }

		/** \brief Gets the coordinate of the right
		*/
		_TReal right() const
		{ return m_center.x + (m_size.w * (_TReal)0.5); }

		/** \brief Gets the coordinate of the bottom
		*/
		_TReal bottom() const
		{ return m_center.y + (m_size.h * (_TReal)0.5); }

	private:	
		Position m_center; ///< Center of the rectangle
		Size m_size; ///< Size extents of the rectangle
	};
	
	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center */
	template< typename _TReal >
	class KISMOFRX_EXPORT Rect3
	{
	public:

		typedef Vector< Rect3 > Array;
		typedef SizeT< _TReal, 2 > Size;
		typedef VectorT< _TReal, 3 > Position;

	public:

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect3() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect3( const _TReal left, const _TReal top, const _TReal right, const _TReal bottom ) :
			m_center( Position( (right+left) * (_TReal)0.5, (bottom+top) * (_TReal)0.5, 0 ) ),
			m_size( Size( right-left, bottom-top ) ) {}

		/** \brief Constructor takes a size and center 
		*/
		Rect3( const Position& center, const Size& size ) :
			m_center( center ), m_size( size ) {}

		/** \brief Gets the center of the rect 
		*/
		inline const Position& center() const
		{ return m_center; }

		/** \brief Gets the size of the rect 
		*/
		const Size& size() const
		{ return m_size; }
	
		/** \brief Gets the w of the rect 
		*/
		_TReal width() const
		{ return m_size.w; }
	
		/** \brief Gets the h of the rect 
		*/
		_TReal height() const
		{ return m_size.h; }

	private:		
		Position m_center; ///< Center of the rectangle
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