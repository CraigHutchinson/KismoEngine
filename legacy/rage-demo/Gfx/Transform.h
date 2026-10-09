/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Transform.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Transform.h (same name, content differs)
 */
#ifndef FRX_TRANSFORM_H
#define	FRX_TRANSFORM_H

#include "Framework/Vector.h"
#include "Framework/Matrix.h"

namespace gfx
{	
	enum eSpace
	{
		eSpace_Local, ///< For translation within the local transform space
		eSpace_InverseLocal, ///< For translation within the inverse local transform space
		//eSpace_Global, ///< For translation within the transforms world space
		eSpace_World, ///< For translation within the world coordinate space
		eSpace_InverseWorld, ///< For translation within the inverse local transform space
	};

	/** \brief A transform for psoition and orientation
	*/
	template < class _TReal >
	class _DllExport Transform
	{
	public:

		typedef _TReal Real_T; //< typedef for convenience
		//TODO: This chouls be Matrix3 once its created as we only need rotation
		typedef frx::Matrix4< Real_T > Matrix_T; ///< typedef for convenience - easy switch between 3/4 tuples
		typedef frx::Vector3< Real_T > Vector_T; ///< typedef for convenience - easy switch between 3/4 tuples
		
		/** \brief Special transform instances
		*/		
		static const Transform Identity;

		/** \brief Enumerator for getting the state of the transform and what data it holds
		*/
		enum eTransformFlag
		{
			TransformFlag_Identity = 0, ///< Holds whether the transform is identity
			TransformFlag_RSMatrix, ///< Holds whether the transform is rotate scale matrix
			TransformFlag_UniformScale, ///< Holds whether the transform is of uniform scale
			TransformFlag_OffsetPivot, ///< Whether the transform has an offset pivot fromt the origin
			//TransformFlag_UserMatrix, ///< Holds whether the transform is user matrix
			TransformFlag_Count, //< Holds the enum count
		};


	public:

		/** \brief Default constuctor initialises to  an identity trasform
			\remarks Where the matrix is identity, tansform is 0 and scale is 1
		*/
		Transform();

		~Transform(){}

		void setRotate( const Matrix_T& rotate );
		const Matrix_T& getRotate() const
		{/* assert( m_flags[ TransformFlag_RSMatrix ] );*/ return m_matrix; }

		//void setUserMatrix( const Matrix_T& matrix );
		//const Matrix_T& getUserMatrix() const
		//{ assert( isUserMatrix() ); return m_matrix; }

		void setTranslate( const Vector_T& translate );
		const Vector_T& getTranslate() const
		{ /*assert( !isUserMatrix() );*/ return m_translate; }

		void setScale( const Vector_T& scale );
		const Vector_T& getScale() const
		{ /*assert( !isUserMatrix() );*/ return m_scale; }

		void setUniformScale( Real_T scale );
		Real_T getUniformScale() const
		{ /*assert( !isUserMatrix() );*/ return m_scale.x; }

		/** \brief Set the position of the pivotr point for rotation
		*/
		void setPivot( const Vector_T& pivot );

		/** \brief Set the position of the pivotr point for rotation
		*/
		const Vector_T& getPivot() const
		{ return m_pivot; }

		/** \brief Makes the transform equal to Identity
		*/
		void makeIdentity();

		/** \brief Makes a uniform scale of 1
		*/
		void makeUnitScale();
		
		/** \brief Gets whether the transform is identity
			\remarks After any call to set???() this will be false
		*/
		Bool isIdentity() const
		{ return m_flags[ TransformFlag_Identity ]; }

		/** \brief Gets whether the transform has a rotate or scale component
			\remarks After any call to setRotate(), setScale(), setMatrix() etc this will be true
		*/
		Bool isRSMatrix() const
		{ return m_flags[ TransformFlag_RSMatrix]; }

		/** \brief Gets whether the matrix is a user matix
			\remarks After any call to setuserMatrix this will be true
		*/
		//Bool isUserMatrix() const
		//{ return m_flags[ TransformFlag_UserMatrix ]; }

		/** \brief gets whether the rotation is offset from centre
		*/
		Bool isOffsetPivot() const
		{ return m_flags[ TransformFlag_OffsetPivot ]; }

		/** \brief Gets whether the transform has a uniform scale
			\remarks After any call to setScale() or setMatrix() this will be false
		*/
		Bool isUniformScale() const
		{ return m_flags[ TransformFlag_UniformScale ]; }

		/** \brief Apply the transformation to the supplied Vector
		*/
		Vector_T applyForward( const Vector_T& v ) const;

		/** \brief Apply the transformation to an array of vectors
		*/
		void applyForward( Count count, const Vector_T* first, Vector_T* output ) const;

		/** \brief Apply the inverse transformation to the supplied Vector
		*/
		Vector_T applyInverse( const Vector_T& v ) const;

		/** \brief Apply the inverse transformation to an array of vectors
		*/
		void applyInverse( Count count, const Vector_T* first, Vector_T* output ) const;

		/** \brief Sets this transform to the product of two transforms
		*/
		void product( const Transform& a, const Transform& b );

		/** \brief Sets this transform to the inverse of another transform
		*/
		void inverse( const Transform& trans );

		/** \brief Modify the transfomr by a translation in the local axis
		*/
		void translate( const Vector_T& translate, eSpace space );
			
		/** \brief Rotation by a matrix
		*/
		void rotate( const Matrix_T& matrix, eSpace space = eSpace_Local  );

		/** \brief Rotation about the X-axis
		*/
		void rotateX( _TReal deg, eSpace space = eSpace_Local  );

		/**Rotation about the Y-axis
		*/
		void rotateY( _TReal deg, eSpace space = eSpace_Local  );

		/** \brief Rotation about the Z-axis
		*/
		void rotateZ( _TReal deg, eSpace space = eSpace_Local  );


	private:

		Matrix_T m_matrix; ///< Represents a rotation or user matrix
		Vector_T m_translate; ///< Strores the translation
		Vector_T m_scale; ///< Stores the scale factors
		Vector_T m_pivot; ///< Stores the spoition of the pivot point
		BitSet< TransformFlag_Count > m_flags; ///< Transform state flags
	};

	typedef Transform< Float32 > TransformF;
	typedef Transform< Float64 > TransformD;
	typedef Transform< Real > TransformR;
	

} //END: gfx

#endif