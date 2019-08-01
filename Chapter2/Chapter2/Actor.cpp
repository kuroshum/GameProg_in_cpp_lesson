#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game) :mState(EActive), 
						mPosition(Vector2::Zero), 
						mScale(1.0f), 
						mRotation(0.0f), 
						mGame(game) 
{
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);
	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime) {

}

void Actor::AddComponent(Component* component) {
	// ソート済みの配列で挿入点を見つける
	// (それは自分よりも順序の高い最初の要素の位置)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// イテレータの位置の前に要素を挿入する
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}