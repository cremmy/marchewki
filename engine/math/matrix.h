/*
 * amatrix.h
 *
 *  Created on: 5 sty 2018
 *      Author: crm
 */

#pragma once

#include "vector.h"

#define LOG_STR_MATRIX(m) "Matrix " #m ":\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]\n    [%5.2f %5.2f %5.2f %5.2f]",\
		m[0][0], m[0][1], m[0][2], m[0][3],\
		m[1][0], m[1][1], m[1][2], m[1][3],\
		m[2][0], m[2][1], m[2][2], m[2][3],\
		m[3][0], m[3][1], m[3][2], m[3][3]
#define LOG_STR_MATRIXI(m) "Matrix " #m ":\n    [%5d %5d %5d %5d]\n    [%5d %5d %5d %5d]\n    [%5d %5d %5d %5d]\n    [%5d %5d %5d %5d]",\
		m[0][0], m[0][1], m[0][2], m[0][3],\
		m[1][0], m[1][1], m[1][2], m[1][3],\
		m[2][0], m[2][1], m[2][2], m[2][3],\
		m[3][0], m[3][1], m[3][2], m[3][3]

namespace Engine
	{
	namespace Math
		{
		template <typename T>
		struct TMatrix
			{
			// Row-major
			explicit TMatrix(const TVector<T>& a=TVector<T>(), const TVector<T>& b=TVector<T>(), const TVector<T>& c=TVector<T>(), const TVector<T>& d=TVector<T>())
				{row[0]=a; row[1]=b; row[2]=c; row[3]=d;}
			TMatrix(const TMatrix<T>& m) {row[0]=m.row[0]; row[1]=m.row[1]; row[2]=m.row[2]; row[3]=m.row[3];}

			TMatrix<T>& operator=(const TMatrix<T>& m) {row[0]=m.row[0]; row[1]=m.row[1]; row[2]=m.row[2]; row[3]=m.row[3]; return *this;}
			TVector<T>& operator[](int i)
				{
				switch(i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						return row[i];
					default:
						return row[3];
					}
				}
			const TVector<T>& operator[](int i) const
				{
				switch(i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						return row[i];
					default:
						return row[3];
					}
				}

			TVector<T> row[4];
			};

		// Typedefy
		typedef TMatrix<float> Matrix;
		typedef TMatrix<float> MatrixF;
		typedef TMatrix<int> MatrixI;
		}
	}
