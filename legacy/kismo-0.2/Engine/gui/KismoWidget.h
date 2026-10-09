/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoWidget.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_IWIDGET_H
#define	_KISMO_GUI_IWIDGET_H

#include "gui\KismoGUI.h"
#include "gui\KismoSlotWidget.h"
#include "framework/KismoRtti.h"
#include "framework/KismoTreeNode.h"
using kismo::frx::Vector2r;

namespace kismo
{
	namespace gui
	{	
		//forward declaration
		class GUIRenderer;

		class _KismoExport Widget : public frx::TreeNode< Widget >, public SlotWidget
		{
			KISMO_DECLARE_RTTI_BASE
		public:

			enum ke_WidgetStyle
			{
				kc_WidgetStyle_Window, //The class for a window.
				kc_WidgetStyle_RenderBox, //The class for a render target area/window.
				kc_WidgetStyle_Button, //The class for a button. 
				kc_WidgetStyle_ComboBox, //The class for a combo box. 
				kc_WidgetStyle_Edit, //The class for an edit control. 
				kc_WidgetStyle_ListBox, //The class for a list box. 
				kc_WidgetStyle_HScrollBar, //The class for a scroll bar. 
				kc_WidgetStyle_VScrollBar, //The class for a scroll bar.
				kc_WidgetStyle_Label, //The class for a static control.
				kc_WidgetStyle_Frame, //The class for a static control.
				kc_WidgetStyle_Group, //The class for a static control.
			};

			struct _KismoExport WidgetDefinition
			{
				ke_WidgetStyle style;
				
				k_Bool visible; //Whether the item is shown
				k_Bool enabled; //Whether the item is enabled
				k_Bool toggled; //If the item is toggled (Checkbox)
				k_Bool editable; //If the item can be used but no edited
				k_Bool resizeable; //If the widget can be resized

				k_String name; //Name used to reference the widget
				k_String text; //Text on the control
				k_Handle cursor; //Handle to the cursor to use (0=default)
				k_Handle icon; //Handle to an icon for the widget (0=default)
				k_UInt16 tabIndex; //The tab index
				k_RGBA32 background; //The background colour
				Vector2r size; //size of the widget
				Vector2r position; //size of the widget
				
				enum ke_DockStyle
				{
					kc_DockStyle_None,
					kc_DockStyle_Left,
					kc_DockStyle_Right,
					kc_DockStyle_Top,
					kc_DockStyle_Bottom,
					kc_DockStyle_Center,
				};
				ke_DockStyle dock; //Docking
			};

		public:

			Widget( Widget* parent );	
			virtual ~Widget();

			WidgetDefinition& getDefinition() { return m_definition; }
			const WidgetDefinition& getDefinition() const { return m_definition; }

			virtual void create(); //Create the instance (Do not show
			virtual void exec(); //Create and show the widget
			DEFINE_SLOT( show ); //Show the widget instance
			DEFINE_SLOT( hide ); //Hide the widget
			void destroy(); //Removes the widget instance
			void setName( const k_String& name ); //Set the identifying name for the widget
			void setText( const k_String& text ); //Set the text/title of the widget
			void setSize( const Vector2r& size ); //Set the dimensions of the widget
			void setPosition( const Vector2r& position ); //Set the position of the widgets top-left
			
			const k_String& getName() { return m_definition.name; }
			const k_String& getText() { return m_definition.text; }

			void setGuiRenderer( GUIRenderer* r ) 
			{
				bool created = isValidPointer( m_guiRenderer );
				if ( created )
					destroy();
				
				m_guiRenderer = r;

				if ( created )
					create();
			}

			//Get a pointer to the parent
			GUIRenderer* guiRenderer() 
			{ 
				if ( isValidPointer( m_guiRenderer ) )
					return m_guiRenderer;
				else
				if ( isValidPointer( getParent() ) )
					return getParent()->guiRenderer();
				else
					return 0;
			}

			RECIEVE_SLOT_FUNC
			{
				EXEC_SLOT( show );
				EXEC_SLOT( hide );
				assert( false == "recieveingSlot's handle is not handled by this widget!"  ); //Testing for now
				return kc_FALSE; //No parent to send msg to
			}

		protected:
			
			
			virtual void beforeParentChanged();
			virtual void onParentChanged();
		
		private:

			WidgetDefinition m_definition; //The definition/state

			GUIRenderer* m_guiRenderer; //The renderer for this widget( 0 = use parent );
		};

	} // END: gui

} // END: kismo

#endif
