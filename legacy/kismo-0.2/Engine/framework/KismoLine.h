/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLine.h
 * Status: superseded by kismo-0.2/Engine/gui/KismoLine.h (same name, content differs)
 */
#ifndef _KISMO_LINE_H
#define	_KISMO_LINE_H

#include "framework/KismoCommon.h"
#include "framework/KismoVector.h"

namespace kismo
{
	namespace frx
	{
		//Forward Declaration
		class Matrix4x4; 
		class Matrix3x3;
		class BoundingSphere;
		class BoundingBox;

		/** \brief 3D Line */
		class _KismoExport Line
		{
		public: 
			/** \brief Constructor defalts to a non-initialised line for performance */
			Line();
			/** \brief Constructor from the start & end points of the line */
			Line( const Vector3& start, const Vector3& end );	
			/** \brief Transform the line by a matrix */
			void transform( const Matrix4x4& matrix );
			void transform( const Matrix3x3& matrix );	
			/** \brief Checks if this line intersects with a box
			\param box Box to check against*/
			k_Bool intersects( const BoundingBox& box ) const;
			/** \brief Checks if this line intersects with sphere
			\param sphere Sphere to check against*/
			k_Bool intersects( const BoundingSphere& sphere ) const;			 
			/** \brief This function takes a vector and returns the nearest point on the line
			\param[in] c Vector positon to test against
			\param[out] p Closest point on the line
			\return	The interval along the line where 0.0 is at the start, 1.0 is at the end */
			k_Real closestPoint( Vector3& p, const Vector3& c ) const;
			/** \brief Returns the start position vector of the line */
			const Vector3& start() const { return m_start; }
			/** \brief Returns the end position vector of the line */
			const Vector3& end() const { return m_end; }
			/** \brief Returns the vector representing the length and direction of the line from the
			start position */
			Vector3 vector() const { return m_end - m_start; }
			/** \brief Returns the direction vector of the line from the start position */
			Vector3 direction() const { return vector().normalise(); }			
			/** \brief Returns the length of the line */
			Vector3 length() const { return vector().length(); }		
			/** \brief Returns the length of the line */
			Vector3 lengthSquared() const { return vector().lengthSquared(); }
		private: 			
			Vector3 m_start; ///< The start poition of the line
			Vector3 m_end; ///< The end position of the line
		};

		
		k_Real Line::closestPoint( Vector3& p, const Vector3& c ) const
		{
			Vector3 ab = m_end - m_start;
			//Project c onto ab, but excluding divide by ab.dot(ab)
			k_Real t = ( c - m_start).dot( ab );
			if ( t <= (k_Real)0.0 )
			{
				//c projects outside the a,b interval on the a-side... clamp to a
				t = (k_Real)0.0;
				p = m_start;
			}
			else
			{
				k_Real denom = ab.dot( ab ); //always nonegative
				if ( t >= denom )
				{
					//c projects outside the a,b on the b-side... clamp to b
					t = (k_Real)1.0;
					p = m_end;
				}
				else
				{
					//c is inside a,b... must interleave
					t /= denom;
					p = m_start + (t * ab );
				}
			}	
			return t;
		}

	
	} // END: frx

} // END: kismo

#endif
