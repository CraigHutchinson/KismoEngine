/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Transform.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Transform.h
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

#ifndef KISMOFRAMEWORK_TRANSFORM_H
#define	KISMOFRAMEWORK_TRANSFORM_H

#include "KismoFramework/VectorN.h"
#include "KismoFramework/MatrixN.h"
#include <bitset>

namespace kismo
{	
	enum eSpace
	{
		Space_Local, ///< For translation within the local transform space
		Space_InverseLocal, ///< For translation within the inverse local transform space
		//Space_Global, ///< For translation within the transforms world space
		Space_World, ///< For translation within the world coordinate space
		Space_InverseWorld, ///< For translation within the inverse local transform space
	};

	/** A transform for position and orientation
	*/
	template < class _Real >
	class Transform
	{
	public:

		typedef _Real Real_T;							///< The floating point type sed for this transforms maths operations and storage unless noted in teh fucntion description
		typedef MatrixT< Real_T, 4,3 > _Matrix43;		///< 4x3 matrix holding a 3x3 rotation and vec3 translation
		typedef MatrixT< Real_T, 3,3 > _Matrix33;		///< 4x3 matrix contianing just rotation
		typedef typename _Matrix43::_Vector _Vector;	///< Vector 3 translation the same as each row fo the 4x3 matrix
				
		/** Special transform instances
		*/		
		static const Transform Identity;
		//static const Transform TransposedIdentity; ///< A transposed Identity matrix 

		/** Enumerator for getting the state of the transform and what data it holds
		*/
		enum eTransformFlag
		{
			TransformFlag_Identity = 0,		///< Holds whether the transform is identity
			//TransformFlag_TransposedIdentity = 0,		///< Holds whether the transform is identity

			TransformFlag_Count, //< Holds the enum count for validation in debug
		};

	public:

		/** Default constuctor initialises
			\notice The transform is NOT initialised to any value for performance by default
			\remarks Where the matrix is identity, tansform is 0 and scale is 1
		*/
		Transform();

		/** Constructs a transform with a given transform
			\remarks This is normally for creating an 'Identity' transform but could create transposed identity, null, or others
		*/
		Transform( eTransformFlag transformType );

		/** Constructs a transform with a given transformation defined by a 4x3 matrix
		*/
		Transform( const _Matrix43& matrix );

		Transform( const _Matrix33& rotation );

		/** Non-virtual destructor. 
			\remarks Transform should not be derived from
		*/
		~Transform(){}

		/** Set the full 4x3 matrix including rotation and translation
			\remarks The translation and rotation are affected by this operation
		*/
		void setMatrix( const _Matrix43& matrix )
		{ m_matrix = matrix; }

		/** Returns a reference to the full 4x3 transform matrix
		*/
		const _Matrix43& matrix() const
		{ return m_matrix; }
		
		_Matrix43& matrix()
		{ return m_matrix; }

		/** set the rotation part ofr the transform
			\remarks The translation is not affected by this operation
		*/
		void setRotation( const _Matrix33& rotation )
		{ m_matrix.asMatrix33() = rotation; }

		/** Returns the rotation part of the transform as a 3x3 matrix
			\notice This is just a 'reference' to the start of the 4x3 so is very lightweight
		*/
		const _Matrix33& rotation() const
		{ return m_matrix.asMatrix33(); }

		//void setTranslation( const _Vector& translate );

		/** get the translation vector of the transform
		*/
		const _Vector& translation() const
		{ return m_matrix.translation(); }

		/** Set the translation vector of the transform replacing any existing value
			\TODO: for interpolation we should have a 'translateTo' function
		*/
		const _Vector& setTranslation( const _Vector& vec )
		{ return m_matrix.setTranslation( vec ).translation(); }

		/** Translates the transform by an offset within the specified coordinate space
		*/
		const _Vector& translate( const _Vector& offset, eSpace space = Space_Local );

		/** \see _Matrix43::right 
		*/
		const _Vector& right() const
		{ return m_matrix.right(); }

		/** \see _Matrix43::up 
		*/
		const _Vector& up() const
		{ return m_matrix.up(); }

