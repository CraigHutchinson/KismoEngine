/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Rect.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Rect.h (same name, content differs)
 */
#ifndef FRX_RECT_H
#define FRX_RECT_H

#include "Framework/Vector.h"
#include "Framework/Size.h"

namespace frx
{

	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center
	*/
	template< typename T > 
	class _DllExport Rect2
	{
	public:
		typedef Vector< Rect2 > Array;
		typedef Size2< T > Size;
		typedef Vector2< T > Position;

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect2() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect2( const T left, const T top, const T right, const T bottom );

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
		{ m_center = pos; m_center.x -= (m_size.width * (T)0.5); m_center.y -= (m_size.height * (T)0.5); }

		/** \brief Gets the size of the rect 
		*/
		const Size& getSize() const
		{ return m_size; }
	
		/** \brief Sets the size of the rect 
		*/
		void setSize( const Size& size )
		{ m_size = size; }

		/** \brief Gets the width of the rect 
		*/
		T width() const
		{ return m_size.width; }
	
		/** \brief Gets the height of the rect 
		*/
		T height() const
		{ return m_size.height; }

		/** \brief Gets the coordinate of the left
		*/
		T left() const
		{ return m_center.x - (m_size.width * (T)0.5); }

		/** \brief Gets the coordinate of the top
		*/
		T top() const
		{ return m_center.y - (m_size.height * (T)0.5); }

		/** \brief Gets the coordinate of the right
		*/
		T right() const
		{ return m_center.x + (m_size.width * (T)0.5); }

		/** \brief Gets the coordinate of the bottom
		*/
		T bottom() const
		{ return m_center.y + (m_size.height * (T)0.5); }

	private:	
		Position m_center; ///< Center of the rectangle
		Size m_size; ///< Size extents of the rectangle
	};
	
	/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center */
	template< typename T >
	class /*_DllExport*/ Rect3
	{
	public:

		typedef Vector< Rect3 > Array;
		typedef Size2< T > Size;
		typedef Vector3< T > Position;

	public:

		/** \brief Constructor does not initialise the rect for efficiency 
		*/
		Rect3() {}

		/** \brief Constructor takes 4 values for the rectangle sides 
		*/
		Rect3( const T left, const T top, const T right, const T bottom ) :
			m_center( Vector2< T >( (right+left) * (T)0.5, (bottom+top) * (T)0.5 ) ),
			m_size( Size2< T >( right-left, bottom-top ) ) {}

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
	
		/** \brief Gets the width of the rect 
		*/
		T width() const
		{ return m_size.width; }
	
		/** \brief Gets the height of the rect 
		*/
		T height() const
		{ return m_size.height; }

	private:		
		Position m_center; ///< Center of the rectangle
		Position m_axis[2]; ///< Unit vectors determinitng the local x and y axes for the rectangle
		Size m_size; ///< Size extents of the rectangle along the axes
	};

	typedef Rect2< Int16 > Rect2s;
	typedef Rect2< Int32 > Rect2i;
	typedef Rect2< Real > Rect2r;
	typedef Rect2< Float32 > Rect2f;
	typedef Rect2< Float64 > Rect2d;

	typedef Rect3< Int16 > Rect3s;
	typedef Rect3< Int32 > Rect3i;
	typedef Rect3< Real > Rect3r;
	typedef Rect3< Float32 > Rect3f;
	typedef Rect3< Float64 > Rect3d;


} //END: frx
#endif