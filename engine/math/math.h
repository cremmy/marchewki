/*
 * math.h
 *
 *  Created on: 5 sty 2018
 *      Author: crm
 */

#pragma once

#include <cmath>

namespace Engine
	{
	namespace Math
		{
		// Stale
		#undef M_PI
		const double M_PI=3.14159265358979323846;

		// Funkcje
		void sincos(float val, float& s, float& c);

		// inline
		inline void sincos(float val, float& s, float& c)
			{
			/*asm volatile(
				"fsincos\n\t"
				: "=t" (c), "=u" (s)
				: "0" (val)
				);*/
			sincosf(val, &s, &c);
			}
		}
	}

#include "matrix.h"
#include "matrix_ops.h"
#include "vector.h"
#include "vector_ops.h"
#include "quaternion.h"
