/*
DxLib
バレットとレーザーの描画関数
発光しているっぽく描画する
*/

#include "DxLib.h"
#include <math.h>

// マクロ定義
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// 構造体宣言
typedef struct tagLASER {
	int x1, y1, x2, y2;
	int vx1, vy1, vx2, vy2;
	int Red, Green, Blue;
	int Thickness;
	int vThickness;

}_laser;

typedef struct tagBULLET {
	int x, y;
	int vx, vy;
	int r;
	int Red, Green, Blue;
}_bullet;

// 関数プロトタイプ宣言
int DrawBullet(int x, int y, int r, int Red, int Green, int Blue);
int DrawLaser(int x1, int y1, int x2, int y2, int Red, int Green, int Blue, int Thickness);

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_laser Laser;
	Laser = { 0, WINDOW_HEIGHT / 2, 0, WINDOW_HEIGHT / 2, -32, 0, 32, 0 , 0, 100, 100, 0, 1 };
	_bullet Bullet;
	Bullet = { 0, 0, 8, 8, 16, 100, 0, 100 };


	// 画面モードの設定
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
	ChangeWindowMode(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		// 画面をクリア
		ClearDrawScreen();

		// レーザーの処理
		{
			// レーザーの描画
			DrawLaser(WINDOW_WIDTH / 2 + Laser.x1, Laser.y1, WINDOW_WIDTH / 2 + Laser.x2, Laser.y2, Laser.Red, Laser.Green, Laser.Blue, Laser.Thickness);
			// レーザーを太く，長くする
			Laser.x1 += Laser.vx1;
			Laser.x2 += Laser.vx2;
			Laser.Thickness += Laser.vThickness;
			// 一定の太さを超えたら反転
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

		// バレットの処理
		{
			// バレットの描画
			DrawBullet(Bullet.x, Bullet.y, Bullet.r, Bullet.Red, Bullet.Green, Bullet.Blue);
			// 移動させる
			Bullet.x += Bullet.vx;
			Bullet.y += Bullet.vy;
			// 壁に当たったら反転
			if (Bullet.x < 0) Bullet.vx *= -1;
			if (Bullet.x > WINDOW_WIDTH) Bullet.vx *= -1;
			if (Bullet.y < 0) Bullet.vy *= -1;
			if (Bullet.y > WINDOW_HEIGHT) Bullet.vy *= -1;
		}


		// 画面を表示
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}

int DrawBullet(int x, int y, int r, int Red, int Green, int Blue) {
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	
	// メインの線を描画
	DrawCircle(x, y, r, GetColor(Red, Green, Blue));
	// 内側に明るい線を入れる
	DrawCircle(x, y, r * 0.9, GetColor(Red + 100, Green + 100, Blue + 100));
	DrawCircle(x, y, r * 0.8, GetColor(255, 255, 255));

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return 0;
}
int DrawLaser(int x1, int y1, int x2, int y2, int Red, int Green, int Blue, int Thickness) {
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	// メインの円を描画
	DrawLine(x1, y1, x2, y2, GetColor(Red, Green, Blue), Thickness);
	// 内側に明るい円を入れる
	DrawLine(x1, y1, x2, y2, GetColor(Red + 100, Green + 100, Blue + 100), Thickness*0.85);
	DrawLine(x1, y1, x2, y2, GetColor(255, 255, 255), Thickness*0.7);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return 0;
}