/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Window.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Window.h (same name, content differs)
 */
#ifndef WINDOW_H
#define WINDOW_H

#include "Framework/Types.h"
#include "Framework/Rect.h"
#include "Framework/Named.h"
#include "Framework/SmartPointer.h"
#include "Framework/EventManager.h"

namespace frx
{
	/** \brief mouse input event
	*/
	class _DllExport MouseEvent : public Event
	{
		DECLARE_RTTI;
		MouseEvent() : Event( _EventManagerPtr ), absolutePos( -1, -1 ), relativePos( 0, 0 ){}
		BitSet< 5 > button;
		frx::Vector2i absolutePos;
		frx::Vector2i relativePos;
	};

	/** \brief Key press event
	*/
	class _DllExport KeyEvent : public Event
	{
		DECLARE_RTTI;
		KeyEvent() : Event( _EventManagerPtr ), keyCode( 0 ), pressed( false ) {}
		UInt8 keyCode; //< The code for the key that was pressed  or released
		bool pressed; //< Whether the kley is pressed or released
	};

	/** \brief Window resize event 
	*/
	class _DllExport ResizeEvent : public Event
	{
		DECLARE_RTTI;
		ResizeEvent() : Event( _EventManagerPtr ), size( frx::Rect2s::Size(0,0) ) {}
		frx::Rect2s::Size size; //< The new size of the window
	};

	/** \brief Window resize event 
	*/
	class _DllExport MoveEvent : public Event
	{
		DECLARE_RTTI;
		MoveEvent() : Event( _EventManagerPtr ), pos( frx::Rect2s::Position(0,0) ) {}
		frx::Rect2s::Position pos; //< The new size of the window
	};

	/** \brief Window defines the base interface for a window in the operating system */
	class _DllExport Window : public Named, public frx::SmartReferenced
	{
	public:

		static Window* create( const Char* name, frx::Rect2s rect = frx::Rect2s( 0, 0, 400, 300 ) );
		
		/** \brief Pointer for automatic lifetime managment
		*/
		typedef frx::SmartPointer< Window > Ptr;

	public:
		
		Window( const String& name, Rect2s rect = Rect2s( 0, 0, 400, 300 ) ) :
			Named( name ),
			m_rect( rect ),
			m_visible( false )
		{}

		virtual ~Window() 
		{}
		
		/** \brief Create the window if necessary
		*/
		virtual Bool create() = 0;

		/** \brief Update the window
		*/
		virtual Bool update() = 0;

		/** \brief Show the window
		\remarks Will create the window if necessary
		*/
		Bool show() { return setVisible( true ); }
		
		/** \brief Set the rectangle and show the window
		\remarks Will create the window if necessary
		\param[in] rect 
		*/
		Bool show( Rect2s rect ) 
		{ return setRect( rect ) && setVisible( true ); }

		/** \brief Hides the window
		*/
		Bool hide() 
		{ return setVisible( false ); }
		
		/** \brief Shows or hides the window depending
		\param[in] visible True will show the window, false will hide it
		*/
		virtual Bool setVisible( Bool visible ) = 0;

		/** \brief get whether the window is visible
		*/
		Bool isVisible() const
		{ return m_visible; }
		
		/** \brief Sets the windows rect
			\param[in] rect The new rectangle for the window
		*/
		virtual Bool setRect( Rect2s rect ) = 0;

		/** \brief Sets the window size
			\param[in] size The new size (h/w) for the window
		*/
		virtual Bool setSize( Rect2s::Size size )
		{ Rect2s trect = getRect(); trect.setSize( size ); return setRect( trect ); }

		/* Get the rectangle for the window 
		*/
		const Rect2s& getRect() const
		{ return m_rect; }

		/* Get the dinwo handle
		*/		
		virtual void* getContextHandle()= 0;

		/** \brief add a child dialog context handle
			\remarks This is used in systems where messages need to be passed to child contexts
			//TODO: This is temporary ubntil there is a proper dialog implementation
		*/
		void addChildContextHandle( void* context )
		{ m_childContexts.insert( context ); }

		void removeChildContextHandle( void* context )
		{
			ContextArray::iterator iFind = m_childContexts.find( context ); 
			if ( iFind != m_childContexts.end() )
				m_childContexts.erase( iFind );
		}

		/** \brief Window events */
		MouseEvent& onMouse() { return m_mouseEvent; }
		KeyEvent& onKey() { return m_keyEvent; }
		ResizeEvent& onResize() { return m_resizeEvent; }
		MoveEvent& onMove() { return m_moveEvent; }

	protected:
		Rect2s m_rect; ///< Rectangle defining the windows size and position
		Bool m_visible; ///< Whether the window is visible
		MouseEvent m_mouseEvent; //< THe mouse event
		KeyEvent m_keyEvent; //< THe key event
		ResizeEvent m_resizeEvent; //< The window size event
		MoveEvent m_moveEvent; //< The window move event


		//TODO: This is temporary ubntil there is a proper dialog implementation
		typedef frx::SmallSet< void* > ContextArray;
		ContextArray m_childContexts; ///< Array of child contexts
	};


} //END: gfx

#endif