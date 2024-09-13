/*
DxLib���g�p
���Ɖ~�̓����蔻��
��]�s���p���ē����蔻�����������
*/


#include <math.h>
#include "DxLib.h"

// �֐��v���g�^�C�v�錾
int CheckHit();

// �O���[�o���ϐ���`
int MouseX = 300, MouseY = 300, Size = 10;
int LineLX = 100, LineLY = 100, LineRX = 400, LineRY = 200, LineThickness = 10;

int RotaX, RotaY;
int RotaLineLX, RotaLineLY, RotaLineRX, RotaLineRY;


// main�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// DxLib����������
	if (DxLib_Init() == -1)	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �`���ʂ𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);
	// �}�E�X�̏����ʒu��ݒ�
	SetMousePoint(MouseX, MouseY);

	// ���C�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// ��ʂ��N���A
		ClearDrawScreen();

		// ��(���[�U�[)�̕`��
		DrawLine(LineLX, LineLY, LineRX, LineRY, GetColor(255, 255, 255), LineThickness);

		// �}�E�X���W�擾
		GetMousePoint(&MouseX, &MouseY);
		if (CheckHit() == 0) {	// �����������Ă��Ȃ����
			// �}�E�X���W�ɔ����~��`��
			DrawCircle(MouseX, MouseY, Size, GetColor(255, 255, 255));
		}
		if (CheckHit() == 1) {	// �����������Ă����
			// �}�E�X���W�ɐԂ��~��`��
			DrawCircle(MouseX, MouseY, Size, GetColor(255, 0, 0));
		}

		// ����ʂ�\��ʂɕς���
		ScreenFlip();
	}

	// DxLib�I������
	DxLib_End();

	return 0;
}

// ���[�U�[�Ɖ~�̓����蔻�������
int CheckHit() {

	// ��]�s��ɂ����W�ϊ��ɂ���ē����蔻�����������

	// ���W�ϊ��Ɏg�p���鏔�ϐ�
	int DivX, DivY;
	double theta;

	// �p�x�����߂�
	theta = -atan2(LineLY - LineRY, LineLX - LineRX);
	// ���炷���������߂�
	DivX = LineRX - ((LineRX - LineLX) / 2);
	DivY = LineRY - ((LineRY - LineLY) / 2);

	// ���W����]�s��ɂ���ĉ�]������
	{
		RotaX = (cos(theta) * (MouseX - DivX)) - (sin(theta) * (MouseY - DivY));
		RotaY = (sin(theta) * (MouseX - DivX)) + (cos(theta) * (MouseY - DivY));

		RotaLineLX = (cos(theta) * (LineLX - DivX)) - (sin(theta) * (LineLY - DivY));
		RotaLineLY = (sin(theta) * (LineLX - DivX)) + (cos(theta) * (LineLY - DivY));

		RotaLineRX = (cos(theta) * (LineRX - DivX)) - (sin(theta) * (LineRY - DivY));
		RotaLineRY = (sin(theta) * (LineRX - DivX)) + (cos(theta) * (LineRY - DivY));
	}

	// �����蔻��
	if (abs(RotaY) < (LineThickness / 2) + Size && abs(RotaX) < abs(RotaLineLX) + Size) {	// �������Ă����
		// 1��Ԃ�
		return 1;
	}
	else {	// �������Ă��Ȃ����
		// 0��Ԃ�
		return 0;
	}
}