/*
 * orientation.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include "math.h"
//#include "quaternion.h"

namespace Engine
	{
	namespace Math
		{
		class Orientation
			{
			protected:
				Vector position;
				Vector right;
				Vector forward;
				Vector up;
				Vector scale;

			public:
				Orientation(): position(), right(), forward(), up(), scale() {} //
				Orientation(const Vector& position, const Vector& right, const Vector& up, const Vector& scale=Vector(1, 1, 1)): position(position), right(VectorNormalize(right)), forward(VectorNormalize(up^right)), up(VectorNormalize(up)), scale(scale) {}
				Orientation(const Vector& position, const Orientation& orientation, const Vector& scale=Vector(1, 1, 1)): position(position), right(orientation.right), forward(orientation.forward), up(orientation.up), scale(scale) {}
				Orientation(const Orientation& o): position(o.position), right(o.right), forward(o.forward), up(o.up), scale(o. scale) {}
				virtual ~Orientation() {}

				Orientation operator+(const Vector& v) const {return Orientation(position+v, right, up);}
				Orientation operator-(const Vector& v) const {return Orientation(position-v, right, up);}
				Orientation& operator+=(const Vector& v) {position+=v; return *this;}
				Orientation& operator-=(const Vector& v) {position-=v; return *this;}

				Vector getPosition() const {return position;}
				Vector getRight() const {return right;}
				Vector getForward() const {return forward;}
				Vector getUp() const {return up;}
				Vector getScale() const {return scale;}
				Matrix getMatrix() const {return Matrix(
						right*scale.x,
						up*scale.z,
						-forward*scale.y,
						Vector(0, 0, 0, 1))*
					Matrix(
						Vector(1, 0, 0, 0),
						Vector(0, 1, 0, 0),
						Vector(0, 0, 1, 0),
						Vector(position, 1)
						);}
				//AQuaternion getRotation() const {return rotation;}

				void setPosition(const Vector& pos) {position=pos;}
				void setScale(const Vector& s) {scale=s;}
				void move(const Vector& path) {position+=path;}
				void moveRight(float dist) {position+=getRight()*dist;}
				void moveForward(float dist) {position+=getForward()*dist;}
				void moveUp(float dist) {position+=getUp()*dist;}
				void rotateRight(float angle) {rotate(getRight(), angle);}
				void rotateForward(float angle) {rotate(getForward(), angle);}
				void rotateUp(float angle) {rotate(getUp(), angle);}
				void rotateX(float angle) {rotate(Vector(1.0f, 0.0f, 0.0f), angle);}
				void rotateY(float angle) {rotate(Vector(0.0f, 1.0f, 0.0f), angle);}
				void rotateZ(float angle) {rotate(Vector(0.0f, 0.0f, 1.0f), angle);}
				void rotate(const Vector& axis, float angle) {const Matrix m=MatrixRotate(angle, axis); right=m*right; up=m*up; forward=up^right;}

				void lookAt(const Vector& target, const Vector& from, const Vector& up=Vector(0, 0, 1));
				void lookAt(const Vector& target, float angle, float elevation, float distance, float /*roll*/=0.0f);

				Orientation interpolate(const Orientation& target, float p) const;

				/*Orientation getMoved(const Vector& path)
					{
					Orientation ret(*this);
					ret.move(path);
					return ret;
					}

				Orientation getRotated(const Vector& axis, float angle)
					{
					Orientation ret(*this);
					ret.rotate(axis, angle);
					return ret;
					}*/

				// XXX pozostałe z gatunku getXed
				public:
					static const Orientation FLAT_XY;
					static const Orientation FLAT_XZ;
					static const Orientation FLAT_YZ;
					static const Orientation GUI;
			};

		} /* namespace Graphics */
	} /* namespace Engine */
