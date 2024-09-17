/*
DxLib
星型正n角形k点飛ばしを描画するプログラム
VertexNum : 頂点の数(n)...左右矢印キーで変更
SkipVertexNum : 頂点の飛ばし方(k)...上下矢印キーで変更

DrawStar() : 星型正n角形k点飛ばしを描画する関数
GetRainbowColor() : 色が滑らかに変化するプログラム
*/

#include "DxLib.h"
#include <math.h>

// マクロ定義
#define WINDOW_WIDTH 640	// ウィンドウの横幅
#define WINDOW_HEIGHT 480	// ウィンドウの立幅
#define CENTER_X 640/2	// ウィンドウの中心x座標
#define CENTER_Y 480/2	// ウィンドウの中心y座標
#define PI 3.141592653	// 円周率
#define MIN_VERTEX_NUM 3	// 正n角形の最低頂点数
#define MAX_VERTEX_NUM 16	// 正n角形の最大頂点数
#define MIN_SKIP_VERTEX_NUM 0	// 星型正n角形k点飛ばしの最小飛ばし数
#define MAX_SKIP_VERTEX_NUM 7	// 星型正n角形k点飛ばしの最大飛ばし数

// 関数プロトタイプ宣言
int DrawStar(int x, int y, int r, double Angle, int Cr, int VertexNum, int SkipVertexNum);
int GetRainbowColor(void);


// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// スター描画関係
	int VertexNum;
	int SkipVertexNum;
	int r;
	double Angle;

	// キー入力関係
	char KeyBuf[256];
	int KeyHitCounter = 0;

	// スター初期値設定
	VertexNum = 6;
	SkipVertexNum = 0;
	r = 128;
	Angle = -PI / 2;

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

		// キー入力
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
		// 画面を表示
		ScreenFlip();
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}

// 星を描画する関数
int DrawStar(int x, int y, int r, double Angle, int Cr, int VertexNum, int SkipVertexNum) {

	int i;
	int NextVertex;
	double VertexX[MAX_VERTEX_NUM];
	double VertexY[MAX_VERTEX_NUM];

	// 上限下限の設定
	if (VertexNum < MIN_VERTEX_NUM) VertexNum = MIN_VERTEX_NUM;
	if (VertexNum > MAX_VERTEX_NUM) VertexNum = MAX_VERTEX_NUM;
	if (SkipVertexNum < MIN_SKIP_VERTEX_NUM) SkipVertexNum = MIN_SKIP_VERTEX_NUM;
	if (SkipVertexNum > MAX_SKIP_VERTEX_NUM) SkipVertexNum = MAX_SKIP_VERTEX_NUM;
	
	// 正n角形の頂点座標を取得
	for (i = 0; i < VertexNum; i++) {
		VertexX[i] = CENTER_X + (r * cos(Angle));
		VertexY[i] = CENTER_Y + (r * sin(Angle));
		Angle += (2 * PI) / VertexNum;
	}

	// 得られた頂点座標配列をもとに星型正n角形k点飛ばしを描画
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

// 色を滑らかに変化させる関数
int GetRainbowColor() {
	static int i = 0;
	static int R = 0, G = 0, B = 255;
	int Cr;

	// RGBをHSVに疑似的に変換する
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