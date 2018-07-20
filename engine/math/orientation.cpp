/*
 * orientation.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "orientation.h"

using namespace Engine::Math;

const Orientation Orientation::FLAT_XY=Orientation(Vector(), Vector(1, 0, 0), Vector(0, 1, 0));
const Orientation Orientation::FLAT_XZ=Orientation(Vector(), Vector(1, 0, 0), Vector(0, 0, 1));
const Orientation Orientation::FLAT_YZ=Orientation(Vector(), Vector(0, 1, 0), Vector(0, 0, 1));
const Orientation Orientation::GUI=    Orientation(Vector(), Vector(1, 0, 0), Vector(0,-1, 0));

void Orientation::lookAt(const Vector& target, const Vector& from, const Vector& baseup)
	{
	position=from;
	up=VectorNormalize(baseup);

	forward=VectorNormalize(target-from);
	right=VectorNormalize(forward^up);
	up=VectorNormalize(right^forward);
	}

void Orientation::lookAt(const Vector& target, float angle, float elevation, float distance, float /*roll*/)
	{
	float sa, ca;	// sin, cos; ang
	float se, ce;	// sin, cos; elev

	sincos(-angle/180.0f*M_PI, sa, ca);
	sincos(-elevation/180.0f*M_PI, se, ce);

	forward=VectorNormalize(Vector(sa*ce, ca*ce, se));
	right=Vector(ca, -sa, 0.0f);
	up=VectorNormalize(right^forward);
	right=VectorNormalize(forward^up);

	position=target-forward*distance;
	}

Orientation Orientation::interpolate(const Orientation& target, float p) const
	{
	return Orientation(VectorLerp(position, target.position, p), VectorSlerp(right, target.right, p), VectorSlerp(up, target.up, p));
	}
