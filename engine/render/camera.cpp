/*
 * camera.cpp
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#include "camera.h"

#include "../debug/log.h"

using namespace Engine::Render;

bool Camera::perspective(float width, float height, float cutnear, float cutfar, float fov)
	{
	this->type=Type::PERSPECTIVE;
	this->fov=fov;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=cutnear;
	this->cutfar=cutfar;

	updateProjectionMatrix();

	return true;
	}

bool Camera::ortho(float width, float height, float cutnear, float cutfar)
	{
	this->type=Type::ORTHOGRAPHIC;
	this->fov=0.0f;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=cutnear;
	this->cutfar=cutfar;

	updateProjectionMatrix();

	return true;
	}

bool Camera::GUI(float width, float height)
	{
	this->type=Type::GUI;
	this->fov=0.0f;
	this->scale=1.0f;
	this->width=width;
	this->height=height;
	this->cutnear=-1.0f;
	this->cutfar=1.0f;

	updateProjectionMatrix();

	return true;
	}

bool Camera::postprocess()
	{
	this->type=Type::POSTPROCESS;
	this->fov=0.0f;
	this->scale=1.0f;
	this->width=2.0f;
	this->height=2.0f;
	this->cutnear=-1.0f;
	this->cutfar=1.0f;

	updateProjectionMatrix();
	return true;
	}

void Camera::updateProjectionMatrix()
	{
	using namespace Math;

	const float N=cutnear;
	const float F=cutfar;

	switch(type)
		{
		case Type::ORTHOGRAPHIC:
			{
			const float TZ=-(F+N)/(F-N);

			this->projection=Matrix(
					Vector(2.0f/width,           0,        0,  0),
					Vector(         0, 2.0f/height,        0,  0),
					Vector(         0,           0, -2/(F-N), TZ),
					Vector(         0,           0,        0,  1)
					);
			}
		break;

		case Type::PERSPECTIVE:
			{
			const float ASPECT=(float)width/height;
			const float FH=tan(fov/360.0*M_PI);
			const float FW=FH*ASPECT;
			const float C=-(F+N)/(F-N);
			const float D=-(2*F*N)/(F-N);

			this->projection=Matrix(
					Vector(1/FW, 0, 0, 0),
					Vector(0, 1/FH, 0, 0),
					Vector(0, 0,  C, D),
					Vector(0, 0, -1, 0)
					);
			}
		break;

		case Type::GUI:
			{
			const float R=width;
			const float L=0;
			const float T=0;
			const float B=height;
			const float TX=-(R+L)/(R-L);
			const float TY=-(T+B)/(T-B);
			const float TZ=-(F+N)/(F-N);

			this->projection=Matrix(
					Vector( 2/(R-L), 0, 0, TX),
					Vector(0,  2/(T-B), 0, TY),
					Vector(0, 0, -2/(F-N), TZ),
					Vector(0, 0, 0, 1)
					);
			}
		break;

		default:
		case Type::POSTPROCESS:
			this->projection=MatrixIdentity<float>();
		break;
		}
	}

void Camera::updateViewMatrix() const
	{
	using namespace Math;

	needupdate=false;

	if(type==Type::GUI || type==Type::POSTPROCESS)
		{
		this->view=Matrix(
				Vector(1, 0, 0, 0),
				Vector(0, 1, 0, 0),
				Vector(0, 0, 1, 0),
				Vector(0, 0, 0, 1));
		}
	else
		{
		this->view=Matrix(
				orientation.getRight()*scale,
				orientation.getUp()*scale,
				-orientation.getForward()*scale,
				Vector(0, 0, 0, 1))*
			Matrix(
				Vector(1, 0, 0, -orientation.getPosition().x),
				Vector(0, 1, 0, -orientation.getPosition().y),
				Vector(0, 0, 1, -orientation.getPosition().z),
				Vector(0, 0, 0, 1)
				);
		}
	}


Engine::Math::Orientation Camera::getBillboard(const Engine::Math::Vector& objpos) const
	{
	using namespace Math;

	switch(type)
		{
		case Type::ORTHOGRAPHIC:
			{
			const Vector f=VectorNormalize(Vector(getForward().x, getForward().y, 0.0f));
			const float ce=VectorDot(getForward(), f);

			if(ce<0.1f && ce>-0.1f)
				return Orientation(objpos, Vector(f.y, -f.x, 0.0f), Vector(0, 0, 1), Vector(1, 1, 0));
			return Orientation(objpos, Vector(f.y, -f.x, 0.0f), Vector(0, 0, 1), Vector(1, 1, 1.0f/ce));
			}
		break;

		case Type::PERSPECTIVE:
			{
			const Vector dist=objpos-getPosition();
			const Vector f=VectorNormalize(Vector(dist.x, dist.y, 0.0f));
			const Vector r=Vector(f.y, -f.x, 0.0f);

			return Orientation(objpos, r, Vector(0, 0, 1), Vector(1, 1, 1));
			}
		break;

		default:
		case Type::GUI:
			return Orientation::GUI;
		break;
		}
	}

Engine::Math::Vector Camera::getPointOnScreen(const Engine::Math::Vector& objpos) const
	{
	using namespace Math;

	Vector ret;

	ret=objpos-getPosition();
	ret=getView()*ret;
	ret=getProjection()*ret;
	if(type==Type::PERSPECTIVE)
		ret=ret/Vector(ret.z, ret.z, 1, 1);
	ret.x=(ret.x+1)*width/2;
	ret.y=(ret.y+1)*height/2;
	ret.z=0;
	ret.w=0;

	return ret;
	}

void Camera::getRay(int mx, int my, Engine::Math::Vector& raypos, Engine::Math::Vector& raydir) const
	{
	using namespace Math;

	float x;
	float y;

	if(type==Type::PERSPECTIVE)
		{
		const float aspect=(float)width/height;
		float fw, fh;
		fh=tan(fov/360.0*M_PI);
		fw=fh*aspect;

		x=( mx/width-0.5f)*2.0f;
		y=(-my/height+0.5f)*2.0f;

		x=fw*x;
		y=fh*y;

		raypos=getPosition() + getForward()*cutnear + getRight()*x/scale + getUp()*y/scale;
		raydir=VectorNormalize(raypos-getPosition());
		}
	else
		{
		x=( mx/(float)width-0.5f)*width;
		y=(-my/(float)height+0.5f)*height;

		raypos=getPosition() - getForward()*cutnear + getRight()*x/scale + getUp()*y/scale;
		raydir=getForward();
		}
	}
