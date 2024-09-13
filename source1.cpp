/*
DxLibを使用
線と円の当たり判定
回転行列を用いて当たり判定を実装する
*/


#include <math.h>
#include "DxLib.h"

// 関数プロトタイプ宣言
int CheckHit();

// グローバル変数定義
int MouseX = 300, MouseY = 300, Size = 10;
int LineLX = 100, LineLY = 100, LineRX = 400, LineRY = 200, LineThickness = 10;

int RotaX, RotaY;
int RotaLineLX, RotaLineLY, RotaLineRX, RotaLineRY;


// main関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// DxLib初期化処理
	if (DxLib_Init() == -1)	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 描画画面を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);
	// マウスの初期位置を設定
	SetMousePoint(MouseX, MouseY);

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		// 画面をクリア
		ClearDrawScreen();

		// 線(レーザー)の描画
		DrawLine(LineLX, LineLY, LineRX, LineRY, GetColor(255, 255, 255), LineThickness);

		// マウス座標取得
		GetMousePoint(&MouseX, &MouseY);
		if (CheckHit() == 0) {	// もし当たっていなければ
			// マウス座標に白い円を描画
			DrawCircle(MouseX, MouseY, Size, GetColor(255, 255, 255));
		}
		if (CheckHit() == 1) {	// もし当たっていれば
			// マウス座標に赤い円を描画
			DrawCircle(MouseX, MouseY, Size, GetColor(255, 0, 0));
		}

		// 裏画面を表画面に変える
		ScreenFlip();
	}

	// DxLib終了処理
	DxLib_End();

	return 0;
}

// レーザーと円の当たり判定をする
int CheckHit() {

	// 回転行列による座標変換によって当たり判定を実装する

	// 座標変換に使用する諸変数
	int DivX, DivY;
	double theta;

	// 角度を求める
	theta = -atan2(LineLY - LineRY, LineLX - LineRX);
	// ずらす長さを求める
	DivX = LineRX - ((LineRX - LineLX) / 2);
	DivY = LineRY - ((LineRY - LineLY) / 2);

	// 座標を回転行列によって回転させる
	{
		RotaX = (cos(theta) * (MouseX - DivX)) - (sin(theta) * (MouseY - DivY));
		RotaY = (sin(theta) * (MouseX - DivX)) + (cos(theta) * (MouseY - DivY));

		RotaLineLX = (cos(theta) * (LineLX - DivX)) - (sin(theta) * (LineLY - DivY));
		RotaLineLY = (sin(theta) * (LineLX - DivX)) + (cos(theta) * (LineLY - DivY));

		RotaLineRX = (cos(theta) * (LineRX - DivX)) - (sin(theta) * (LineRY - DivY));
		RotaLineRY = (sin(theta) * (LineRX - DivX)) + (cos(theta) * (LineRY - DivY));
	}

	// 当たり判定
	if (abs(RotaY) < (LineThickness / 2) + Size && abs(RotaX) < abs(RotaLineLX) + Size) {	// 当たっていらば
		// 1を返す
		return 1;
	}
	else {	// 当たっていなければ
		// 0を返す
		return 0;
	}
}