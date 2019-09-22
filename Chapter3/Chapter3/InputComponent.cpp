#include "InputComponent.h"
#include "Actor.h"
#include "Math.h"

InputComponent::InputComponent(class Actor* owner, float mass, Vector2 velocityLimit)
	:MoveComponent(owner, mass, velocityLimit),
	mForwardKey(0),
	mBackKey(0),
	mClockwiseKey(0),
	mCounterClockwiseKey(0)
{

}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// MoveComponentのための前進速度を計算
	float forwardSpeed = 0.0f;
	
	/*-------------------------変更した箇所-------------------------------------*/

	Vector2 force = Vector2::Zero;
	if (keyState[mForwardKey])
	{
		//forwardSpeed += mMaxForwardSpeed;
		force = Vector2(50.0f, 50.0f);
	}
	else if (keyState[mBackKey])
	{
		//forwardSpeed -= mMaxForwardSpeed;
		force = Vector2(-10.0f, -10.0f);
	}
	else
	{
		SetForwardSpeed(Vector2::Zero);
	}
	AddForce(force);
	//SetForwardSpeed(forwardSpeed);
	
	/*-------------------------------ここまで-----------------------------------*/

	// Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}