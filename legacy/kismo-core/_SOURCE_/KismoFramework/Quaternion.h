/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Quaternion.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_QUATERNION_H
#define	KISMOFRAMEWORK_QUATERNION_H

#include "KismoFramework/VectorN.h"
#include "KismoFramework/Math.hpp"

namespace kismo
{
	/**
		\brief Quaternion
	*/
	class Quaternion
	{
	public:

		Real w, x, y, z;

		Quaternion( Real angle, const Vector3& axis )
		{
			fromAxis( angle, axis );
		}

		Quaternion::Quaternion( Real nw, Real nx, Real ny, Real nz ) :
			w( nw ), x( nx ), y( ny ), z( nz )
		{}

		Vector3 rotate( const Vector3& point )
		{
			//http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
			Real t2 =   w*x;
			Real t3 =   w*y;
			Real t4 =   w*z;
			Real t5 =  -x*x;
			Real t6 =   x*y;
			Real t7 =   x*z;
			Real t8 =  -y*y;
			Real t9 =   y*z;
			Real t10 = -z*z;
			return Vector3( 2*( (t8 + t10)*point.x + (t6 -  t4)*point.y + (t3 + t7)*point.z ) + point.x, 
				2*( (t4 +  t6)*point.x + (t5 + t10)*point.y + (t9 - t2)*point.z ) + point.y, 
				2*( (t7 -  t3)*point.x + (t2 +  t9)*point.y + (t5 + t8)*point.z ) + point.z );
		}


		Quaternion operator *( const Quaternion &rhs ) const
		{
		  Real nw = w*rhs.w - (x*rhs.x + y*rhs.y + z*rhs.z);
		  
		  Real nx = w*rhs.x + rhs.w*x + y*rhs.z - z*rhs.y;
		  Real ny = w*rhs.y + rhs.w*y + z*rhs.x - x*rhs.z;
		  Real nz = w*rhs.z + rhs.w*z + x*rhs.y - y*rhs.x;

		  return Quaternion(nw,nx,ny,nz); 
		}

		const Quaternion& operator *= ( const Quaternion& rhs )
		{
			Real nw = w*rhs.w - (x*rhs.x + y*rhs.y + z*rhs.z);

			Real nx = w*rhs.x + rhs.w*x + y*rhs.z - z*rhs.y;
			Real ny = w*rhs.y + rhs.w*y + z*rhs.x - x*rhs.z;
			Real nz = w*rhs.z + rhs.w*z + x*rhs.y - y*rhs.x;

			w = nw; x = nx; y = ny; z = nz;
			return *this;
		}