		/** \see _Matrix43::forwards 
		*/
		const _Vector& forwards() const
		{ return m_matrix.forwards(); }


		//void setScale( const _Vector& scale );
		//const _Vector& getScale() const
		//{ /*Assert( !isUserMatrix() );*/ return m_scale; }

		//void setUniformScale( Real_T scale );
		//Real_T getUniformScale() const
		//{ /*Assert( !isUserMatrix() );*/ return m_scale.x; }

		///** \brief Set the position of the pivotr point for rotation
		//*/
		//void setPivot( const _Vector& pivot );

		///** \brief Set the position of the pivotr point for rotation
		//*/
		//const _Vector& getPivot() const
		//{ return m_pivot; }

		///** \brief Makes the transform equal to Identity
		//*/
		//void makeIdentity();

		///** \brief Makes a uniform scale of 1
		//*/
		//void makeUnitScale();
		//
		///** \brief Gets whether the transform is identity
		//	\remarks After any call to set???() this will be false
		//*/
		//Bool isIdentity() const
		//{ return m_flags[ TransformFlag_Identity ]; }

		///** \brief Gets whether the transform has a rotate or scale component
		//	\remarks After any call to setRotate(), setScale(), setMatrix() etc this will be true
		//*/
		//Bool isRSMatrix() const
		//{ return m_flags[ TransformFlag_RSMatrix]; }

		///** \brief Gets whether the matrix is a user matix
		//	\remarks After any call to setuserMatrix this will be true
		//*/
		////Bool isUserMatrix() const
		////{ return m_flags[ TransformFlag_UserMatrix ]; }

		///** \brief gets whether the rotation is offset from centre
		//*/
		//Bool isOffsetPivot() const
		//{ return m_flags[ TransformFlag_OffsetPivot ]; }

		///** \brief Gets whether the transform has a uniform scale
		//	\remarks After any call to setScale() or setMatrix() this will be false
		//*/
		//Bool isUniformScale() const
		//{ return m_flags[ TransformFlag_UniformScale ]; }

		///** \brief Apply the transformation to the supplied _Vector
		//*/
		//_Vector applyForward( const _Vector& v ) const;

		///** \brief Apply the transformation to an array of vectors
		//*/
		//void applyForward( Count count, const _Vector* first, _Vector* output ) const;

		///** \brief Apply the inverse transformation to the supplied _Vector
		//*/
		//_Vector applyInverse( const _Vector& v ) const;

		///** \brief Apply the inverse transformation to an array of vectors
		//*/
		//void applyInverse( Count count, const _Vector* first, _Vector* output ) const;

		///** \brief Sets this transform to the product of two transforms
		//*/
		//void product( const Transform& a, const Transform& b );

		///** \brief Sets this transform to the inverse of another transform
		//*/
		//void inverse( const Transform& trans );

		///** \brief Modify the transfomr by a translation in the local axis
		//*/
		//void translate( const _Vector& translate, eSpace space );
		//	
		///** \brief Rotation by a matrix
		//*/
		//void rotate( const _Matrix43& matrix, eSpace space = Space_Local  );

		///** \brief Rotation about the X-axis
		//*/
		//void rotateX( _Real deg, eSpace space = Space_Local  );

		///**Rotation about the Y-axis
		//*/
		//void rotateY( _Real deg, eSpace space = Space_Local  );

		///** \brief Rotation about the Z-axis
		//*/
		//void rotateZ( _Real deg, eSpace space = Space_Local  );

	private:
		
		_Matrix43 m_matrix; ///< Represents a rotation or user matrix
		//_Vector m_translate; ///< Strores the translation
		//_Vector m_scale; ///< Stores the scale factors
		//_Vector m_pivot; ///< Stores the spoition of the pivot point
		//std::bitset< TransformFlag_Count > m_flags; ///< Transform state flags
	};

	typedef Transform< Float32 > TransformF;
	typedef Transform< Float64 > TransformD;
	typedef Transform< Real > TransformR;
	

} //END: kismo

#endif