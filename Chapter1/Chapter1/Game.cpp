#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true)
{

}

bool Game::Initialize() {
	// SDL初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());
		return false;
	}

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)", // ウィンドウのタイトル
		100, // ウィンドウ左上隅のx座標
		100, // ウィンドウ左上隅のy座標
		1024, // ウィンドウの幅(width)
		768, // ウィンドウの高さ(height)
		0 // フラグ(設定しない時は0)
	);

	if (!mWindow) {
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	Ball ball0_info = { {1024.0f / 2.0f, 768.0f / 2.0f}, {-200.0f, 235.0f} };
	Ball ball1_info = { {1024.0f / 4.0f, 768.0f / 2.0f}, {200.0f, 235.0f} };

	mBall.push_back(ball0_info);
	mBall.push_back(ball1_info);


	mPaddlePos[0].x = 10.0f;
	mPaddlePos[0].y = 768.0f / 2.0f;
	mPaddlePos[1].x = 1000.0f;
	mPaddlePos[1].y = 768.0f / 2.0f;

	/*
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	*/

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir[0] = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir[0] += 1;
	}

	mPaddleDir[1] = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddleDir[1] -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddleDir[1] += 1;
	}


}

void Game::UpdateGame() {
	//前のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatimeは前のフレームとの時刻の差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatimeを最大値で制限する
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	/*
	 * 2つのボール用のループ
	 */
	for (int b = 0; b < 2; b++) {
		//mBallPos.x += mBallVel.x * deltaTime;
		//mBallPos.y += mBallVel.y * deltaTime;
		mBall[b].pos.x += mBall[b].vel.x * deltaTime;
		mBall[b].pos.y += mBall[b].vel.y * deltaTime;

		float diff[2] = { 0,0 };
		/*
		 * 2つのパドル用のループ
		 */
		for (int i = 0; i < 2; i++) {
			if (mPaddleDir[i] != 0) {
				mPaddlePos[i].y += mPaddleDir[i] * 300.0f * deltaTime;
				// パドルが画面から出ないようにする
				if (mPaddlePos[i].y < (paddleH / 2.0f + thickness)) {
					mPaddlePos[i].y = paddleH / 2.0f + thickness;
				}
				else if (mPaddlePos[i].y > (768.0f - paddleH / 2.0f - thickness)) {
					mPaddlePos[i].y = 768.0f - paddleH / 2.0f - thickness;
				}
			}


			// Bounce if needed
			// Did we intersect with the paddle?
			diff[i] = mPaddlePos[i].y - mBall[b].pos.y;
			// Take absolute value of difference
			diff[i] = (diff[i] > 0.0f) ? diff[i] : -diff[i];
			if (
				// Our y-difference is small enough
				(diff[i] <= paddleH / 2.0f &&
					// We are in the correct x-position
					mBall[b].pos.x <= 25.0f && mBall[b].pos.x >= 20.0f &&
					// The ball is moving to the left
					mBall[b].vel.x < 0.0f) ||

					(diff[i] <= paddleH / 2.0f &&

						mBall[b].pos.x <= 990.0f && mBall[b].pos.x >= 985.0f &&

						mBall[b].vel.x > 0.0f)
				)
			{
				mBall[b].vel.x *= -1.0f;
			}
			// Did the ball go off the screen? (if so, end game)
			else if (mBall[b].pos.x <= 0.0f || mBall[b].pos.x >= 1024.0f)
			{
				mIsRunning = false;
			}
			// Did the ball collide with the right wall?
			//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
			//{
			//	mBallVel.x *= -1.0f;
			//}

			// Did the ball collide with the top wall?
			if (mBall[b].pos.y <= thickness && mBall[b].vel.y < 0.0f)
			{
				mBall[b].vel.y *= -1;
			}
			// Did the ball collide with the bottom wall?
			else if (mBall[b].pos.y >= (768 - thickness) &&
				mBall[b].vel.y > 0.0f)
			{
				mBall[b].vel.y *= -1;
			}
		}
	}


}

void Game::GenerateOutput() {

	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 255, 255 // (R,G,B,A)
	);

	// Clear back color
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// width
		thickness	// height

	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
	/*
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	*/
	// Draw paddle
	SDL_Rect paddle0{
		static_cast<int>(mPaddlePos[0].x),
		static_cast<int>(mPaddlePos[0].y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle0);

	SDL_Rect paddle1{
		static_cast<int>(mPaddlePos[1].x),
		static_cast<int>(mPaddlePos[1].y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw ball
	SDL_Rect ball0{
		static_cast<int>(mBall[0].pos.x - thickness / 2),
		static_cast<int>(mBall[0].pos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball0);

	// Draw ball
	SDL_Rect ball1{
		static_cast<int>(mBall[1].pos.x - thickness / 2),
		static_cast<int>(mBall[1].pos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball1);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}