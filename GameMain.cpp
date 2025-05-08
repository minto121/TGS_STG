#include"DxLib.h"
#include "GameMain.h"
#include"Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include "Enemy.h"


GameMain::GameMain()
{
	P_SHOT = new Player_Shot;
	D_PLAYER = new demo_Player;
	BULLET_DATE = new Bullet;
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv"); // ← CSV読み込み

	enemy = new Enemy(320.0f, 100.0f);
	nowtime = 0;
}

GameMain::~GameMain()
{
	delete P_SHOT;
	delete D_PLAYER;
	delete BULLET_DATE;
}

AbstractScene* GameMain::Update()
{

	nowtime++;

	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y);
	D_PLAYER->move();
	D_PLAYER->Update(BULLET_DATE->GetBullets());
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // プレイヤーが弾を発射

	if (enemy != nullptr) {
		enemy->Update();
	}

	// 弾と敵の当たり判定
	for (auto& b : P_SHOT->bullets) {  // P_SHOTの弾をチェック
		if (b.active && enemy != nullptr) {
			// プレイヤーの弾であることを確認して衝突判定
			if (enemy->CheckCollision(b.x, b.y, true)) {  // trueでプレイヤーの弾と判定
				enemy->OnHit();  // HPを減らす
				b.active = false;  // 弾を消す

				if (enemy->IsDead()) {
					delete enemy;
					enemy = nullptr;
					printfDx("WIN");

					break;
				}
			}
		}
	}

	return this;
}

void GameMain::Draw() const
{
	// プレイヤー描画（仮：白い四角）
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %d", nowtime);

	// ↓ null チェックを追加
	if (enemy != nullptr) {
		enemy->Draw();
	}
}
