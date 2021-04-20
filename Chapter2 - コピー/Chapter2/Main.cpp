// サンプルプログラム:
// SDL2でアプリケーションウィンドウを生成する

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

	SDL_Window *window;                    // ポインタを宣言する

	SDL_Init(SDL_INIT_VIDEO);              // SDL2を初期化する

	// 次の設定でアプリケーションウィンドウを生成する:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // ウィンドウのタイトル
		SDL_WINDOWPOS_UNDEFINED,           // X座標の初期値
		SDL_WINDOWPOS_UNDEFINED,           // Y座標の初期値
		640,                               // 幅のピクセル数
		480,                               // 高さのピクセル数
		SDL_WINDOW_OPENGL                  // フラグ
	);

	// ウィンドウの生成に成功したかチェックする
	if (window == NULL) {
		// ここを通ったならばウィンドウを生成できなかった...
		printf("ウィンドウを生成できなかった: %s\n", SDL_GetError());
		return 1;
	}

	// ウィンドウが開いた: ここでプログラムループに入る (SDL_PollEvent()を参照すること)

	SDL_Delay(3000);  // 例として3000ミリ秒間停止する

	// ウィンドウを閉じて破棄する
	SDL_DestroyWindow(window);

	// 終了処理
	SDL_Quit();
	return 0;
}