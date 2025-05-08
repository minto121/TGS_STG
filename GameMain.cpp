#include"DxLib.h"
#include "GameMain.h"
#include"Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include "Enemy.h"
#include "Title.h"


GameMain::GameMain()
{
	P_SHOT = new Player_Shot;
	D_PLAYER = new demo_Player;
	BULLET_DATE = new Bullet;
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // ← CSV読み込み

	enemy = new Enemy(320.0f, 100.0f);
	nowtime = 0;
	currentPattern = 0;
	bool isCKeyPressed = false;//確認用

}

GameMain::~GameMain()
{
	delete P_SHOT;
	delete D_PLAYER;
	delete BULLET_DATE;
	delete enemy;
}

AbstractScene* GameMain::Update()
{

	nowtime++;

	if (CheckHitKey(KEY_INPUT_C)) {
		if (!isCKeyPressed) {
			isCKeyPressed = true;

			currentPattern = (currentPattern + 1) % 3;

			if (currentPattern == 0) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_date.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // 通常弾は反射しない
			}
			else if (currentPattern == 1) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(true); // 通常弾は反射しない
			}
			else if (currentPattern == 2) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // 追尾弾は反射させない方が良い
			}
		}
		nowtime = 0;
	}
	else {
		isCKeyPressed = false;
	}

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

	if (D_PLAYER->GameOver()) {
		return new Title();
	}
	enemy->Update();
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
