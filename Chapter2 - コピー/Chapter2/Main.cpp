// �T���v���v���O����:
// SDL2�ŃA�v���P�[�V�����E�B���h�E�𐶐�����

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

	SDL_Window *window;                    // �|�C���^��錾����

	SDL_Init(SDL_INIT_VIDEO);              // SDL2������������

	// ���̐ݒ�ŃA�v���P�[�V�����E�B���h�E�𐶐�����:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // �E�B���h�E�̃^�C�g��
		SDL_WINDOWPOS_UNDEFINED,           // X���W�̏����l
		SDL_WINDOWPOS_UNDEFINED,           // Y���W�̏����l
		640,                               // ���̃s�N�Z����
		480,                               // �����̃s�N�Z����
		SDL_WINDOW_OPENGL                  // �t���O
	);

	// �E�B���h�E�̐����ɐ����������`�F�b�N����
	if (window == NULL) {
		// ������ʂ����Ȃ�΃E�B���h�E�𐶐��ł��Ȃ�����...
		printf("�E�B���h�E�𐶐��ł��Ȃ�����: %s\n", SDL_GetError());
		return 1;
	}

	// �E�B���h�E���J����: �����Ńv���O�������[�v�ɓ��� (SDL_PollEvent()���Q�Ƃ��邱��)

	SDL_Delay(3000);  // ��Ƃ���3000�~���b�Ԓ�~����

	// �E�B���h�E����Ĕj������
	SDL_DestroyWindow(window);

	// �I������
	SDL_Quit();
	return 0;
}