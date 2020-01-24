// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Tower.h"
#include "Enemy.h"
#include "Grid.h"
#include "Game.h"
#include "Tile.h"

#define PATROL_TIME 5.0f

class AIState
{
public:
	AIState(class AIComponent* owner)
		:mOwner(owner)
		,mTower(nullptr)
		,mEnemy(nullptr)
	{ }
	// State-specific behavior
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	// Getter for string name of state
	virtual const char* GetName() const = 0;
protected:
	class Enemy* mEnemy;
	class Tower* mTower;
	class AIComponent* mOwner;
};

/*=================================================================*/
// 塔のためのステート

class EnemyState : public AIState
{
public:
	EnemyState(class AIComponent* owner, class Enemy* enemy)
		:AIState(owner)
	{
		mEnemy = enemy;
	}
};

class EnemyPatrol : public EnemyState
{
public:
	EnemyPatrol(class AIComponent* owner, class Enemy* enemy)
		:EnemyState(owner, enemy)
	{
	}

	// Override with behaviors for this state
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Patrol";
	}
};

class EnemyDeath : public EnemyState
{
public:
	EnemyDeath(class AIComponent* owner, class Enemy* enemy)
		:EnemyState(owner, enemy)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Death";
	}
};

/*=================================================================*/

/*=================================================================*/
// 塔のためのステート

class TowerState : public AIState
{
public:
	TowerState(class AIComponent* owner, class Tower* tower)
		:AIState(owner)
	{
		mTower = tower;
	}
protected:
	float PatrolTime = PATROL_TIME;
};

class TowerPatrol : public TowerState
{
public:
	TowerPatrol(class AIComponent* owner, class Tower* tower)
		:TowerState(owner, tower)
	{
	}

	// Override with behaviors for this state
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Patrol"; }
};

class TowerDeath : public TowerState
{
public:
	TowerDeath(class AIComponent* owner, class Tower* tower)
		:TowerState(owner, tower)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Death"; }
};

class TowerAttack : public TowerState
{
public:
	TowerAttack(class AIComponent* owner, class Tower* tower)
		:TowerState(owner, tower)
	{
	}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{ return "Attack"; }

};
/*=================================================================*/

