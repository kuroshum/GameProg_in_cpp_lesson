#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"
#include "Math.h"

Asteroid::Asteroid(Game* game)
	:Actor(game),
	mCircle(nullptr)
{
	// ランダムな位置 / 向きで初期化する
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// スプライトコンポーネントを作成し、テクスチャを設定する
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	
	/*-------------------------追加した箇所-------------------------------------*/

	// 運動コンポーネントを作成し、前進速度を設定する
	mMove = new MoveComponent(this, 0.1f, Vector2(140.0f, 140.0f));
	Vector2 force = Vector2(10.0f, 10.0f);
	mMove->AddForce(force);
	//mc->SetForwardSpeed(150.0f);
	/*-------------------------------ここまで-----------------------------------*/

	// 衝突判定のためにサークルコンポーネントを作成
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}

/*-------------------------追加した箇所-------------------------------------*/

void Asteroid::UpdateActor(float deltaTime)
{
	Vector2 force = Vector2(1.0f, 1.0f);
	mMove->AddForce(force);
}
/*-------------------------------ここまで-----------------------------------*/
