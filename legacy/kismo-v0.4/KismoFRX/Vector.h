/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Vector.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Vector.h (same name, content differs)
 */
#ifndef VECTOR_H
#define VECTOR_H

#include "KismoFRX/Common.h"
#include "KismoFRX/Types.h"

/**
	\brief Defines some basic types used in the rendering of ray
*/
namespace kismo
{

	typedef std::basic_istream< Byte, std::char_traits< Byte > > IBStream;
	typedef std::basic_ostream< Byte, std::char_traits< Byte > > OBStream;
	typedef std::basic_ifstream< Byte, std::char_traits< Byte > > IBFStream;
	typedef std::basic_ofstream< Byte, std::char_traits< Byte > > OBFStream;

	/**
		\param _IntrinsicType Specifies that the data type is intrinsic(correct word!?) and can be re-allocated without calling a copy constructor
	*/
	template < class _ValType, bool _IntrinsicType = false >
	class Vector
	{
	public:
		
		class iterator
		{
			friend class Vector<_ValType,_IntrinsicType>;
		public:
		

#ifdef _DEBUG
#	define ITERATOR( _data ) iterator it( _data, this )
			iterator( _ValType* data, const Vector* owner ) : m_data( data ), dbg_owner( owner ) {}
			iterator( void ) : m_data(0), dbg_owner(0) {}
#else
#	define ITERATOR( _data ) iterator it( _data )
			iterator( _ValType* data ) : m_data( data ) {}
			iterator( void ) : m_data(0) {}
#endif

			void operator = ( const iterator& rhs ) const
			{ DEBUG_ONLY( dbg_owner = rhs.dbg_owner; ) m_data = rhs.m_data; }

			iterator& operator ++ ()
			{ DEBUG_ONLY( assert( m_data != dbg_owner->end().m_data ); ) ++m_data; return *this; }

			const iterator& operator ++ () const
			{ DEBUG_ONLY( assert( m_data != dbg_owner->end().m_data ); ) ++m_data; return *this; }

			const iterator& operator -- () const
			{ DEBUG_ONLY( assert( m_data != dbg_owner->begin().m_data ); ) --m_data; return *this; }

			iterator& operator -- ()
			{ DEBUG_ONLY( assert( m_data != dbg_owner->begin().m_data ); ) --m_data; return *this; }

			iterator operator -( Int32 records ) const
			{ return *this + (-records); }

			iterator operator +( Int32 records ) const
			{ 
				DEBUG_ONLY( assert( records <= 0 || m_data != dbg_owner->end().m_data ); )
				DEBUG_ONLY( assert( records >= 0 || m_data != dbg_owner->begin().m_data ); )
				iterator ret = *this; 
				ret.m_data += records;
				return ret;
			}

			bool operator !() const
			{ return m_data == 0; }

			/*operator const void* () const
			{ return (const void*)m_data; }*/

			const _ValType* operator -> () const
			{ return m_data; }
			const _ValType& operator * () const
			{ return *m_data; }

			_ValType* operator -> ()
			{ return m_data; }
			_ValType& operator * ()
			{ return *m_data; }

			bool operator > ( const iterator& rhs ) const
			{ DEBUG_ONLY( assert( dbg_owner == rhs.dbg_owner );) return m_data > rhs.m_data; }
			bool operator < ( const iterator& rhs ) const
			{ DEBUG_ONLY( assert( dbg_owner == rhs.dbg_owner );) return m_data < rhs.m_data; }
			bool operator == ( const iterator& rhs ) const
			{ DEBUG_ONLY( assert( dbg_owner == rhs.dbg_owner );) return m_data == rhs.m_data; }
			bool operator != ( const iterator& rhs ) const
			{ DEBUG_ONLY( assert( dbg_owner == rhs.dbg_owner );) return m_data != rhs.m_data; }

		private:			
		//	operator iterator() const ///< Prevent casting away from const
		//	{ assert(false); return *this; }

			DEBUG_ONLY( mutable const Vector* dbg_owner; ) ///, The owning vector for range checking - mutable for assignement when const
			mutable _ValType* m_data; ///< pointer to the iterators data position - mutable for const_iterator
		};

		typedef const iterator const_iterator;

		typedef _ValType value_type;

	public:

		Vector() : m_data(0), m_size(0), m_capacity(0) {}
		Vector( const Vector& rhs ) : m_data(0), m_size(0), m_capacity( 0 )
		{ *this = rhs; }

