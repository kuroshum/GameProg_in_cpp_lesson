// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Bullet.h"
#include "AIState.h"

Tower::Tower(class Game* game)
:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);
	
	mNextAttack = AttackTime;

	// AIComponent‚ğì¬
	mAic = new AIComponent(this);

	// ó‘Ô‚ğAIComponent‚É“o˜^
	mAic->RegisterState(new TowerPatrol(mAic, this));
	mAic->RegisterState(new TowerDeath(mAic, this));
	mAic->RegisterState(new TowerAttack(mAic, this));

	// Patrol‚É‰Šú‰»
	mAic->ChangeState("Patrol");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	


	/*
	if (GetGame()->GetNearestEnemy(GetPosition()) != nullptr)
	{
		// ˆê”Ô‹ß‚¢“G‚ğ•Û‘¶
		mEnemy = GetGame()->GetNearestEnemy(GetPosition());
		
		if (mAic->GetCurrentState()->GetName() == "Attack")
		{
			// Attack‚ÉØ‚è‘Ö‚¦
			mAic->ChangeState("Attack");
		}

		// ƒŠƒLƒƒƒXƒgƒ^ƒCƒ€‚ğŒvZ
		mNextAttack += AttackTime;
	}
	else
	{
		mAic->ChangeState("Patrol");
	}

	
	if (mNextAttack <= 0.0f)
	{
		// ˆê”Ô‹ß‚¢“G‚ğæ“¾
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		
		// “G‚ª‘¶İ‚µ‚½‚ç
		if (e != nullptr)
		{
			// ©•ª‚©‚ç“G‚É‘Î‚·‚éƒxƒNƒgƒ‹‚ğŒvZ
			Vector2 dir = e->GetPosition() - GetPosition();
			// “G‚Æ‚Ì‹——£‚ğŒvZ
			float dist = dir.Length();
			// UŒ‚”ÍˆÍ‚É“G‚ª‚¢‚½‚ç
			if (dist < AttackRange)
			{
				// “G‚Ì•ûŒü‚É•ûŒü“]Š·
				SetRotation(Math::Atan2(-dir.y, dir.x));

				// “ƒ(©•ª)‚ÌÀ•W‚ÉƒoƒŒƒbƒg‚ğ¶¬
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		
	}
	*/
}
