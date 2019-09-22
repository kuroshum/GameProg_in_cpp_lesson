// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mShipCooldown(2.0f)
	, mCircle(nullptr)
{
	// スプライトコンポーネントを作成
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// インプットコンポーネントとキーとスピードを作成
	InputComponent* ic = new InputComponent(this, 1.0f, Vector2(200.0f, 200.0f));
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// サークルコンポーネントを作成
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	/*-------------------------追加した箇所-------------------------------------*/
	
	// 状態がEActiveなら衝突判定を計算
	if (GetState() == EActive)
	{
		// アステロイドと衝突したかを判定
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// 衝突したら状態をEPausedに設定
				SetState(EPaused);
				break;
			}
		}
	}
	
	// 状態がEPausedならリスポーンするかを計算
	if (GetState() == EPaused)
	{	
		// リスポーンのクールダウン時間を計算
		mShipCooldown -= deltaTime;
		
		// クールダウン時間になったらshipを初期位置・角度にリスポーン 
		if (mShipCooldown <= 0.0f)
		{
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(Math::PiOver2);
			SetState(EActive);
			mShipCooldown = 2.f;
		}
	}
	/*-------------------------------ここまで-----------------------------------*/
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