		~Vector() { if ( m_data ) delete[] m_data; }

		const _ValType* c_ptr() const
		{ return m_data; }
		
		_ValType* c_ptr()
		{ return m_data; }

		void operator = ( const Vector& rhs )
		{
			if ( rhs.empty() )
				clear();
			else
			if ( capacity() >= rhs.size() )
			{
				assign( m_data, rhs.m_data, rhs.size() );
				m_size = rhs.size();
			}
			else
				allocate( rhs.m_data, rhs.m_size );
		}

		void operator += ( const Vector& rhs )
		{
			if ( !rhs.empty() )
			{
				reserve( m_size + rhs.size() );
				assign( m_data + m_size, rhs.m_data, rhs.size() );
				m_size += rhs.size();
			}
		}

		void reserve( Count newCapacity )
		{
			if ( newCapacity > capacity() )
				reallocate( newCapacity );
		}

		bool resize( Count newCapacity )
		{
			if ( newCapacity != capacity() )
				reallocate( newCapacity );

			m_size = newCapacity;
			return true;
		}

		iterator erase( iterator iter )
		{
			DEBUG_ONLY( assert( iter.dbg_owner == this ) ); //, make sure we don't call erse using a diffferent vector!
			assert( iter != end() );
			assert(m_size);
			iter.m_data->_ValType::~_ValType(); //Destruct this element
			size_t size = ((m_data + m_size - 1 )-iter.m_data);
			if ( size > 0 )
				memmove_s( iter.m_data, size * sizeof(_ValType), iter.m_data+1, size * sizeof(_ValType) ); //, Move elements back an element - we don't need to use a copy contrsutor and can direct copy/move memory
			memset( iter.m_data + size, 0 , sizeof(_ValType) ); //, When we destroy we are goingto call the destructor on thsi last element which has been moved so tidy things up				
			--m_size;
			return iter;
		}

		void allocate( _ValType* newData, Count dataSize )
		{
			if ( m_data ) 
			{
				delete[] m_data;
				m_data = 0;
			}
			if ( dataSize  )
				m_data = new _ValType[ dataSize ]();
			m_capacity = dataSize;	
			if ( newData )
			{
				m_size = dataSize;
				assign( m_data, newData, m_size ); ///< Copy 1-by-1 with copy constrtuctors
			}
			else
				m_size = 0;
		}

		void reallocate( Count newCapacity )
		{
			//_heapchk();

			/*const size_t dataSize = sizeof(_ValType);
			if ( dataSize * newCapacity > SIZE_MAX )
			{
				assert( false == "Cannot allocate as this exceeds available allocation limit!" );
			}*/

			_ValType* oldData = m_data;
			if ( newCapacity )
				m_data = new _ValType[ newCapacity ]();
			else
				m_data = 0;
		
			 

			m_capacity = newCapacity;
			m_size = ( m_size < m_capacity ? m_size : m_capacity ); //, make sure size is shrunk if we have allocated less!
			if( oldData )
			{
				if ( m_data && m_size )
					assign( m_data, oldData, m_size ); ///< Copy 1-by-1 with copy constrtuctors
				delete[] oldData;
			}
		}

		/**
			\brief Transfer array ownership to another vector
		*/
		bool moveTo( Vector& rhs )
		{
			rhs.clear();
			rhs.m_capacity = m_capacity; m_capacity = 0;
			rhs.m_data = m_data; m_data = 0;
			rhs.m_size = m_size; m_size = 0;
			return true;
		}

		void assign( _ValType* dest, _ValType* first, Size size )
		{
			if ( !_IntrinsicType )
			{
				_ValType* last = first + size;
				for (; first != last; ++dest, ++first)///< Copy 1-by-1 with copy constrtuctors
					*dest = *first;
			}
			else
				memcpy( dest,/* m_capacity,*/ first, sizeof(_ValType) * size ); ///< Doesn't call copy constructors
		}

		void clear()
		{ resize(0); }

		Bool empty() const
		{ return m_size == 0; }

		Count capacity() const
		{ return m_capacity; }

		Count size() const
		{ return m_size; }
		
		_ValType& operator [] ( Index i )
		{ assert( i < size() ); return m_data[ i ]; }
		const _ValType& operator [] ( Index i ) const
		{ assert( i < size() ); return m_data[ i ]; }