		/**
			\brief Conjugate Quaternion
		*/
		const Quaternion& operator ~ ()
		{			
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		/**
			\brief Inverts the quaternion
		*/
		const Quaternion& operator  - ()
		{
			Real norm = sqrt(w*w + x*x + y*y + z*z);
			if ( norm == (Real)0.0 )
				 norm = (Real)1.0;

			norm = (Real)1.0 / norm;
			w *= norm;
			x *= -norm;
			y *= -norm;
			z *= -norm;

			return *this;
		}

		/**
			\brief Normalise
		*/
		const Quaternion& normalise()
		{			
			Real norm = sqrt(w*w + x*x + y*y + z*z);
			if (norm == (Real)0.0)
			{
				w = x = y = z = 0;
			}
			else
			{
				norm = (Real)1.0/norm;
				w *= norm; x *= norm; y *= norm; z *= norm;
			}
			return *this;
		}
		
		/**
			\brief Copy-Normalise
		*/
		Quaternion normalised() const
		{	
			Real norm = sqrt(w*w + x*x + y*y + z*z);
			if (norm == (Real)0.0)
				return Quaternion(0,0,0,0);
			else
			{
				norm = (Real)1.0/norm;
				return Quaternion( w * norm, x * norm, y * norm, z * norm );
			}
		}

		/**
			\brief Assignment from an axis and angle
		*/
		const Quaternion& fromAxis( Real angle, const Vector3& axis )
		{ return fromAxis( angle, axis.x, axis.y, axis.z ); }

		/**
			\brief Assignment from an axis and angle
		*/
		const Quaternion& fromAxis( Real angle, Real vx, Real vy, Real vz )
		{
			Real s = sqrt(vx*vx + vy*vy + vz*vz);

			if ( fabs( s ) > Real_Epsilon )
			{
				Real c = (Real)1.0/s;

				vx *= c; vy *= c; vz *= c;

				Real omega = (Real)-0.5 * angle;
				s = (Real)sin(omega);

				x = s*vx; y = s*vy; z = s*vz;
				w = (Real)cos(omega);
			}
			else
			{
				w = x = y = z = (Real)1.0f;
			}
			
			return normalise();
		}

		//To Matrix
		//t1 =  cos(theta)
		//t2 =  1 - t1
		//t3 =  v1*v1
		//t6 =  t2*v1
		//t7 =  t6*v2
		//t8 =  sin(theta)
		//t9 =  t8*v3
		//t11 = t6*v3
		//t12 = t8*v2
		//t15 = v2*v2
		//t19 = t2*v2*v3
		//t20 = t8*v1
		//t24 = v3*v3
		//R[1, 1] = t1 + t2*t3
		//R[1, 2] = t7 - t9
		//R[1, 3] = t11 + t12
		//R[2, 1] = t7 + t9
		//R[2, 2] = t1 + t2*t15
		//R[2, 3] = t19 - t20
		//R[3, 1] = t11 - t12
		//R[3, 2] = t19 + t20
		//R[3, 3] = t1 + t2*t24

				/*
		void Quaternion::ToMatrix(float matrix[3][3]) const
		{
		  matrix[0][0] = 1.0 - 2*Y*Y - 2*Z*Z;
		  matrix[1][0] = 2*X*Y + 2*W*Z;
		  matrix[2][0] = 2*X*Z - 2*W*Y;

		  matrix[0][1] = 2*X*Y - 2*W*Z;
		  matrix[1][1] = 1.0 - 2*X*X - 2*Z*Z;
		  matrix[2][1] = 2*Y*Z + 2*W*X;

		  matrix[0][2] = 2*X*Z + 2*W*Y;
		  matrix[1][2] = 2*Y*Z - 2*W*X;
		  matrix[2][2] = 1.0 - 2*X*X - 2*Y*Y;
		}


		void Quaternion::Slerp(const Quaternion &a,const Quaternion &b, const float t)
		{
		  float omega, cosom, sinom, sclp, sclq;
		  int i;


		  cosom = a.X*b.X + a.Y*b.Y + a.Z*b.Z + a.W*b.W;


		  if ((1.0f+cosom) > FLT_EPSILON)
			{
			  if ((1.0f-cosom) > FLT_EPSILON)
				{
				  omega = acos(cosom);
				  sinom = sin(omega);
				  sclp = sin((1.0f-t)*omega) / sinom;
				  sclq = sin(t*omega) / sinom;
				}
			  else
				{
				  sclp = 1.0f - t;
				  sclq = t;
				}

			  X = sclp*a.X + sclq*b.X;
			  Y = sclp*a.Y + sclq*b.Y;
			  Z = sclp*a.Z + sclq*b.Z;
			  W = sclp*a.W + sclq*b.W;

			}
		  else
			{
			  X =-a.Y;
			  Y = a.X;
			  Z =-a.W;
			  W = a.Z;

			  sclp = sin((1.0f-t) * PI * 0.5);
			  sclq = sin(t * PI * 0.5);

			  X = sclp*a.X + sclq*b.X;
			  Y = sclp*a.Y + sclq*b.Y;
			  Z = sclp*a.Z + sclq*b.Z;

			}
		}

		const Quaternion& Quaternion::exp()
		{                               
		  float Mul;
		  float Length = sqrt(X*X + Y*Y + Z*Z);

		  if (Length > 1.0e-4)
			Mul = sin(Length)/Length;
		  else
			Mul = 1.0;

		  W = cos(Length);

		  X *= Mul;
		  Y *= Mul;
		  Z *= Mul; 

		  return *this;
		}

		const Quaternion& Quaternion::log()
		{
		  float Length;

		  Length = sqrt(X*X + Y*Y + Z*Z);
		  Length = atan(Length/W);

		  W = 0.0;

		  X *= Length;
		  Y *= Length;
		  Z *= Length;

		  return *this;
		}*/

	};

} //END: kismo

#endif