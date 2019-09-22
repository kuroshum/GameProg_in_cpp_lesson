#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, int updateOrder)
	:Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(Vector2::Zero),
	mMass(mass),
	mVelocityLimit(velocityLimit)
{

}

void MoveComponent::Update(float deltaTime)
{
	// 回転スピードが0に近ければ角度を更新しない
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	// 前進スピードが0に近ければ位置を更新しない
	if (!Math::NearZero(mSumOfForces.Length()))
	{
		/*-------------------------変更した箇所-------------------------------------*/

		Vector2 pos = mOwner->GetPosition();

		// 力の合計から加速度を更新
		mFowardAcceleration = mSumOfForces * (1.0f / mMass);
		// 速度を更新
		mForwardSpeed += mFowardAcceleration * deltaTime;

		// 速度を制限する
		// Math.h を少しいじりました。
		if (mForwardSpeed > mVelocityLimit)
		{
			mForwardSpeed = mVelocityLimit;
		}

		mSumOfForces = Vector2::Zero;

		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		/*-------------------------------ここまで-----------------------------------*/

		//mForwardSpeed = 0.0f;
		
		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}
}

/*-------------------------追加した箇所-------------------------------------*/

void MoveComponent::AddForce(Vector2& force)
{	
	mSumOfForces += force;
}
/*-------------------------------ここまで-----------------------------------*/
