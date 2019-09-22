#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// UpdateOrderの値が小さいほど、先に更新される
	MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, int updateOrder = 10);

	void Update(float deltaTime) override;

	/*-------------------------追加した箇所-------------------------------------*/

	void AddForce(Vector2& force);
	/*-------------------------------ここまで-----------------------------------*/

	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	
	/*-------------------------変更した箇所-------------------------------------*/
	Vector2 GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(Vector2 speed) { mForwardSpeed = speed; }

	Vector2 GetSumOfForces() const { return mSumOfForces; }
	void SetSumOfForces(Vector2 sumOfFoces) { mSumOfForces = sumOfFoces; }
	/*-------------------------------ここまで-----------------------------------*/



private:
	// 回転を制御する (ラジアン / 秒)
	float mAngularSpeed;
	
	/*-------------------------変更した箇所-------------------------------------*/

	// 前進運動を制御する (単位長 / 秒)
	Vector2 mForwardSpeed;

	/*-------------------------------ここまで-----------------------------------*/


	/*-------------------------追加した箇所-------------------------------------*/
	// 質量
	float mMass;
	// 力の合計
	Vector2 mSumOfForces;
	// 速度
	Vector2 mVelocity;
	// 制限速度
	Vector2 mVelocityLimit;
	// 加速度
	Vector2 mFowardAcceleration;
	/*-------------------------------ここまで-----------------------------------*/
};