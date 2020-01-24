// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Enemy.h"
#include "AIComponent.h"
#include "Actor.h"

class Tower : public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

	const float GetAttackRange() { return AttackRange; }

	const float GetPatrolTime() { return PatrolTime; }
	
	float GetNextAttack() { return mNextAttack; }
	void SetNextAttack(float NextAttack) { mNextAttack = NextAttack; }

	const float GetAttackTime() { return AttackTime; }

private:
	class MoveComponent* mMove;
	class AIComponent* mAic;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.0f;
	const float PatrolTime = 10.0f;
};
