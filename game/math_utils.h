/*
 * math_utils.h
 *
 *  Created on: 24 lip 2018
 *      Author: crm
 */

#pragma once

#include "../engine/math/math.h"

namespace Game
	{
	namespace MathUtils
		{
		inline Engine::Math::Vector getPositionAtZ0ByRay(const Engine::Math::Vector& raypos, const Engine::Math::Vector& raydir)
			{
			const float Z_DISTANCE=raypos.z/raydir.z;
			return raypos-raydir*Z_DISTANCE;
			}

		inline int taxiDistance(const Engine::Math::VectorI& a, const Engine::Math::VectorI& b)
			{
			return std::abs(a.x-b.x)+std::abs(a.y-b.y);
			};
		}
	}