		_ValType& at( Index i )
		{ assert( i < size() ); return m_data[ i ]; }
		const _ValType& at( Index i ) const
		{ assert( i < size() ); return m_data[ i ]; }
		
		_ValType& push_back( const _ValType& val )
		{ 
			if ( m_size + 1 > capacity() )
				reallocate( (capacity() + 1) << 1 ); ///< Double capacity
			m_data[ m_size ] = val; 
			return m_data[ m_size++ ]; //, Return the new instance and increment the size after
		}

		iterator begin()
		{ ITERATOR( m_data ); return it; }
		const_iterator begin() const
		{ const ITERATOR( m_data ); return it; }

		iterator end()
		{ ITERATOR( m_data + m_size ); return it; }
		const_iterator end() const
		{ const ITERATOR( m_data + m_size ); return it; }

		iterator rbegin()
		{ ITERATOR( m_data + m_size - 1 ); return it; }
		const_iterator rbegin() const
		{ const ITERATOR( m_data + m_size - 1 ); return it; }

		iterator rend()
		{ ITERATOR( m_data - 1 ); return it; }
		const_iterator rend() const
		{ const ITERATOR( m_data - 1 ); return it; }


	private:
		Count m_capacity; ///< The capacity
		Count m_size; ///, The currently used size
		_ValType* m_data; ///< The data

	};

	/**
		\brief Writes the vector to file
		\remarks This fast method is only valid if _IntrinsicType == true which means the data has no pointers and can be copied directly to file
		which is significantly faster
	*/
	template< class _ValType >
	OBStream& operator << ( OBStream& stream, const typename Vector< _ValType, true  >& rhs )
	{
		//stream << '\0';
	
		Count size = rhs.size(); 
		stream.write( (Byte*)&size, sizeof( Count ) ); //< output the number of elements in the vector
		
		if ( size != 0 )
		{
			Size valueSize = (Size)sizeof( _ValType );
			stream.write( (Byte*)&valueSize, sizeof( Size ) );
			
			assert( rhs.c_ptr() ); //No need to bother outputing anything for zero sized vectors			
			stream.write( (Byte*)rhs.c_ptr(), (std::streamsize)valueSize * size );
		}
		
		//stream << '\0';
		return stream;
	}

	/**
		\brief Writes the vector to file
		\remarks This slower method must be used if _IntrinsicType == false and will use the stream << operator of the data type
	*/
	template< class _ValType >
	OBStream& operator << ( OBStream& stream, const typename Vector< _ValType, false  >& rhs )
	{
		//stream << '\0';

		Count size = rhs.size(); 
		stream.write( (Byte*)&size, sizeof( Count ) ); //< output the number of elements in the vector

		if ( size != 0 )
		{
			Size valueSize = (Size)sizeof( _ValType );
			stream.write( (Byte*)&valueSize, sizeof( Size ) );
		
			//As the data type is not intrinsic we must write each element individually using its stream operator
			for ( typename Vector< _ValType, false  >::const_iterator iVal = rhs.begin(); iVal != rhs.end(); ++iVal )
				stream << *iVal;
		}
		//stream.flush();
		stream;// << '\0';
		return stream;
	}
	
	/**
		\brief Reads the vector from file
		\remarks This fast method is only valid if _IntrinsicType == true which means the data has no pointers and can be copied directly from file
		which is significantly faster
	*/
	template< class _ValType >
	IBStream& operator >> ( IBStream& stream, typename Vector< _ValType, true  >& rhs )
	{
		//Byte tchar = '\0'; stream >> tchar; assert( tchar == '\0' );

		Count size = 0;
		stream.read( (Byte*)&size, sizeof( Count ) );
		if ( size != 0 )
		{
			Size valueSize;
			stream.read( (Byte*)&valueSize, sizeof( Size ) );
			if ( valueSize != sizeof( _ValType ) )
			{
				stream.setstate( std::ios_base::failbit );
			}
			else
			{
				rhs.resize( size ); //< Resize the vector to store this data
				
				std::streamsize bytesRemaining = valueSize * size;
				assert( bytesRemaining < Int32_Max );
				stream.read( (Byte*)rhs.c_ptr(), bytesRemaining );
				assert( stream.gcount() == valueSize * size );
			}
		}
			
		//stream >> tchar; assert( tchar == '\0' );

		return stream;
	}

