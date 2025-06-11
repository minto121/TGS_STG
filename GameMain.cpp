#include"DxLib.h"
#include "GameMain.h"
#include"Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include "Enemy.h"
#include "Title.h"

//#define PI 3.1415926f


GameMain::GameMain()
{
	D_PLAYER = new demo_Player;
	P_SHOT = new Player_Shot;
	BULLET_DATE = new Bullet;
	BULLET_DATE->SetPlayer(D_PLAYER);
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // ← CSV読み込み
	D_PLAYER->SetBulletManager(BULLET_DATE);

	//画像読み込み
	UI_Img[0] = LoadGraph("Resource/image/score_img.png");
	UI_Img[1] = LoadGraph("Resource/image/highscore_img.png");
	UI_Img[2] = LoadGraph("Resource/image/plyerlife.png");
	UI_Img[3] = LoadGraph("Resource/image/time_img.png");
	UI_Img[4] = LoadGraph("Resource/image/bomb_img.png");

	LifeImg = LoadGraph("Resource/image/life_img.png");
	BackGroundImg = LoadGraph("Resource/image/kuraimori.jpg");

	enemy = new Enemy(320.0f, 100.0f);
	int EnemyPhase = 0;
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
				BULLET_DATE->bi.fall == true;
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
	D_PLAYER->move();
	D_PLAYER->Update(BULLET_DATE->GetBullets());
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y);
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // プレイヤーが弾を発射

	if (enemy != nullptr) {
		enemy->Update();
		// 敵の現在位置をBulletに教える
		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	}

	// 弾と敵の当たり判定
	for (auto& b : P_SHOT->bullets) {  // P_SHOTの弾をチェック
		if (b.active && enemy != nullptr) {
			if (enemy->CheckCollision(b.x, b.y, true)) {
				enemy->OnHit(); // HPを減らす
				b.active = false;  // 弾を消す

				if (enemy->IsDead()) { // ★ HPが0以下なら倒す
					EnemyPhase++;

					if (EnemyPhase < 3) {
						delete enemy;
						enemy = new Enemy(320.0f, 100.0f); // 敵復活

						if (EnemyPhase == 1) {
							BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
							BULLET_DATE->SetReflectEnable(true);
							currentPattern = 1;
						}
						else if (EnemyPhase == 2) {
							BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
							BULLET_DATE->SetReflectEnable(false);
							currentPattern = 2;
						}
					}
					else {
						delete enemy;
						enemy = nullptr;

						BULLET_DATE->ClearAllBullets();
						printfDx("WIN!! 最終形態撃破\n");
					}
				}

				break;
			}
		}
	}

	if (D_PLAYER->GameOver()) {
		return new Title();
	}
	return 0;
}

void GameMain::Draw() const
{
	DrawGraph(0, -600, BackGroundImg, FALSE);

	// プレイヤー描画（仮：白い四角）
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %f", nowtime);

	// ↓ null チェックを追加
	if (enemy != nullptr) {
		enemy->Draw();
	}

	DrawBox(850, 0, 1280, 720, 0xffffff, TRUE);		//UI表示座標

	DrawGraph(850, 30, UI_Img[0], TRUE);	//スコア
	DrawGraph(850, 130, UI_Img[1], TRUE);	//ハイスコア
	DrawGraph(840, 230, UI_Img[2], TRUE);	//プレイヤー残機
	DrawGraph(850, 330, UI_Img[3], TRUE);	//タイム
	DrawGraph(850, 430, UI_Img[4], TRUE);	//ボム数

	//プレイヤー残機画像
	DrawGraph(1050, 260, LifeImg, TRUE);
	DrawGraph(1100, 260, LifeImg, TRUE);
	DrawGraph(1150, 260, LifeImg, TRUE);
}
