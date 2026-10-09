/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoWidget.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoWidget.h"
#include "gui\KismoGuiRenderer.h"

namespace kismo
{
	namespace gui
	{	
		const frx::Rtti Widget::TYPE( _T("gui.Widget"), 0 );

		Widget::Widget( Widget* parent ) : 
			frx::TreeNode< Widget >(),
			m_guiRenderer( 0 )
		{
			setParent( parent );
		}
		
		Widget::~Widget()
		{
			destroy();
		}

		void Widget::beforeParentChanged()
		{
			//destroy();
		}

		void Widget::onParentChanged()
		{
			//create();
		}

		void Widget::exec()
		{ 
			create(); 

			//Execute any children
			Widget* child = 0;
			while ( child = nextChild( child == 0 ) )
			{
				child->exec();
			}

			show(); 
		}
		void Widget::create()
		{
			guiRenderer()->sendMessage(new GUIMsg_CreateWidget( this ) ); 
		}

		void Widget::destroy()
		{ 
			assert( guiRenderer() );
			guiRenderer()->sendMessage( new GUIMsg_DestroyWidget( this ) ); 
		}

		IMPL_SLOT( Widget, show ) 
		{ 
			guiRenderer()->sendMessage( new GUIMsg_ShowWidget( this ) ); 
		}

		IMPL_SLOT( Widget, hide ) 
		{
			guiRenderer()->sendMessage(new GUIMsg_HideWidget( this ) ); 
		}

		void Widget::setName( const k_String& name )
		{ 
			//_Gui.setName( this, name );
		}

		void Widget::setText( const k_String& text )
		{ 
			guiRenderer()->sendMessage(new GUIMsg_SetWidgetText( this, text ) );
		}

		void Widget::setSize( const Vector2r& size ) 
		{ 
			guiRenderer()->sendMessage(new GUIMsg_SizeWidget( this, size ) );
		}

		void Widget::setPosition( const Vector2r& position ) 
		{ 
			guiRenderer()->sendMessage(new GUIMsg_PositionWidget( this, position ) );
		}

	}
}