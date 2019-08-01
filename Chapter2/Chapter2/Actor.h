#pragma once
#include <vector>
#include "Math.h"

class Actor {
public:
	// アクターの状態管理用
	enum State {
		EActive,
		EPaused,
		EDead
	};

	// コンストラクタとデストラクタ
	Actor(class Game* game);
	virtual ~Actor();

	// ゲームから呼び出される更新関数(オーバーライド不可)
	void Update(float deltatime);
	// アクターが持つ全コンポーネントを更新(オーバーライド不可)
	void UpdateComponents(float deltatime);
	// アクター独自の更新処理(オーバーライド可能)
	virtual void UpdateActor(float deltatime);

	// ゲッター / セッター
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// コンポーネントの追加 / 消去
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// アクターの状態
	State mState;

	// 座標変換
	Vector2 mPosition;	// アクターの中心位置
	float mScale;		// アクターのスケール (1.0fが100%)
	float mRotation;	// 回転の角度 (ラジアン)
	
	// アクターが持つコンポーネント
	std::vector<class Component*> mComponents;
	class Game* mGame;
};