/*
 * unit.cpp
 *
 *  Created on: 25 lip 2018
 *      Author: crm
 */

#include "unit.h"

#include "../engine/render/render.h"

#include "level.h"
#include "turret.h"

using namespace Game;

void Unit::print(float tinterp)
	{
	using namespace Engine::Math;
	using namespace Engine::Render;

	if(const Level::Field* field=((const Level*)level)->getField(level->getPositionOnField(position)))
		{
		if(field->turret && (field->turret->getType()==TurretType::PLAYER_BASE || field->turret->getType()==TurretType::ENEMY_SPAWNER))
			return;
		}

	const Camera& cam=*Render::getInstance().getCurrentCamera();

	/*const Vector directionRelative=VectorNormalize(cam.getView()*(target-position));
	const float angle=(directionRelative.x>0.0f)?(360.0f-acos(directionRelative.y)*180.0f/M_PI) : (acos(directionRelative.y)*180.0f/M_PI);

	sprite.setDirection(Engine::Graphics::Sprite::DirectionName::UP+angle);*/
	if(((target-position)|cam.getRight())<0.0f)
		sprite.setDirection(Engine::Graphics::Sprite::DirectionName::LEFT);
	else
		sprite.setDirection(Engine::Graphics::Sprite::DirectionName::RIGHT);

	if(damageTimer>0.0f)
		{
		Render::getInstance().setColor(Vector(1.0f, 0.25f, 0.25f, 1.0f));
		Render::getInstance().draw(cam.getBillboard(position), sprite);
		Render::getInstance().setColor(Vector(1, 1, 1, 1));
		}
	else
		{
		Render::getInstance().draw(cam.getBillboard(position), sprite);
		}

	if(hp>0.0f && hp<maxhp)
		{
		const int HP_BAR_WIDTH=sprite.getCurrentFrame().getWidth();
		const int HP_BAR_HEIGHT=4;

		const std::vector<Vector> verticesBG(
			{
			Vector(position.x - HP_BAR_WIDTH/2, position.y - HP_BAR_HEIGHT/2, position.z-0.1f),
			Vector(position.x + HP_BAR_WIDTH/2, position.y - HP_BAR_HEIGHT/2, position.z-0.1f),
			Vector(position.x + HP_BAR_WIDTH/2, position.y + HP_BAR_HEIGHT/2, position.z-0.1f),
			Vector(position.x - HP_BAR_WIDTH/2, position.y + HP_BAR_HEIGHT/2, position.z-0.1f),
			});
		const std::vector<Vector> verticesHP(
			{
			Vector(position.x - HP_BAR_WIDTH/2, position.y - HP_BAR_HEIGHT/2, position.z),
			Vector(position.x - HP_BAR_WIDTH/2 + HP_BAR_WIDTH*(hp/maxhp), position.y - HP_BAR_HEIGHT/2, position.z),
			Vector(position.x - HP_BAR_WIDTH/2 + HP_BAR_WIDTH*(hp/maxhp), position.y + HP_BAR_HEIGHT/2, position.z),
			Vector(position.x - HP_BAR_WIDTH/2, position.y + HP_BAR_HEIGHT/2, position.z),
			});

		const Vector color=VectorLerp(Vector(1, 0, 0, 0.75f), Vector(0, 1, 0, 0.9f), hp/maxhp);
		const Vector outline=Vector(1, 1, 1, 1);
		const Vector background=Vector(0, 0, 0, 0.75f);

		Render::getInstance().drawPolygon(verticesBG, outline, background);
		Render::getInstance().drawPolygon(verticesHP, Vector(0, 0, 0, 0), color);
		}
	}

