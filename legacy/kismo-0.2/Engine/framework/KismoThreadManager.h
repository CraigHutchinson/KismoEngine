/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoThreadManager.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_THREAD_MANAGER_H
#define _KISMO_THREAD_MANAGER_H

#include "framework/KismoThread.h"

namespace kismo
{
	namespace frx
	{	
		/** ThreadManager maintains the creation, termination and processing priority of 
		concurrent code sequences. */
		class _KismoExport ThreadManager
		{
		public:

			/** Constructor 
			\param[in] Specifies whether multi-threading is to be used */
			ThreadManager();
			
			/** \brief Destructor sends a terminate signal to all running threads */
			~ThreadManager();

			/** \brief Creates a new thread
			\param[in] name Specifies the name used to identify the thread (optional)
			\return Returns a pointer to the new thread. 
			\return If a thread of the same name exists or creation failed a 0 will be returned */
			Thread* create( const k_String& name = "" );

			/** \brief Finds a thread of the specified name
			\param[in] name Specifies the name of the thread to find
			\return Returns a pointer to the existing thread or 0 if no thread exists */
			Thread* find( const k_String& name );
			
			/** \brief Pauses/Un-pauses a thread by name
			\param[in] name Specifies the name of the thread to find
			\return Whether the thread was found and paused/un-paused successfully */
			k_Bool setPaused( const k_String& name, bool frozen );
			{ 
				Thread* t = find( name ); 
				return ( isValidPointer( t ) ? t->setPaused( frozen ) : false ); 
			}

			/** \brief Stops a thread by name sending it a terminate message and waiting for it to finish
			and free any memory it is using
			\param[in] name Specifies the name of the thread to find
			\return Whether the thread was found and terminated successfully */
			k_Bool stop( const k_String& name )
			{ 
				Thread* t = find( name ); 
				return ( isValidPointer( t ) ? t->stop() : false ); 
			}

			/** \brief Post a message to a threads queue for procesing at a later date */
			virtual bool postMessage( const k_String& name, Message::Ptr msg ) 
			{ 
				Thread* t = find( name ); 
				return ( isValidPointer( t ) ? t->postMessage( msg ) : false );		
			}

			/** \brief Sends a message to a threads waiting for it to be processed before returning */
			virtual bool sendMessage( const k_String& name, Message::Ptr msg ) 
			{ 
				Thread* t = find( name ); 
				return ( isValidPointer( t ) ? t->sendMessage( msg ) : false );		
			}

		public:

			

			//Starts the thread
			k_Bool start();

			//Stops the thread
			void stop();

			//Set the thread priority onces its running
			void setThreadPriority( ke_ThreadPriority priority = kc_ThreadPriority_Normal );

			//Wait for the thread to complete
			void waitForThreadTerminate( k_UInt32 time = kc_UInt32_MAX );

			//Inform the thread to terminate prematurely
			// NOTE: Will only work if the run() function checks for shouldTerminate()
			// - if the teminateTimeout is reached the thread will then be 'forced' to terminate
			// forced = Closes the thread stopping execution wherever it was in the code! NOT RECOMMENDED
			void terminateThread( k_Bool forced = false );

			///Check if the thread has finished running its process
			k_Bool isThreadFinished() const;

			//Check if the thread is running
			k_Bool isThreadRunning() const; 

			//Check whether this thread is the current one we are running in
			k_Bool isCurrentThread() const;
			


		protected:

			//process a message
			virtual void processMessage( Message::Ptr& msg ) {};
			
			//Does the calculations of the thread
			// NOTE: Should check for shouldTerminate() in order to accept clean termination
			// - use setTerminateTimeout( TIME ) to change how long to wait before a terminate is forced
			virtual	void run() = 0;

			virtual	void onstart(){};
			virtual	void onTerminate(){};

			//Checks if the thread should terminate
			k_Bool shouldThreadTerminate();

			//Set the amount of time to wait before terminate is forced
			// This should be a time in milliseconds larger than the running loops execution time
			void setThreadTerminateTimeout( k_UInt32 milliseconds );

			/** \brief The default message handler */
			bool defaultMessageHandler( Message::Ptr& msg );

		private:

			//forward declaration
			struct Impl;
			Impl* m_impl;

			k_Handle m_threadHandle; //The threads handle (NOTE: For internal use and is not the win32 handle)

			//Static thread process distrubutes thread process to the class instance
			static k_Int32 staticThreadProcess( void* data );

		};
	} //END: frx
} //END: kismo

#endif