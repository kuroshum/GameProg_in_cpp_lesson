#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2 {
	float x;
	float y;
};

struct Ball {
	Vector2 pos;
	Vector2 vel;
};

class Game {
public:
	Game();
	//ゲームを初期化する
	bool Initialize();
	//ゲームオーバーまでゲームループを実行する
	void RunLoop();
	//ゲームをシャットダウンする
	void Shutdown();
private:
	// ゲームループのためのヘルパー関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDLが作るウィンドウ
	SDL_Window* mWindow;
	//2D描画のためのレンダリング
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	//ゲームの続行を支持する
	bool mIsRunning;

	// Pong specific
	// Direction of paddle
	int mPaddleDir[2];
	// Position of paddle
	Vector2 mPaddlePos[2];

	std::vector<Ball> mBall;

	/*
	// Position of ball
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;
	*/
};