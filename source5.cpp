/*
DxLib
�Q�l : https://zenn.dev/kkeeth/articles/rose-curve-variations
�o���Ȑ���`���v���O����
n...���E���L�[�ŕύX
d...�㉺���L�[�ŕύX

DrawRose() : �o���Ȑ���`���֐�
*/

#include "DxLib.h"
#include <math.h>

// �}�N����`
#define WINDOW_WIDTH 640	// �E�B���h�E�̉���
#define WINDOW_HEIGHT 480	// �E�B���h�E�̗���
#define CENTER_X 640/2	// �E�B���h�E�̒��Sx���W
#define CENTER_Y 480/2	// �E�B���h�E�̒��Sy���W
#define PI 3.141592653f	// �~����
#define WAIT_NEXT_KEY_INPUT 4
#define MIN_N 1
#define MAX_N 16
#define MIN_D 1
#define MAX_D 16
#define DEFAULT_THICNESS 0.03125f


// �֐��v���g�^�C�v�錾
int DrawRose(float x, float y, float Size, float Angle, int Cr, float n, float d, float dtheta = DEFAULT_THICNESS, float Thickness = 1.0f);
int GetRainbowColor(void);

// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	float n, d, dtheta;
	float Angle, Size;

	// �L�[���͊֌W
	char KeyBuf[256];
	int KeyHitCounter = 0;

	Size = 256.0f;
	Angle = 0.0f;
	dtheta = 7.0f;
	n = 1.0f;
	d = 1.0f;

	// ��ʃ��[�h�̐ݒ�
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
	ChangeWindowMode(TRUE);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1) return -1;

	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		// ��ʂ��N���A
		ClearDrawScreen();

		// �L�[����
		{
			GetHitKeyStateAll(KeyBuf);

			if (KeyHitCounter == 0) {
				if (KeyBuf[KEY_INPUT_LEFT] == 1)
				{
					n--;
					if (n < MIN_N) n = MIN_N;
					KeyHitCounter = WAIT_NEXT_KEY_INPUT;
				}
				if (KeyBuf[KEY_INPUT_RIGHT] == 1)
				{
					n++;
					if (n > MAX_N) n = MAX_N;
					KeyHitCounter = WAIT_NEXT_KEY_INPUT;
				}
				if (KeyBuf[KEY_INPUT_UP] == 1)
				{
					d--;
					if (d < MIN_D) d = MIN_D;
					KeyHitCounter = WAIT_NEXT_KEY_INPUT;
				}
				if (KeyBuf[KEY_INPUT_DOWN] == 1)
				{
					d++;
					if (d > MAX_D) d = MAX_D;
					KeyHitCounter = WAIT_NEXT_KEY_INPUT;
				}
				else
				{

				}
			}
			else {
				KeyHitCounter--;
			}
		}

		DrawRose(CENTER_X, CENTER_Y, Size, Angle, GetRainbowColor(), n, d, dtheta);
		DrawFormatString(10, 10, GetColor(255, 255, 255), "n = %f, d = %f", n, d);


		Angle += 0.0625f;
		if (Angle > 2.0f * PI) {
			Angle = 0.0f;
		}
		Sleep(60.0);
		// ��ʂ�\��
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

int DrawRose(float x, float y, float Size, float Angle, int Cr, float n, float d, float dtheta, float Thickness) {

	float x1, y1, x2, y2;
	float theta = 0;
	float r;

	while (theta < d * 2 * PI * (dtheta / DEFAULT_THICNESS)) {
		r = sin((n * theta) / d);
		x1 = x + (r * cos(theta + Angle) * Size);
		y1 = y + (r * sin(theta + Angle) * Size);


		theta += dtheta;

		r = sin((n * theta) / d);
		x2 = x + (r * cos(theta + Angle) * Size);
		y2 = y + (r * sin(theta + Angle) * Size);

		DrawLineAA(x1, y1, x2, y2, Cr, Thickness);
	}

	return 0;
}



// �F�����炩�ɕω�������֐�
int GetRainbowColor() {
	static int i = 0;
	static int R = 0, G = 0, B = 255;
	int Cr;

	// RGB��HSV�ɋ^���I�ɕϊ�����
	if (i < 256) {
		Cr = GetColor(R, G, B);
		R++;
	}
	else if (i < 256 * 2) {
		Cr = GetColor(R, G, B);
		B--;
	}
	else if (i < 256 * 3) {
		Cr = GetColor(R, G, B);
		G++;
	}
	else if (i < 256 * 4) {
		Cr = GetColor(R, G, B);
		R--;
	}
	else if (i < 256 * 5) {
		Cr = GetColor(R, G, B);
		B++;
	}
	else if (i < 256 * 6) {
		Cr = GetColor(R, G, B);
		G--;
	}
	else {
		Cr = GetColor(R, G, B);
		i = 0;
	}
	if (R > 255) R = 255;
	if (R < 0) R = 0;
	if (G > 255) G = 255;
	if (G < 0) G = 0;
	if (B > 255) B = 255;
	if (B < 0) B = 0;
	i++;
	return Cr;
}