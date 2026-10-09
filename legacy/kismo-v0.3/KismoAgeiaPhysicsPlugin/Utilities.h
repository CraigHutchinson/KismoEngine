/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\Utilities.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYSX_UTILITIES
#define PHYSX_UTILITIES

#include "Framework/Vector.h"
#include "Framework/Matrix.h"
#include <NxVec3.h>
#include <NxMat34.h>
#include <NxStream.h>

/** COnverts a SDK create error to a string literal value
*/
const char* getNxSDKCreateError(const NxSDKCreateError& errorCode);

/** Helper function for converting the framework vector into a Ageia PhysX vector
*/
template < class TReal >
inline NxVec3 toNxVec3( const frx::Vector3< TReal >& vec )
{ return NxVec3( vec.x, vec.y, vec.z ); }

/** Helper function to convert from a Ageia PhysX vector to an engine vector
*/
inline frx::Vector3r toVec3r( const NxVec3& vec )
{ return frx::Vector3r( vec.x, vec.y, vec.z ); }

/** Helper function to convert from a Ageia PhysX matrix to a transform rotation
*/
inline frx::Matrix4r toRotateMatrix4r( const NxMat34& m )
{ return frx::Matrix4r(		m.M(0,0), m.M(1,0), m.M(2,0), 0,
							m.M(0,1), m.M(1,1), m.M(2,1), 0,
							m.M(0,2), m.M(1,2), m.M(2,2), 0,
							0, 0, 0, 1 ); }

/** Helper function to convert from a Ageia PhysX matrix to a transform translation
*/
inline frx::Vector3r toVec3r( const NxMat34& mat )
{ return frx::Vector3r( mat.t.x, mat.t.y,  mat.t.z ); }

inline NxMat33 toNxMat33( const frx::Matrix4r& m )
{
	return NxMat33( NxVec3( m(0,0),  m(0,1),  m(0,2) ),
					NxVec3( m(1,0),  m(1,1),  m(1,2) ),
					NxVec3( m(2,0),  m(2,1),  m(2,2) ) );
}

/** Stream implementation taken directly from teh PhysX samples
*/
class MemoryWriteBuffer : public NxStream
{
public:
								MemoryWriteBuffer();
	virtual						~MemoryWriteBuffer();
				void			clear();

	virtual		NxU8			readByte()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU16			readWord()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		NxU32			readDword()								const	{ NX_ASSERT(0);	return 0;	}
	virtual		float			readFloat()								const	{ NX_ASSERT(0);	return 0.0f;}
	virtual		double			readDouble()							const	{ NX_ASSERT(0);	return 0.0;	}
	virtual		void			readBuffer(void* buffer, NxU32 size)	const	{ NX_ASSERT(0);				}

	virtual		NxStream&		storeByte(NxU8 b);
	virtual		NxStream&		storeWord(NxU16 w);
	virtual		NxStream&		storeDword(NxU32 d);
	virtual		NxStream&		storeFloat(NxReal f);
	virtual		NxStream&		storeDouble(NxF64 f);
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size);

				NxU32			currentSize;
				NxU32			maxSize;
				NxU8*			data;
};

/** Stream buffer taken directly from the PhysX sample documentation
*/
class MemoryReadBuffer : public NxStream
	{
	public:
								MemoryReadBuffer(const NxU8* data);
	virtual						~MemoryReadBuffer();

	virtual		NxU8			readByte()								const;
	virtual		NxU16			readWord()								const;
	virtual		NxU32			readDword()								const;
	virtual		float			readFloat()								const;
	virtual		double			readDouble()							const;
	virtual		void			readBuffer(void* buffer, NxU32 size)	const;

	virtual		NxStream&		storeByte(NxU8 b)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeWord(NxU16 w)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDword(NxU32 d)							{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeFloat(NxReal f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeDouble(NxF64 f)						{ NX_ASSERT(0);	return *this;	}
	virtual		NxStream&		storeBuffer(const void* buffer, NxU32 size)	{ NX_ASSERT(0);	return *this;	}

	mutable		const NxU8*		buffer;
	};

#endif