	//template< class _ValType >
	//IBFStream& operator >> ( IBFStream& stream, typename Vector< _ValType, true  >& rhs )
	//{
	//	Byte tchar = '\0'; stream >> tchar; assert( tchar == '\0' );

	//	Size valueSize;
	//	stream >> valueSize >> tchar; assert( tchar == '\0' ); ///< input the size of the data elements so we can make sure the version of data is compatible at a later date
	//	if ( valueSize != sizeof( _ValType ) )
	//	{
	//		stream.setstate( std::ios_base::failbit );
	//	}
	//	else
	//	{
	//		Count size = 0;
	//		stream >> size >> tchar; assert( tchar == '\0' ); //< input the number of elements in the vector
	//		rhs.resize( size ); //< Resize the vector to store this data
	//		if ( size )
	//		{
	//			std::streamsize bytesRemaining = (size_t)valueSize * size;
	//			assert( bytesRemaining < Int32_Max );
	//			stream.read( (Byte*)rhs.c_ptr(), bytesRemaining );
	//			assert( stream.gcount() == valueSize * size );
	//		}
	//	}
	//		
	//	stream >> tchar; assert( tchar == '\0' );

	//	return stream;
	//}

	/**
		\brief Reads the vector from file
		\remarks This slower method must be used if _IntrinsicType == false and will use the stream >> operator of the data type
	*/
	template< class _ValType >
	IBStream& operator >> ( IBStream& stream, typename Vector< _ValType, false  >& rhs )
	{		
		//Byte tchar = '\0'; stream >> tchar; assert( tchar == '\0' );

		Count size = 0;
		stream.read( (Byte*)&size, sizeof( Count ) );
	
		if ( size != 0 )
		{			
			Size valueSize;
			stream.read( (Byte*)&valueSize, sizeof( Size ) ); ///< input the size of the data elements so we can make sure the version of data is compatible at a later date
			if ( valueSize != sizeof( _ValType ) )
			{
				stream.setstate( std::ios_base::failbit );
			}
			else
			{
				rhs.resize( size ); //< Resize the vector to store this data			
				
				//As the data type is not intrinsic we must read each element individually using its stream operator
				for ( typename Vector< _ValType, false  >::iterator iVal = rhs.begin(); iVal != rhs.end(); ++iVal )
					stream >> *iVal;
			}
		}
			
		//stream >> tchar; assert( tchar == '\0' );

		return stream;
	}

	/**
		\brief Checks two vectors for equality
		\remarks This fast method is only valid if _IntrinsicType == true which means the data has no pointers and can be memory-compared on its data bytes
	*/
	template< class _ValType >
	bool operator == ( const typename Vector< _ValType, true  >& lhs, const typename Vector< _ValType, true  >& rhs )
	{
		if ( lhs.size() != rhs.size() )
			return false;

		//Size is the same so do a memory compare
		return memcmp( lhs.c_ptr(), rhs.c_ptr(), sizeof( _ValType ) * lhs.size() ) == 0;
	}

	/**
		\brief Checks two vectors for equality
		\remarks This slower method must be used if _IntrinsicType == false and will use the datas equality operator
	*/
	template< class _ValType >
	bool operator == ( const typename Vector< _ValType, false  >& lhs, const typename Vector< _ValType, false  >& rhs )
	{
		if ( lhs.size() != rhs.size() )
			return false;

		//Size is the same so do a element compare
		typename Vector< _ValType, false  >::const_iterator iLhsVal = lhs.begin();
		typename Vector< _ValType, false  >::const_iterator iRhsVal = rhs.begin();
		for (  ; iLhsVal != lhs.end(); ++iLhsVal, ++iRhsVal )
			if ( *iLhsVal != *iRhsVal ) ///< TODO: This does not work with vectors of pointers!
				return false;
		
		return true; //< All elements are the same
	}

	template< class _ValType, bool _IntrinsicType >
	bool operator != ( const typename Vector< _ValType, _IntrinsicType  >& lhs, const typename Vector< _ValType, _IntrinsicType  >& rhs )
	{
		return !(lhs == rhs);
	}

	//typedef Vector< String > StringArray; //String
	typedef Vector< Index, true > IndexArray;
	typedef Vector< String, false > StringArray;
	typedef Vector< Byte, true > ByteArray;

} //END: Kismo

//#endif
#endif