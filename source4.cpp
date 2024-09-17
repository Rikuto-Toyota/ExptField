/*
DxLib
���^��n�p�`k�_��΂���`�悷��v���O����
VertexNum : ���_�̐�(n)...���E���L�[�ŕύX
SkipVertexNum : ���_�̔�΂���(k)...�㉺���L�[�ŕύX

DrawStar() : ���^��n�p�`k�_��΂���`�悷��֐�
GetRainbowColor() : �F�����炩�ɕω�����v���O����
*/

#include "DxLib.h"
#include <math.h>

// �}�N����`
#define WINDOW_WIDTH 640	// �E�B���h�E�̉���
#define WINDOW_HEIGHT 480	// �E�B���h�E�̗���
#define CENTER_X 640/2	// �E�B���h�E�̒��Sx���W
#define CENTER_Y 480/2	// �E�B���h�E�̒��Sy���W
#define PI 3.141592653	// �~����
#define MIN_VERTEX_NUM 3	// ��n�p�`�̍Œᒸ�_��
#define MAX_VERTEX_NUM 16	// ��n�p�`�̍ő咸�_��
#define MIN_SKIP_VERTEX_NUM 0	// ���^��n�p�`k�_��΂��̍ŏ���΂���
#define MAX_SKIP_VERTEX_NUM 7	// ���^��n�p�`k�_��΂��̍ő��΂���

// �֐��v���g�^�C�v�錾
int DrawStar(int x, int y, int r, double Angle, int Cr, int VertexNum, int SkipVertexNum);
int GetRainbowColor(void);


// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �X�^�[�`��֌W
	int VertexNum;
	int SkipVertexNum;
	int r;
	double Angle;

	// �L�[���͊֌W
	char KeyBuf[256];
	int KeyHitCounter = 0;

	// �X�^�[�����l�ݒ�
	VertexNum = 6;
	SkipVertexNum = 0;
	r = 128;
	Angle = -PI / 2;

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
					VertexNum--;
					if (VertexNum < MIN_VERTEX_NUM) VertexNum = MIN_VERTEX_NUM;
					KeyHitCounter = 16;
				}
				if (KeyBuf[KEY_INPUT_RIGHT] == 1)
				{
					VertexNum++;
					if (VertexNum > MAX_VERTEX_NUM) VertexNum = MAX_VERTEX_NUM;
					KeyHitCounter = 16;
				}
				if (KeyBuf[KEY_INPUT_UP] == 1)
				{
					SkipVertexNum--;
					if (SkipVertexNum < MIN_SKIP_VERTEX_NUM) SkipVertexNum = MIN_SKIP_VERTEX_NUM;
					KeyHitCounter = 16;
				}
				if (KeyBuf[KEY_INPUT_DOWN] == 1)
				{
					SkipVertexNum++;
					if (SkipVertexNum > MAX_SKIP_VERTEX_NUM) SkipVertexNum = MAX_SKIP_VERTEX_NUM;
					KeyHitCounter = 16;
				}
				else
				{

				}
			}
			else {
				KeyHitCounter--;
			}
		}

		DrawPixel(CENTER_X, CENTER_Y, GetColor(255, 255, 255));
		DrawStar(CENTER_X, CENTER_Y, r, Angle, GetRainbowColor(), VertexNum, SkipVertexNum);

		DrawFormatString(0, WINDOW_HEIGHT / 8, GetColor(255, 255, 255), "VertexNum: %d", VertexNum);
		DrawFormatString(0, (WINDOW_HEIGHT * 2) / 8, GetColor(255, 255, 255), "SkipVertexNum: %d", SkipVertexNum);


		Angle += 0.025;
		if (Angle > 2 * PI) {
			Angle = 0.0;
		}
		// ��ʂ�\��
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

// ����`�悷��֐�
int DrawStar(int x, int y, int r, double Angle, int Cr, int VertexNum, int SkipVertexNum) {

	int i;
	int NextVertex;
	double VertexX[MAX_VERTEX_NUM];
	double VertexY[MAX_VERTEX_NUM];

	// ��������̐ݒ�
	if (VertexNum < MIN_VERTEX_NUM) VertexNum = MIN_VERTEX_NUM;
	if (VertexNum > MAX_VERTEX_NUM) VertexNum = MAX_VERTEX_NUM;
	if (SkipVertexNum < MIN_SKIP_VERTEX_NUM) SkipVertexNum = MIN_SKIP_VERTEX_NUM;
	if (SkipVertexNum > MAX_SKIP_VERTEX_NUM) SkipVertexNum = MAX_SKIP_VERTEX_NUM;
	
	// ��n�p�`�̒��_���W���擾
	for (i = 0; i < VertexNum; i++) {
		VertexX[i] = CENTER_X + (r * cos(Angle));
		VertexY[i] = CENTER_Y + (r * sin(Angle));
		Angle += (2 * PI) / VertexNum;
	}

	// ����ꂽ���_���W�z������Ƃɐ��^��n�p�`k�_��΂���`��
	for (i = 0; i < VertexNum; i++) {
		NextVertex = i + (SkipVertexNum + 1);
		if (NextVertex < VertexNum) {
			DrawLineAA(VertexX[i], VertexY[i], VertexX[NextVertex], VertexY[NextVertex], Cr);
		}
		else {
			NextVertex -= VertexNum;
			DrawLineAA(VertexX[i], VertexY[i], VertexX[NextVertex], VertexY[NextVertex], Cr);
		}

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