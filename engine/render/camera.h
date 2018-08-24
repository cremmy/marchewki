/*
 * camera.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include "../math/orientation.h"

namespace Engine
	{
	namespace Render
		{
		class Camera
			{
			public:
				enum class Type
					{
					PERSPECTIVE,
					ORTHOGRAPHIC,
					GUI,
					POSTPROCESS,
					};

			protected:
				Math::Orientation orientation;
				Type type;

				float fov;
				float scale;

				float width;
				float height;

				float cutnear;
				float cutfar;

				Math::Matrix projection;
				mutable Math::Matrix view;

				mutable bool needupdate;

				void updateProjectionMatrix();
				void updateViewMatrix() const;

				//bool intposition;

			public:
				Camera(): orientation(), type(Type::ORTHOGRAPHIC), fov(60.0f), scale(1.0f), width(1.0f), height(1.0f), cutnear(0.0f), cutfar(1000.0f), needupdate(true) {/*updateProjectionMatrix(); updateViewMatrix();*/}
				~Camera() {}

				bool perspective(float width, float height, float cutnear=1.0, float cutfar=1000.0f, float fov=60.0f);
				bool ortho(float width, float height, float cutnear=0.0, float cutfar=1000.0f);
				bool GUI(float width, float height);
				bool postprocess();

				Math::Vector getPosition() const {return orientation.getPosition();}
				Math::Vector getRight() const {return orientation.getRight();}
				Math::Vector getForward() const {return orientation.getForward();}
				Math::Vector getUp() const {return orientation.getUp();}
				//Math::AQuaternion getRotation() const {return orientation.getRotation();}

				const Math::Matrix& getProjection() const {return projection;}
				//const Math::Matrix& getView() const {return view;}
				const Math::Matrix& getView() const {if(needupdate) updateViewMatrix(); return view;}

				Math::Orientation getBillboard(const Math::Vector& pos) const;
				Math::Vector getPointOnScreen(const Math::Vector& pos) const;
				void getRay(int mx, int my, Math::Vector& raypos, Math::Vector& raydir) const;

				void setScale(float s) {scale=s; needupdate=true;}
				void setFOV(float s) {fov=s; updateProjectionMatrix();}

				void setPosition(const Math::Vector& pos) {orientation.setPosition(pos); needupdate=true;}
				void setOrientation(const Math::Orientation& ori) {orientation=ori; needupdate=true;}
				void move(const Math::Vector& path) {orientation.move(path); needupdate=true;}
				void moveRight(float dist) {orientation.moveRight(dist); needupdate=true;}
				void moveForward(float dist) {orientation.moveForward(dist); needupdate=true;}
				void moveUp(float dist) {orientation.moveUp(dist); needupdate=true;}
				void rotateRight(float angle) {orientation.rotateRight(angle); needupdate=true;}
				void rotateForward(float angle) {orientation.rotateForward(angle); needupdate=true;}
				void rotateUp(float angle) {orientation.rotateUp(angle); needupdate=true;}
				void rotateX(float angle) {orientation.rotateX(angle); needupdate=true;}
				void rotateY(float angle) {orientation.rotateY(angle); needupdate=true;}
				void rotateZ(float angle) {orientation.rotateZ(angle); needupdate=true;}
				void rotate(const Math::Vector& axis, float angle) {orientation.rotate(axis, angle); needupdate=true;}

				void lookAt(const Math::Vector& target, const Math::Vector& from, const Math::Vector& up=Math::Vector(0, 0, 1)) {orientation.lookAt(target, from, up); updateViewMatrix();}
				void lookAt(const Math::Vector& target, float angle, float elevation, float distance, float roll=0.0f) {orientation.lookAt(target, angle, elevation, distance, roll); updateViewMatrix();}

				/*void movePathHermite(const Math::Vector& from, const Math::Vector& handlea, const Math::Vector& target, const Math::Vector& hangleb, float t);
				void movePathBezier(const Math::Vector& from, const Math::Vector& handlea, const Math::Vector& target, const Math::Vector& hangleb, float t);
				void movePathBSpline(const Math::Vector& from, const Math::Vector& handlea, const Math::Vector& target, const Math::Vector& hangleb, float t);
				void movePathNURBS(const Math::Vector& from, const Math::Vector& handlea, const Math::Vector& target, const Math::Vector& hangleb, float t);*/
			};

		} /* namespace Graphics */
	} /* namespace Engine */
