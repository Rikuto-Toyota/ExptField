/*
DxLib
属性ダメージっぽい攻撃の実装
キーボード1, 2, 3で属性変化
1: 物理...通常攻撃
2: 凍結...移動速度低下＋継続ダメージ
3: 炎上...継続ダメージ
*/

#include <math.h>
#include "DxLib.h"

// マクロ定義
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define SHOT_NUM 16
#define PARTICLE_NUM 256
#define PI 3.14159

#define NORMAL 0
#define FROSTBITE 1
#define BURN 2
#define NORMAL_INTERVAL 4
#define FROSTBITE_INTERVAL 16
#define BURN_INTERVAL 8
#define NORMAL_DURATION 4
#define FROSTBITE_DURATION 32
#define BURN_DURATION 32


// 構造体宣言
typedef struct tagENEMY {
	int x, y;
	int vx, vy;
	int Size, Cr;
	double MaxSpeed;
	double Angle, Speed;
	int MaxLife;
	int Life, State;
	int DamageAmount, DamageInterval, DamageDuration;
	bool ValidFlag;
}_enemy;

typedef struct tagPLAYER {
	int x, y;
	int vx, vy;
	int Size, Cr;
	double Angle, Speed;
	int Life;
	int ShotProperty, ShotCounter;
	bool ValidFlag;
}_player;

typedef struct tagSHOT {
	int x, y;
	int vx, vy;
	int Size, Cr;
	double Angle, Speed;
	int Damage, Property;
	int DamageInterval, DamageDuration;
	bool ValidFlag;
}_shot;

typedef struct tagPARTICLE {
	int x, y;
	int vx, vy;
	int Cr;
	double Angle, Speed;
	int Counter;
	bool ValidFlag;
}_particle;

// 関数プロトタイプ宣言
int GeneratePlayer(void);
int GenerateEnemy(void);
int GenerateShot(int x, int y, int Property, int DamageInterval, int DamageDuration);
int GenerateParticle(int x, int y, int Property);
int ActCharacter(void);
int ActPlayer(void);
int ActShot(void);
int ActEnemy(void);
int ActParticle(void);
int CheckHit(void);
int DealBadStatus(void);
int DealNormal(void);
int DealFrostbite(void);
int DealBurn(void);
int DealDamage(_enemy* target, int Damage);
int DrawCharacter(void);
int SetPlayerProperty(void);

// グローバル変数
_player player;
_shot shot[SHOT_NUM];
_enemy enemy;
_particle particle[PARTICLE_NUM];
int BadStatusInterval[3] = { NORMAL_INTERVAL, FROSTBITE_INTERVAL, BURN_INTERVAL };
int BadStatusDuration[3] = { NORMAL_DURATION, FROSTBITE_DURATION, BURN_DURATION };
int (*BadStatus[])(void) = {
	DealNormal,DealFrostbite, DealBurn
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	GeneratePlayer();
	GenerateEnemy();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		ClearDrawScreen();
		SetPlayerProperty();

		if (enemy.ValidFlag == FALSE) GenerateEnemy();
		ActCharacter();
		CheckHit();
		DealBadStatus();
		DrawCharacter();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}

int GeneratePlayer() {
	player.x = WINDOW_WIDTH / 5;
	player.y = WINDOW_HEIGHT / 2;
	player.vx = 0;
	player.vy = 0;
	player.Size = 16;
	player.Cr = GetColor(255, 255, 255);
	player.Angle = 0;
	player.Speed = 0;
	player.Life = 128;
	//player.ShotProperty = NORMAL;
	player.ShotProperty = FROSTBITE;
	player.ShotCounter = 0;
	player.ValidFlag = TRUE;

	return 0;
}

