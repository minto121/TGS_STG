#pragma once
#include"DxLib.h"

#define ENEMY_MAX 30

//敵に関する構造体
typedef struct {
	//フラグ、カウンタ、移動パターン、向き、敵の種類、HP最大値、落とすアイテム、画像
	int flag, cnt, pattern, muki, knd, hp, hp_max, item_n[6], img;
	//座標、速度x成分、速度y成分、スピード、角度
	double x, y, vx, vy, sp, ang;
	//弾幕開始時間、弾幕の種類、弾の種類、色、状態、待機時間、停滞時間
	int bltime, blknd, blknd2, col, state, wtime, wait;
}enemy_t;


class Mob_Enemy
{
private:

	enemy_t enemy[ENEMY_MAX];

	int stage_count;

public:

	//コンストラクタ
	Mob_Enemy();

	//デストラクタ
	~Mob_Enemy();

	// 更新処理
	void  Update();

	// 描画処理
	 void Draw() const;


	 //敵の移動パターン0での移動制御
	 void enemy_pattern0(int i);

	 //敵データの登録
	 void enemy_enter();

	 //敵の行動制御
	 void enemy_act();
};

