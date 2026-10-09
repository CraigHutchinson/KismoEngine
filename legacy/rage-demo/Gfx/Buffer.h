/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Buffer.h
 * Status: duplicate of kismo-v0.3/Gfx/Buffer.h
 */
#ifndef GFX_BUFFER_H
#define GFX_BUFFER_H

#include "Framework/Types.h"
#include "Framework/SmartPointer.h"
#include "Gfx/BufferDefinition.h"

namespace gfx
{
	/** \brief Base buffer class for vertex or index buffer implimentations
	\remarks
		This buffer type can be ither an index or vertex buffer implimentation and is defined by the ePurpose
		property. This allows for a more flexible storage and no need to optimise seperate structures that stor
		basically the same information.
	*/
	class _DllExport Buffer : public frx::SmartReferenced
	{
	public:

		typedef frx::SmartPointer< Buffer > Ptr;
		typedef Vector< Buffer::Ptr > PtrArray;

		/** \brief Buffer purpose enumerator
		*/
		enum ePurpose
		{
			Purpose_Vertex, ///< vertex buffer, used in a renderer to draw 3d vertex positions for a mesh
			Purpose_Index, ///< Index buffer, used in a renderer to index the data inside a vertex buffer
			Purpose_Ext = 32, ///< Extended purpose i.e. custom buffer for other uses
		};

		/** \brief Buffer usage enumerator
		\remarks
			Based from D3D10 
		\remarks
			Used to define memory location and locking used for hardware supported buffers
		*/
		enum eUsage 
		{
			/** \brief Default access
			\remarks 
				A resource that requires read and write access by the GPU. This is likely 
				to be the most common usage choice. 
			*/
			Usage_Default, 	

			/** \brief Dynamic access
			\remarks
				A resource that is accessible by both the GPU (read only) and the 
				CPU (write only). A dynamic resource is a good choice for a resource that will 
				be updated by the CPU at least once per frame. 
			*/
			Usage_Dynamic, 

			/** \brief Immutable access
			\remarks 
				A resource that can only be read by the GPU. It cannot be written by the GPU,
				and cannot be accessed at all by the CPU. This type of resource must be 
				initialized when it is created, since it cannot be changed after creation. 
			*/
			Usage_Immutable,

			/** \brief Staging access
			\remarks
				A resource that supports data transfer (copy) from the GPU to the CPU. 
			*/
			//Usage_Staging, 
		};

		enum eLockFlag
		{
			LockFlag_ReadWrite  = 0, ///< Lock the buffer for reading and writting
			LockFlag_Default = LockFlag_ReadWrite, ///< Default read/write usage
			LockFlag_ReadOnly, ///< Read only
		};

	public:

		Buffer( ePurpose purpose = Purpose_Ext, Size byteSize = 0, eUsage usage = Usage_Default ) :
			m_usage( usage ),
			m_size( byteSize ),
			m_purpose( purpose ),
			m_definition( BufferDefinition( BufferElement( BufferElement::Element_Ext, typeOf< Byte >(), 1 ) ) ) 
		{
		}

		/** \brief 
		*/
		Buffer( ePurpose purpose, const BufferDefinition& definition, Size elementCount, eUsage usage = Usage_Default ) :
			m_usage( usage ),
			m_size( elementCount * definition.getSize() ),
			m_purpose( purpose ),
			m_definition( definition )
		{}

		virtual ~Buffer(){}

		/** \brief Initialise the vertex buffer to the specified size filled with data is supplied
		\param[in] size (optional) The size to allocate in the buffer - defaults 
			to size used in constructor if not 0
		\param[in] data (optional) Pointer to the buffer data - defaults to un-initialised memory
		*/
		virtual bool initialise( Size size = 0, const Byte* data = 0 ) = 0;

		/** \brief Lock the buffer and return a pointer to the data
		\param[in] startOffset Offset to the start position of the lock
		\param[in] lengthToLock Length of buffer to lock
		\param[in] flags Specifies the type of lock
		*/
		virtual void* lock( Size startOffset, Size lengthToLock,  eLockFlag flags = LockFlag_Default ) = 0;

		/** \brief Lock the entire buffer and return a pointer to the data
		\param[in] flags Specifies the type of lock
		*/
		void* lock( eLockFlag flags = LockFlag_Default )
		{ return lock( 0, 0, flags ); }

		/** \brief Releases locked sections of the buffer for other processes
		*/
		virtual void release() = 0;

		/** \brief Bind the buffer to its usage target
		*/


		/** \brief Gets the size of the buffer in bytes
		*/
		inline Size getSize() const
		{ return m_size; }
		
		/** \brief Get the vertex buffer definition
		*/
		inline const BufferDefinition& getDefinition() const
		{ return m_definition; }

		/** \brief Get the perspose defined for this buffer
		*/
		inline ePurpose getPurpose() const
		{ return m_purpose; }

	protected:
		ePurpose m_purpose; ///< The intened purpose of the buffer i.e. vertex or index storage
		eUsage m_usage; ///< The usage mode of the buffer
		Size m_size; ///< Stores the size of a buffer in bytes
		BufferDefinition m_definition; ///< Stores the definition of this buffer
	};


	/** \brief Software buffer implimentation
	*/
	class _DllExport SwBuffer : public Buffer
	{
	public:
		typedef frx::SmartPointer< Buffer > Ptr;

		static Ptr create( ePurpose purpose, const BufferDefinition& vertexDefinition, Size elementCount, eUsage usage = Usage_Default );
		static Ptr create( ePurpose purpose = Purpose_Ext, Size byteSize = 0, eUsage usage = Usage_Default );
	
	protected:

		SwBuffer( ePurpose purpose, const BufferDefinition& vertexDefinition, Size elementCount, eUsage usage = Usage_Default );
		SwBuffer( ePurpose purpose = Purpose_Ext, Size byteSize = 0, eUsage usage = Usage_Default );

		virtual ~SwBuffer();

	public:

		/** \brief \copydoc gfx::Buffer::initialise()
		*/
		virtual bool initialise( Size size = 0, const Byte* data = 0 );

		/** \brief \copydoc gfx::Buffer::lock()
		\remarks
			TODO: Proper locking - currently locks entire buffer
		*/
		virtual void* lock( Size startOffset, Size lengthToLock, eLockFlag flags = LockFlag_Default );

		/** \brief \copydoc gfx::Buffer::release()
		*/
		virtual void release();

	private:

		Byte* m_data; ///< Pointer to the software buffer
#ifdef _DEBUG
		Byte* m_lockData; ///< Pointer to the locked data buffer used to detect overruns in debug
		Size m_lengthToLock;
		Size m_lockStartOffset;
#endif
		volatile Count m_readLocks; ///< Stores a count for the number of read locks
		volatile Bool m_locked; //< Stores whether there is an explicit lock for writing to the buffer

	};



} //END: gfx

#endif