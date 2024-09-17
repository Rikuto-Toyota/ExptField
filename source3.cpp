/*
DxLib
�o���b�g�ƃ��[�U�[�̕`��֐�
�������Ă�����ۂ��`�悷��
*/

#include "DxLib.h"
#include <math.h>

// �}�N����`
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// �\���̐錾
typedef struct tagLASER {
	int x1, y1, x2, y2;
	int vx1, vy1, vx2, vy2;
	int Cr;
	int Thickness;
	int vThickness;

}_laser;

typedef struct tagBULLET {
	int x, y;
	int vx, vy;
	int r;
	int Cr;
}_bullet;

// �֐��v���g�^�C�v�錾
int DrawBullet(int x, int y, int r, int Cr);
int DrawLaser(int x1, int y1, int x2, int y2, int Cr, int Thickness = 1);

// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_laser Laser;
	Laser = { 0, WINDOW_HEIGHT / 2, 0, WINDOW_HEIGHT / 2, -32, 0, 32, 0 , 0, 0, 1 };
	Laser.Cr = GetColor(0, 100, 100);
	_bullet Bullet;
	Bullet = { 0, 0, 8, 8, 32, 100 };
	Bullet.Cr = GetColor(100, 0, 100);


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

		// ���[�U�[�̏���
		{
			// ���[�U�[�̕`��
			DrawLaser(WINDOW_WIDTH / 2 + Laser.x1, Laser.y1, WINDOW_WIDTH / 2 + Laser.x2, Laser.y2, Laser.Cr, Laser.Thickness);
			// ���[�U�[�𑾂��C��������
			Laser.x1 += Laser.vx1;
			Laser.x2 += Laser.vx2;
			Laser.Thickness += Laser.vThickness;
			// ���̑����𒴂����甽�]
			if (Laser.Thickness > 32) {
				Laser.vx1 *= -1;
				Laser.vx2 *= -1;
				Laser.vThickness *= -1;
			}
			if (Laser.Thickness < 0) {
				Laser.vx1 *= -1;
				Laser.vx2 *= -1;
				Laser.vThickness *= -1;
			}
		}

		// �o���b�g�̏���
		{
			// �o���b�g�̕`��
			DrawBullet(Bullet.x, Bullet.y, Bullet.r, Bullet.Cr);
			// �ړ�������
			Bullet.x += Bullet.vx;
			Bullet.y += Bullet.vy;
			// �ǂɓ��������甽�]
			if (Bullet.x < 0) Bullet.vx *= -1;
			if (Bullet.x > WINDOW_WIDTH) Bullet.vx *= -1;
			if (Bullet.y < 0) Bullet.vy *= -1;
			if (Bullet.y > WINDOW_HEIGHT) Bullet.vy *= -1;
		}


		// ��ʂ�\��
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

int DrawBullet(int x, int y, int r, int Cr) {
	
	// ���C���̐���`��
	DrawCircle(x, y, r, Cr);
	// �����ɖ��邢�~������
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawCircle(x, y, r * 0.9, GetColor(100, 100, 100));
	DrawCircle(x, y, r * 0.8, GetColor(255, 255, 255));

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return 0;
}
int DrawLaser(int x1, int y1, int x2, int y2, int Cr, int Thickness) {

	// ���C���̐���`��
	DrawLine(x1, y1, x2, y2, Cr, Thickness);
	// �����ɖ��邢��������
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawLine(x1, y1, x2, y2, GetColor(100, 100, 100), Thickness*0.85);
	DrawLine(x1, y1, x2, y2, GetColor(255, 255, 255), Thickness*0.7);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return 0;
}