/*
 * Quaternion.h
 *
 *  Created on: 26 lut 2017
 *      Author: crm
 */

#pragma once

#include "math.h"

#define LOG_STR_QUATERNION(q) "Quaternion " #q ": [%7.4f %7.4f %7.4f %7.4f]", q.r, q.i, q.j, q.k

// QUATERNION_ASSUME_ALWAYS_NORMALIZED - do liczenia odwrotnosci, usuwa dzielenie przez kwadrat normy kwaternionu
// (...bo zakladamy, ze jest rowna 1)
#define QUATERNION_ASSUME_ALWAYS_NORMALIZED

namespace Engine
	{
	namespace Math
		{
		/** Kwaternion
		 * Do obliczania obrotów używa prawej ręki: kciuk w kierunku osi obrotu, zgięte palce pokazują kierunek obrotu wektorów
		 */
		struct Quaternion
			{
			explicit Quaternion(float r=0.0f, float i=0.0f, float j=0.0f, float k=0.0f):
				r(r), i(i), j(j), k(k) {}
			Quaternion(const Quaternion& a): r(a.r), i(a.i), j(a.j), k(a.k) {}

			inline Quaternion& operator=(const Quaternion& a) {r=a.r; i=a.i; j=a.j; k=a.k; return *this;}

			inline operator Matrix() {return matrix();}
			inline Matrix matrix() const
				{
				return Matrix(Vector(1.0f-2.0f*j*j-2.0f*k*k,      2.0f*i*j-2.0f*k*r,      2.0f*i*k+2.0f*j*r, 0.0f),
							   Vector(     2.0f*i*j+2.0f*k*r, 1.0f-2.0f*i*i-2.0f*k*k,      2.0f*j*k-2.0f*i*r, 0.0f),
							   Vector(     2.0f*i*k-2.0f*j*r,      2.0f*j*k+2.0f*i*r, 1.0f-2.0f*i*i-2.0f*j*j, 0.0f),
							   Vector(0.0f, 0.0f, 0.0f, 1.0f));
				}

			float r, i, j, k;
			};

		Quaternion QuaternionAdd(const Quaternion& a, const Quaternion& b);
		Quaternion QuaternionSub(const Quaternion& a, const Quaternion& b);
		Quaternion QuaternionMultiply(const Quaternion& a, const Quaternion& b);
		Quaternion QuaternionNormalize(const Quaternion& a);

		Quaternion QuaternionConjugate(const Quaternion& a);  // Koniunkcja ( a* )
		float QuaternionLength(const Quaternion& a);          // Norma ( ||a|| )
		Quaternion QuaternionReciprocal(const Quaternion& a); // Odwrotnosc ( a^-1 / len^2)

		Vector VectorTransform(const Vector& a, const Quaternion& q);

		Quaternion QuaternionSlerp(const Quaternion& a, const Quaternion& b, float p);

		Quaternion QuaternionFromVectors(const Vector& a, const Vector& b);
		Quaternion QuaternionRotate(float angle, const Vector& dir);

		// operatory
		inline Quaternion operator+(const Quaternion& a, const Quaternion& b) {return QuaternionAdd(a, b);}
		inline Quaternion operator-(const Quaternion& a, const Quaternion& b) {return QuaternionAdd(a, b);}
		inline Quaternion operator*(const Quaternion& a, const Quaternion& b) {return QuaternionMultiply(a, b);}
		inline Quaternion operator*(const Quaternion& a) {return QuaternionConjugate(a);}
		inline Vector operator*(const Quaternion& q, const Vector& a) {return VectorTransform(a, q);}
		inline Quaternion operator*(const Quaternion& a, float v) {return Quaternion(a.r*v, a.i*v, a.j*v, a.k*v);}
		inline Quaternion operator/(const Quaternion& a, float v) {return Quaternion(a.r/v, a.i/v, a.j/v, a.k/v);}
		inline Quaternion operator+(Quaternion& a, const Quaternion& b) {return a=QuaternionAdd(a, b);}
		inline Quaternion operator*=(Quaternion& a, const Quaternion& b) {return a=QuaternionMultiply(a, b);}

		// inline
		inline Quaternion QuaternionAdd(const Quaternion& a, const Quaternion& b)
			{
			return Quaternion(a.r+b.r, a.i+b.i, a.j+b.j, a.k+b.k);
			}

		inline Quaternion QuaternionSub(const Quaternion& a, const Quaternion& b)
			{
			return Quaternion(a.r-b.r, a.i-b.i, a.j-b.j, a.k-b.k);
			}

		inline Quaternion QuaternionMultiply(const Quaternion& a, const Quaternion& b)
			{
			/*return Quaternion(a.k*b.r+a.r*b.k+a.i*b.j-a.j*b.i, // Doooobra... skad ja wytrzasnalem ten *niepoprawny* wzor?
							   a.k*b.i-a.r*b.j+a.i*b.k+a.j*b.r,
							   a.k*b.j+a.r*b.i-a.i*b.r+a.j*b.k,
							   a.k*b.k-a.r*b.r-a.i*b.i-a.j*b.j);*/
			return Quaternion(a.r*b.r - a.i*b.i - a.j*b.j - a.k*b.k,
							   a.r*b.i + a.i*b.r + a.j*b.k - a.k*b.j,
							   a.r*b.j - a.i*b.k + a.j*b.r + a.k*b.i,
							   a.r*b.k + a.i*b.j - a.j*b.i + a.k*b.r);
			}

		inline Quaternion QuaternionNormalize(const Quaternion& a)
			{
			const float len=QuaternionLength(a);

			return Quaternion(a.r/len, a.i/len, a.j/len, a.k/len);
			}

		inline Quaternion QuaternionConjugate(const Quaternion& a)
			{
			return Quaternion(a.r, -a.i, -a.j, -a.k);
			}

		inline float QuaternionLength(const Quaternion& a)
			{
			return sqrt(a.r*a.r + a.i*a.i + a.j*a.j + a.k*a.k);
			}

		inline Quaternion QuaternionReciprocal(const Quaternion& a)
			{
			// a^-1 = a*/||a||^2 // Koniunkcja podzielona przez kwadrat normy

#ifndef QUATERNION_ASSUME_ALWAYS_NORMALIZED
			const float len=QuaternionLength(a);
			const float len2=len*len;

			return Quaternion(a.r/len2, -a.i/len2, -a.j/len2, -a.k/len2);
#else
			return Quaternion(a.r, -a.i, -a.j, -a.k);
#endif
			}

		inline Vector VectorTransform(const Vector& a, const Quaternion& q)
			{
			const Quaternion b=QuaternionMultiply(QuaternionMultiply(q, Quaternion(0.0f, a.x, a.y, a.z)), QuaternionReciprocal(q));

			return Vector(b.i, b.j, b.k, a.w);
			}

		inline Quaternion QuaternionSlerp(const Quaternion& a, const Quaternion& b, float p)
			{
			const float DOTP=(Vector(a.r, a.i, a.j, a.k)|Vector(b.r, b.i, b.j, b.k));
			const float THETA=acos(DOTP)*p;

			if(DOTP>0.9995f)
				return QuaternionNormalize(a+(b-a)*p);

			return (a*cos(THETA)) + QuaternionNormalize(b-a*DOTP)*sin(THETA);
			}

		inline Quaternion QuaternionFromVectors(const Vector& a, const Vector& b)
			{
			// XXX Yhhh... to chyba nie działa najlepiej
			float ablen=sqrt((a|a)*(b|b));
			float qreal=ablen+(a|b);

			Vector w;

			if(qreal<1e-6)
				{
				w=(fabs(a.x)>fabs(a.z))?Vector(-a.y, a.x, 0.0f):Vector(0.0f, -a.z, a.y);
				qreal=0.0f;
				}
			else
				{
				w=a^b;
				}

			Quaternion q(qreal, w.x, w.y, w.z);

			return QuaternionNormalize(q);
			}

		inline Quaternion QuaternionRotate(float angle, const Vector& dir)
			{
			float s, c;

			sincos(angle*M_PI/360.0f, s, c); // *0.5*M_PI/180.0f == *M_PI/360.0f

			return Quaternion(c, dir.x*s, dir.y*s, dir.z*s);
			}
		}
	}