int GenerateShot(int x, int y, int Property, int DamageInterval, int DamageDuration) {
	int i;

	for (i = 0; i < SHOT_NUM; i++) {
		if (shot[i].ValidFlag == FALSE) break;
	}
	shot[i].x = x;
	shot[i].y = y;
	shot[i].vx = 0;
	shot[i].vy = 0;
	shot[i].Size = 4;
	if (Property == NORMAL)shot[i].Cr = GetColor(255, 255, 255);
	if (Property == FROSTBITE)shot[i].Cr = GetColor(0, 200, 255);
	if (Property == BURN)shot[i].Cr = GetColor(255, 200, 0);
	shot[i].Angle = 0;
	shot[i].Speed = 16;
	shot[i].Damage = 16;
	shot[i].Property = Property;
	shot[i].DamageInterval = DamageInterval;
	shot[i].DamageDuration = DamageDuration;
	shot[i].ValidFlag = TRUE;

	return 0;
}

int GenerateEnemy() {
	enemy.x = WINDOW_WIDTH;
	enemy.y = WINDOW_HEIGHT / 2;
	enemy.vx = 0;
	enemy.vy = 0;
	enemy.Size = 16;
	enemy.Cr = GetColor(255, 255, 255);
	enemy.Angle = PI;
	enemy.MaxSpeed = 4;
	enemy.Speed = enemy.MaxSpeed;
	enemy.MaxLife = 64;
	enemy.Life = enemy.MaxLife;
	enemy.State = NORMAL;
	enemy.ValidFlag = TRUE;

	return 0;
}

int GenerateParticle(int x, int y, int Property) {
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < PARTICLE_NUM; j++) {
			if (particle[j].ValidFlag == FALSE) break;
		}
		particle[j].x = x;
		particle[j].y = y;;
		particle[j].vx = 0;
		particle[j].vy = 0;
		if (Property == NORMAL)particle[j].Cr = GetColor(255, 255, 255);
		if (Property == FROSTBITE)particle[j].Cr = GetColor(0, 200, 255);
		if (Property == BURN)particle[j].Cr = GetColor(255, 200, 0);
		particle[j].Angle = ((3.0*PI)/2.0) + ((double)GetRand(1570)/1000.0);
		particle[j].Speed = 1 + GetRand(8);
		particle[j].Counter = 8;
		particle[j].ValidFlag = TRUE;
	}

	return 0;
}

int ActCharacter() {
	ActPlayer();
	ActShot();
	ActEnemy();
	ActParticle();

	return 0;
}

int ActPlayer(void) {
	if (player.ShotCounter == 64) {
		GenerateShot(player.x + player.Size, player.y, player.ShotProperty, BadStatusInterval[player.ShotProperty], BadStatusDuration[player.ShotProperty]);
		player.ShotCounter = 0;
	}
	player.ShotCounter++;
	return 0;
}
int ActShot(void) {
	int i;
	
	for (i = 0; i < SHOT_NUM; i++) {
		if (shot[i].ValidFlag == FALSE) continue;

		// 移動処理
		{
			shot[i].vx = cos(shot[i].Angle) * shot[i].Speed;
			shot[i].vy = sin(shot[i].Angle) * shot[i].Speed;

			shot[i].x += shot[i].vx;
			shot[i].y += shot[i].vy;
		}

		// 画面買いに出ていたら消去
		if (shot[i].x < 0 || shot[i].x > WINDOW_WIDTH || shot[i].y < 0 || shot[i].y > WINDOW_HEIGHT) {
			shot[i].ValidFlag = FALSE;
		}
	}

	return 0;
}
int ActEnemy(void) {

	enemy.vx = cos(enemy.Angle) * enemy.Speed;
	enemy.vy = sin(enemy.Angle) * enemy.Speed;

	enemy.x += enemy.vx;
	enemy.y += enemy.vy;

	// 体力が0ならば消去
	if (enemy.Life < 1) {
		enemy.ValidFlag = FALSE;
	}

	// 画面買いに出ていたら消去
	if (enemy.x < 0 || enemy.x > WINDOW_WIDTH || enemy.y < 0 || enemy.y > WINDOW_HEIGHT) {
		enemy.ValidFlag = FALSE;
	}

	return 0;
}

