/*
 * matrix_ops.h
 *
 *  Created on: 5 sty 2018
 *      Author: crm
 */

#pragma once

#include "matrix.h"
#include "vector.h"

namespace Engine
	{
	namespace Math
		{
		/*****************************************************************************/
		/* Deklaracje ****************************************************************/
		/*****************************************************************************/
		template<typename T> TVector<T> VectorTransform(const TVector<T>& a, const TMatrix<T>& m);

		template<typename T> TMatrix<T> MatrixTranspose(const TMatrix<T>& m);
		template<typename T> TMatrix<T> MatrixAdd(const TMatrix<T>& a, const TMatrix<T>& b);
		template<typename T> TMatrix<T> MatrixSubtract(const TMatrix<T>& a, const TMatrix<T>& b);
		template<typename T> TMatrix<T> MatrixMultiply(const TMatrix<T>& a, const TMatrix<T>& b);

		template<typename T> TMatrix<T> MatrixAbsolute(const TMatrix<T>& a);

		template<typename T> TMatrix<T> MatrixInverse(const TMatrix<T>& m);
		template<typename T> TVector<T> MatrixDeterminant(const TMatrix<T>& m);
		template<typename T> TMatrix<T> MatrixLU(const TMatrix<T>& m);

		template<typename T> TMatrix<T> MatrixRotX(float ang);
		template<typename T> TMatrix<T> MatrixRotY(float ang);
		template<typename T> TMatrix<T> MatrixRotZ(float ang);
		template<typename T> TMatrix<T> MatrixRotate(float angle, const TVector<T>& dir);
		template<typename T> TMatrix<T> MatrixTranslate(const TVector<T>& v);
		template<typename T> TMatrix<T> MatrixSkew(const TVector<T>& a);

		template<typename T> TMatrix<T> MatrixZero();
		template<typename T> TMatrix<T> MatrixIdentity();
		template<typename T> TMatrix<T> MatrixFill(T val);
		template<typename T> TMatrix<T> MatrixDiagonal(T val);

		/*****************************************************************************/
		/* Operatory *****************************************************************/
		/*****************************************************************************/
		template<typename T> inline TMatrix<T> operator-(const TMatrix<T>& a) {return TMatrix<T>(-a.row[0], -a.row[1], -a.row[2], -a.row[3]);}
		template<typename T> inline TMatrix<T> operator+(const TMatrix<T>& m, const TMatrix<T>& n) {return MatrixAdd(m, n);}
		template<typename T> inline TMatrix<T> operator-(const TMatrix<T>& m, const TMatrix<T>& n) {return MatrixSubtract(m, n);}
		template<typename T> inline TMatrix<T> operator*(const TMatrix<T>& m, const TMatrix<T>& n) {return MatrixMultiply(m, n);}
		template<typename T> inline TMatrix<T> operator*(const TMatrix<T>& m, T v) {return MatrixMultiply(m, MatrixFill(v));}
		template<typename T> inline TMatrix<T> operator*(T v, const TMatrix<T>& m) {return MatrixMultiply(MatrixFill(v), m);}
		template<typename T> inline TVector<T> operator*(const TMatrix<T>& m, const TVector<T>& a) {return VectorTransform(a, m);}

		/*****************************************************************************/
		/* Definicje *****************************************************************/
		/*****************************************************************************/
		template<typename T> inline TVector<T> VectorTransform(const TVector<T>& a, const TMatrix<T>& m)
			{
			TVector<T> c;

			c.x=a.x*m.row[0].x+a.y*m.row[0].y+a.z*m.row[0].z+a.w*m.row[0].w;
			c.y=a.x*m.row[1].x+a.y*m.row[1].y+a.z*m.row[1].z+a.w*m.row[1].w;
			c.z=a.x*m.row[2].x+a.y*m.row[2].y+a.z*m.row[2].z+a.w*m.row[2].w;
			c.w=a.x*m.row[3].x+a.y*m.row[3].y+a.z*m.row[3].z+a.w*m.row[3].w;

			return c;
			}


		template<typename T> inline TMatrix<T> MatrixTranspose(const TMatrix<T>& m)
			{
			return TMatrix<T>(
				TVector<T>(m.row[0].x, m.row[1].x, m.row[2].x, m.row[3].x),
				TVector<T>(m.row[0].y, m.row[1].y, m.row[2].y, m.row[3].y),
				TVector<T>(m.row[0].z, m.row[1].z, m.row[2].z, m.row[3].z),
				TVector<T>(m.row[0].w, m.row[1].w, m.row[2].w, m.row[3].w)
				);
			}

		template<typename T> inline TMatrix<T> MatrixAdd(const TMatrix<T>& a, const TMatrix<T>& b)
			{
			return TMatrix<T>(
				VectorAdd(a.row[0], b.row[0]),
				VectorAdd(a.row[1], b.row[1]),
				VectorAdd(a.row[2], b.row[2]),
				VectorAdd(a.row[3], b.row[3]));
			}

		template<typename T> inline TMatrix<T> MatrixSubtract(const TMatrix<T>& a, const TMatrix<T>& b)
			{
			return TMatrix<T>(
				VectorSubtract(a.row[0], b.row[0]),
				VectorSubtract(a.row[1], b.row[1]),
				VectorSubtract(a.row[2], b.row[2]),
				VectorSubtract(a.row[3], b.row[3]));
			}

		template<typename T> inline TMatrix<T> MatrixMultiply(const TMatrix<T>& a, const TMatrix<T>& b)
			{
			return TMatrix<T>(
				TVector<T>(
					a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0] + a[0][3]*b[3][0],
					a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1] + a[0][3]*b[3][1],
					a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2] + a[0][3]*b[3][2],
					a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3]*b[3][3]),
				TVector<T>(
					a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0] + a[1][3]*b[3][0],
					a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1] + a[1][3]*b[3][1],
					a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2] + a[1][3]*b[3][2],
					a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3]*b[3][3]),
				TVector<T>(
					a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0] + a[2][3]*b[3][0],
					a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1] + a[2][3]*b[3][1],
					a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2] + a[2][3]*b[3][2],
					a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3]*b[3][3]),
				TVector<T>(
					a[3][0]*b[0][0] + a[3][1]*b[1][0] + a[3][2]*b[2][0] + a[3][3]*b[3][0],
					a[3][0]*b[0][1] + a[3][1]*b[1][1] + a[3][2]*b[2][1] + a[3][3]*b[3][1],
					a[3][0]*b[0][2] + a[3][1]*b[1][2] + a[3][2]*b[2][2] + a[3][3]*b[3][2],
					a[3][0]*b[0][3] + a[3][1]*b[1][3] + a[3][2]*b[2][3] + a[3][3]*b[3][3]));
			}

		template<typename T> inline TMatrix<T> MatrixAbsolute(const TMatrix<T>& a)
			{
			return TMatrix<T>(
				VectorAbsolute(a.row[0]),
				VectorAbsolute(a.row[1]),
				VectorAbsolute(a.row[2]),
				VectorAbsolute(a.row[3]));
			}

		template<typename T> inline TMatrix<T> MatrixInverse(const TMatrix<T>& mm)
			{
			TMatrix<T> mi;

			T *m=(T*)&mm;
			T *inv=(T*)&mi;
			float det;

			inv[ 0]= m[ 5]*m[10]*m[15] - m[ 5]*m[11]*m[14] - m[ 9]*m[ 6]*m[15] + m[9]*m[ 7]*m[14] + m[13]*m[ 6]*m[11] - m[13]*m[ 7]*m[10];
			inv[ 4]=-m[ 4]*m[10]*m[15] + m[ 4]*m[11]*m[14] + m[ 8]*m[ 6]*m[15] - m[8]*m[ 7]*m[14] - m[12]*m[ 6]*m[11] + m[12]*m[ 7]*m[10];
			inv[ 8]= m[ 4]*m[ 9]*m[15] - m[ 4]*m[11]*m[13] - m[ 8]*m[ 5]*m[15] + m[8]*m[ 7]*m[13] + m[12]*m[ 5]*m[11] - m[12]*m[ 7]*m[ 9];
			inv[12]=-m[ 4]*m[ 9]*m[14] + m[ 4]*m[10]*m[13] + m[ 8]*m[ 5]*m[14] - m[8]*m[ 6]*m[13] - m[12]*m[ 5]*m[10] + m[12]*m[ 6]*m[ 9];
			inv[ 1]=-m[ 1]*m[10]*m[15] + m[ 1]*m[11]*m[14] + m[ 9]*m[ 2]*m[15] - m[9]*m[ 3]*m[14] - m[13]*m[ 2]*m[11] + m[13]*m[ 3]*m[10];
			inv[ 5]= m[ 0]*m[10]*m[15] - m[ 0]*m[11]*m[14] - m[ 8]*m[ 2]*m[15] + m[8]*m[ 3]*m[14] + m[12]*m[ 2]*m[11] - m[12]*m[ 3]*m[10];
			inv[ 9]=-m[ 0]*m[ 9]*m[15] + m[ 0]*m[11]*m[13] + m[ 8]*m[ 1]*m[15] - m[8]*m[ 3]*m[13] - m[12]*m[ 1]*m[11] + m[12]*m[ 3]*m[ 9];
			inv[13]= m[ 0]*m[ 9]*m[14] - m[ 0]*m[10]*m[13] - m[ 8]*m[ 1]*m[14] + m[8]*m[ 2]*m[13] + m[12]*m[ 1]*m[10] - m[12]*m[ 2]*m[ 9];
			inv[ 2]= m[ 1]*m[ 6]*m[15] - m[ 1]*m[ 7]*m[14] - m[ 5]*m[ 2]*m[15] + m[5]*m[ 3]*m[14] + m[13]*m[ 2]*m[ 7] - m[13]*m[ 3]*m[ 6];
			inv[ 6]=-m[ 0]*m[ 6]*m[15] + m[ 0]*m[ 7]*m[14] + m[ 4]*m[ 2]*m[15] - m[4]*m[ 3]*m[14] - m[12]*m[ 2]*m[ 7] + m[12]*m[ 3]*m[ 6];
			inv[10]= m[ 0]*m[ 5]*m[15] - m[ 0]*m[ 7]*m[13] - m[ 4]*m[ 1]*m[15] + m[4]*m[ 3]*m[13] + m[12]*m[ 1]*m[ 7] - m[12]*m[ 3]*m[ 5];
			inv[14]=-m[ 0]*m[ 5]*m[14] + m[ 0]*m[ 6]*m[13] + m[ 4]*m[ 1]*m[14] - m[4]*m[ 2]*m[13] - m[12]*m[ 1]*m[ 6] + m[12]*m[ 2]*m[ 5];
			inv[ 3]=-m[ 1]*m[ 6]*m[11] + m[ 1]*m[ 7]*m[10] + m[ 5]*m[ 2]*m[11] - m[5]*m[ 3]*m[10] - m[ 9]*m[ 2]*m[ 7] + m[ 9]*m[ 3]*m[ 6];
			inv[ 7]= m[ 0]*m[ 6]*m[11] - m[ 0]*m[ 7]*m[10] - m[ 4]*m[ 2]*m[11] + m[4]*m[ 3]*m[10] + m[ 8]*m[ 2]*m[ 7] - m[ 8]*m[ 3]*m[ 6];
			inv[11]=-m[ 0]*m[ 5]*m[11] + m[ 0]*m[ 7]*m[ 9] + m[ 4]*m[ 1]*m[11] - m[4]*m[ 3]*m[ 9] - m[ 8]*m[ 1]*m[ 7] + m[ 8]*m[ 3]*m[ 5];
			inv[15]= m[ 0]*m[ 5]*m[10] - m[ 0]*m[ 6]*m[ 9] - m[ 4]*m[ 1]*m[10] + m[4]*m[ 2]*m[ 9] + m[ 8]*m[ 1]*m[ 6] - m[ 8]*m[ 2]*m[ 5];

			det=m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];

			if(det==0.0f)
				return MatrixIdentity<T>();

			det=1.0f/det;

			for(int i=0; i<16; i++)
				inv[i]*=det;

			return mi;
			}

		template<typename T> inline float MatrixDeterminant(const TMatrix<T>& mm)
			{
			T *m=(T*)&mm;

			const float da= m[ 5]*m[10]*m[15]-m[ 5]*m[11]*m[14]-m[ 9]*m[ 6]*m[15]+m[9]*m[ 7]*m[14]+m[13]*m[ 6]*m[11]-m[13]*m[ 7]*m[10];
			const float db=-m[ 4]*m[10]*m[15]+m[ 4]*m[11]*m[14]+m[ 8]*m[ 6]*m[15]-m[8]*m[ 7]*m[14]-m[12]*m[ 6]*m[11]+m[12]*m[ 7]*m[10];
			const float dc= m[ 4]*m[ 9]*m[15]-m[ 4]*m[11]*m[13]-m[ 8]*m[ 5]*m[15]+m[8]*m[ 7]*m[13]+m[12]*m[ 5]*m[11]-m[12]*m[ 7]*m[ 9];
			const float dd=-m[ 4]*m[ 9]*m[14]+m[ 4]*m[10]*m[13]+m[ 8]*m[ 5]*m[14]-m[8]*m[ 6]*m[13]-m[12]*m[ 5]*m[10]+m[12]*m[ 6]*m[ 9];

			return m[0]*da+m[1]*db+m[2]*dc+m[3]*dd;
			}

		template<typename T> inline TMatrix<T> MatrixLU(const TMatrix<T>& mm)
			{
			TMatrix<T> mlu;

			T *m=(T*)&mm;
			T *lu=(T*)&mlu;

			/*lu[ 0]=m[ 0];
			lu[ 1]=m[ 1];
			lu[ 2]=m[ 2];
			lu[ 3]=m[ 3];*/
			lu[ 4]= m[ 4]/m[ 0];
			lu[ 8]= m[ 8]/m[ 0];
			lu[12]= m[12]/m[ 0];
			lu[ 5]= m[ 5]-(m[ 4]*m[ 1]);
			lu[ 6]= m[ 6]-(m[ 4]*m[ 2]);
			lu[ 7]= m[ 7]-(m[ 4]*m[ 3]);
			lu[ 9]=(m[ 9]- m[ 8]*m[ 1])/m[ 5];
			lu[10]= m[10]-(m[ 8]*m[ 2]+m[ 9]*m[ 6]);
			lu[11]= m[11]-(m[ 8]*m[ 3]+m[ 9]*m[ 7]);
			lu[13]=(m[13]- m[12]*m[ 1])/m[ 5];
			lu[14]=(m[14]- m[12]*m[ 2]-m[13]*m[ 6])/m[10];
			lu[15]= m[15]-(m[12]*m[ 3]+m[13]*m[ 7]+m[14]*m[15]);

			return mlu;
			}

		template<typename T> inline TMatrix<T> MatrixRotX(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return TMatrix<T>(
				TVector<T>( 1,  0,  0,  0),
				TVector<T>( 0,  c, -s,  0),
				TVector<T>( 0,  s,  c,  0),
				TVector<T>( 0,  0,  0,  1));
			}

		template<typename T> inline TMatrix<T> MatrixRotY(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return TMatrix<T>(
				TVector<T>( c,  0,  s,  0),
				TVector<T>( 0,  1,  0,  0),
				TVector<T>(-s,  0,  c,  0),
				TVector<T>( 0,  0,  0,  1));
			}

		template<typename T> inline TMatrix<T> MatrixRotZ(float ang)
			{
			float s, c;
			sincos(ang*M_PI/180, s, c);

			return TMatrix<T>(
				TVector<T>( c, -s,  0,  0),
				TVector<T>( s,  c,  0,  0),
				TVector<T>( 0,  0,  1,  0),
				TVector<T>( 0,  0,  0,  1));
			}

		template<typename T> inline TMatrix<T> MatrixRotate(float angle, const TVector<T>& dir)
			{
			TMatrix<T> m;

			float s, c;
			sincos(angle*M_PI/180.0f, s, c);

			TVector<T> u=VectorNormalize(dir);
			float uxy=u.x*u.y*(1-c);
			float uyz=u.y*u.z*(1-c);
			float uxz=u.x*u.z*(1-c);
			float ux2=u.x*u.x*(1-c);
			float uy2=u.y*u.y*(1-c);
			float uz2=u.z*u.z*(1-c);
			float uxsin=u.x*s;
			float uysin=u.y*s;
			float uzsin=u.z*s;

			m.row[0]=TVector<T>(c+ux2, uxy+uzsin, uxz-uysin);
			m.row[1]=TVector<T>(uxy-uzsin, c+uy2, uyz+uxsin);
			m.row[2]=TVector<T>(uxz+uysin, uyz-uxsin, c+uz2);
			m.row[3]=TVector<T>(0, 0, 0, 1);

			return m;
			}

		template<typename T> inline TMatrix<T> MatrixTranslate(const TVector<T>& v)
			{
			return TMatrix<T>(
				TVector<T>(1, 0, 0, v.x),
				TVector<T>(0, 1, 0, v.y),
				TVector<T>(0, 0, 1, v.z),
				TVector<T>(0, 0, 0,   1));
			}

		template<typename T> inline TMatrix<T> MatrixSkew(const TVector<T>& a)
			{
			// VectorCross(TVector<T>(), TVector<T>) == VectorTransform(MatrixSkew(TVector<T>()), TVector<T>())

			return TMatrix<T>(
				TVector<T>(   0, -a.z,  a.y, 0),
				TVector<T>( a.z,    0, -a.x, 0),
				TVector<T>(-a.y,  a.x,    0, 0),
				TVector<T>(   0,    0,    0, 1));
			}


		template<typename T> inline TMatrix<T> MatrixZero()
			{
			return TMatrix<T>(
				TVector<T>(0.0f, 0.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, 0.0f, 0.0f));
			}

		template<typename T> inline TMatrix<T> MatrixIdentity()
			{
			return TMatrix<T>(
				TVector<T>(1.0f, 0.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, 1.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, 1.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, 0.0f, 1.0f));
			}

		template<typename T> inline TMatrix<T> MatrixFill(float v)
			{
			return TMatrix<T>(
				TVector<T>(v, v, v, v),
				TVector<T>(v, v, v, v),
				TVector<T>(v, v, v, v),
				TVector<T>(v, v, v, v));
			}

		template<typename T> inline TMatrix<T> MatrixDiagonal(float v)
			{
			return TMatrix<T>(
				TVector<T>(v, 0.0f, 0.0f, 0.0f),
				TVector<T>(0.0f, v, 0.0f, 0.0f),
				TVector<T>(0.0f, 0.0f, v, 0.0f),
				TVector<T>(0.0f, 0.0f, 0.0f, v));
			}
		}
	}
