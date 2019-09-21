#pragma once
#include "Actor.h"
#include "MoveComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	/*-------------------------í«â¡ÇµÇΩâ”èä-------------------------------------*/

	void UpdateActor(float deltaTime);
	/*-------------------------------Ç±Ç±Ç‹Ç≈-----------------------------------*/

	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;

	/*-------------------------í«â¡ÇµÇΩâ”èä-------------------------------------*/

	class MoveComponent* mMove;

	/*-------------------------------Ç±Ç±Ç‹Ç≈-----------------------------------*/

};