#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	// update orderが小さいほど早く更新される
	InputComponent(class Actor* owner, float mass, Vector2 velocityLimit);
	
	void ProcessInput(const uint8_t* keyState) override;

	// Getters / setters for private variables
	float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

private:
	// 前進 / 回転の最大速度
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	// 前進 / 後退のためのキー
	int mForwardKey;
	int mBackKey;
	// 回転運動のためのキー
	int mClockwiseKey;
	int mCounterClockwiseKey;
};