int ActParticle(void) {
	int i;

	for (i = 0; i < PARTICLE_NUM; i++) {
		if (particle[i].ValidFlag == FALSE) continue;

		particle[i].vx = cos(particle[i].Angle) * particle[i].Speed;
		particle[i].vy = sin(particle[i].Angle) * particle[i].Speed;

		particle[i].x += particle[i].vx;
		particle[i].y += particle[i].vy;

		particle[i].Counter--;
		if (particle[i].Counter < 0) {
			particle[i].ValidFlag = FALSE;
		}
	}


	return 0;
}

int CheckHit() {
	int i;
	double d;

	if (enemy.ValidFlag == TRUE) {
		for (i = 0; i < SHOT_NUM; i++) {
			if (shot[i].ValidFlag == FALSE) continue;

			d = pow((pow((double)(enemy.x - shot[i].x), 2.0) + pow((double)(enemy.y - shot[i].y), 2.0)), 0.50);
			if (d < (enemy.Size / 2) + shot[i].Size) {
				DealDamage(&enemy, shot[i].Damage);
				enemy.State = shot[i].Property;
				enemy.DamageDuration = shot[i].DamageDuration;
				enemy.DamageInterval = shot[i].DamageInterval;
				GenerateParticle(enemy.x, enemy.y, shot[i].Property);
				shot[i].ValidFlag = FALSE;
			}
		}
	}

	return 0;
}

int DealBadStatus(void) {
	BadStatus[enemy.State]();

	return 0;
}
int DealNormal(void) {
	enemy.Cr = GetColor(255, 255, 255);
	enemy.Speed = enemy.MaxSpeed;
	return 0;
}
int DealFrostbite(void) {
	enemy.Cr = GetColor(0, 0, 255);
	enemy.Speed = enemy.MaxSpeed * 0.5;
	if (enemy.DamageDuration % enemy.DamageInterval == 0) {
		DealDamage(&enemy, 8);
	}
	enemy.DamageDuration--;
	if (enemy.DamageDuration == 0) {
		enemy.State = NORMAL;
	}
	return 0;
}
int DealBurn(void) {
	enemy.Cr = GetColor(255, 0, 0);
	if (enemy.DamageDuration % enemy.DamageInterval == 0) {
		DealDamage(&enemy, 8);
	}
	enemy.DamageDuration--;
	if (enemy.DamageDuration == 0) {
		enemy.State = NORMAL;
	}
	return 0;
}

int DealDamage(_enemy* target, int Damage) {

	target->Life -= Damage;
	DrawFormatString(target->x, target->y - target->Size - 10, GetColor(255, 255, 255), "%d", Damage);

	return 0;
}

int DrawCharacter() {
	int i;

	if (player.ValidFlag == TRUE) {
		DrawBox(player.x - (player.Size / 2), player.y - (player.Size / 2), player.x + (player.Size / 2), player.y + (player.Size / 2), player.Cr, FALSE);
	}
	if (enemy.ValidFlag == TRUE) {
		DrawBox(enemy.x - (enemy.Size / 2), enemy.y - (enemy.Size / 2), enemy.x + (enemy.Size / 2), enemy.y + (enemy.Size / 2), enemy.Cr, FALSE);
	}
	for (i = 0; i < SHOT_NUM; i++) {
		if (shot[i].ValidFlag == TRUE) {
			DrawCircle(shot[i].x, shot[i].y, shot[i].Size, shot[i].Cr, TRUE);
		}
	}
	for (i = 0; i < PARTICLE_NUM; i++) {
		if (particle[i].ValidFlag == TRUE) {
			DrawBox(particle[i].x - 2, particle[i].y - 2, particle[i].x + 2, particle[i].y + 2, particle[i].Cr, TRUE);
		}
	}

	return 0;
}

int SetPlayerProperty() {

	char Buf[256];
	
	GetHitKeyStateAll(Buf);

	if (Buf[KEY_INPUT_1] == 1)
	{
		player.ShotProperty = NORMAL;
	}
	if (Buf[KEY_INPUT_2] == 1)
	{
		player.ShotProperty = FROSTBITE;
	}
	if (Buf[KEY_INPUT_3] == 1)
	{
		player.ShotProperty = BURN;
	}

	return 0;
}