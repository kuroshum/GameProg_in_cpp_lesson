#pragma once
#include "Actor.h"
#include "CircleComponent.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

private:
	float mLaserCooldown;
	float mShipCooldown;
	class CircleComponent *mCircle;
};