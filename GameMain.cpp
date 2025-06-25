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


	//BGM・SE読込
	GameMain_BGM = LoadSoundMem("Resource/bgm/GameMain_BGM.mp3");

	//画像読み込み
	UI_Img[0] = LoadGraph("Resource/image/score_img.png");
	UI_Img[1] = LoadGraph("Resource/image/highscore_img.png");
	UI_Img[2] = LoadGraph("Resource/image/plyerlife.png");
	UI_Img[3] = LoadGraph("Resource/image/time_img.png");
	UI_Img[4] = LoadGraph("Resource/image/bomb_img.png");

	LifeImg = LoadGraph("Resource/image/life_img.png");
	BackGroundImg = LoadGraph("Resource/image/kuraimori.jpg");

	enemy = new Enemy(320.0f, 100.0f);
	EnemyPhase = 0;
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

	//BGM削除
	DeleteSoundMem(GameMain_BGM);
	StopSoundMem(GameMain_BGM);
}

AbstractScene* GameMain::Update()
{
	//BGM
	if (CheckSoundMem(GameMain_BGM) == 0)
	{
		PlaySoundMem(GameMain_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}

	nowtime++;

	//spiralの回転を逆にする
	if (nowtime % 300 == 0) {
		BULLET_DATE->ReverseSpiralDirection(); 
	}
	//ゲームクリア処理
	if (isGameClear) {
		clearTimer++;

		if (!clearBulletStopped) {
			BULLET_DATE->StopAllBullets();  // 全ての弾を停止する関数（自作する）
			clearBulletStopped = true;
		}

		// 100フレーム（約1.6秒）待ったらタイトルに戻す、または演出続行
		if (clearTimer >= 200) {
			 return new Title(); // タイトルへ戻る
		}
		return this;
	}

	if (CheckHitKey(KEY_INPUT_C)) {
		if (!isCKeyPressed) {
			isCKeyPressed = true;

			currentPattern = (currentPattern + 1) % 3;

			if (currentPattern == 0) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_date.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // 通常弾は反射しない
				BULLET_DATE->bi.fall = true;
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
	bool canFire = !(D_PLAYER->GameOver() && D_PLAYER->Zanki == 0);
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y,canFire);
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // プレイヤーが弾を発射

	//if (enemy != nullptr) {
	//	if (!enemy->IsDead()) {
	//		enemy->Update();  // 死んでなければ更新
	//		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	//	}
	//}

	// 弾と敵の当たり判定処理（シンプルに）
	if (enemy != nullptr && enemy->GetState() == EnemyLifeState::ALIVE) {
		for (auto& b : P_SHOT->bullets) {
			if (b.active && enemy->CheckCollision(b.x, b.y, true)) {
				enemy->OnHit(); // HPを減らす
				b.active = false; // 弾を消す
			}
		}
	}

	// 敵の状態別処理（毎フレーム）
	if (enemy != nullptr) {
		switch (enemy->GetState()) {
		case EnemyLifeState::ALIVE:
			enemy->Update();
			BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
			if (enemy->GetHP() <= 0) {
				if (EnemyPhase < MaxEnemyPhase) {
					// 次のフェーズへ移行
					EnemyPhase++;
					delete enemy;
					enemy = new Enemy(320.0f, 100.0f);

					// 弾幕切り替え
					if (EnemyPhase == 1) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(true);
						currentPattern = 1;
					}
					else if (EnemyPhase == 2) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(false);

						BULLET_DATE->EnableSpiral(0.0f, 50.0f, 1);  // 任意の角度・速度・間隔
						BULLET_DATE->SetPhase(2);
						//BULLET_DATE->CreateSatelliteBullets(4, 60.0f, 0.05f);  // 例：4個、60px半径、ゆっくり回転
						currentPattern = 2;
					}
					return this;
				}else {
					// 最終フェーズ終了 → DYINGへ移行
					enemy->RequestDying();
				}
			}
			// DYING予約が入っていたら次のフレームから移行
			if (enemy->IsRequestingDying()) {
				enemy->StartDying();
			}
			break;

		case EnemyLifeState::DYING:
			enemy->UpdateDying();
			BULLET_DATE->ClearAllBullets();
			BULLET_DATE->StopAllBullets();
			BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
			if (enemy->IsDyingFinished()) {
				enemy->SetState(EnemyLifeState::DEAD);
			}
			break;

		case EnemyLifeState::DEAD:
			delete enemy;
			enemy = nullptr;
			isGameClear = true;
			clearTimer = 0;
			BULLET_DATE->ClearAllBullets();
			BULLET_DATE->StopAllBullets();
			//return new Title;
			isGameClear = true;
			break;
		}
	}
					//if (enemy != nullptr && enemy->GetHP() <= 0 && enemy->IsDead()) {
					//	delete enemy;
					//	enemy = nullptr;
					//	printfDx("WIN");

					//	isGameClear = true;
					//	clearTimer = 0;

					//	//P_SHOT->StopAllBullets();
					//	return this;  // ← return しないで次フレームでタイマーを進める
					//}
		
	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // 約2秒（60FPS想定）
			return new Title();
		}
	}
	return this;
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

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %d", nowtime);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "State: %d", static_cast<int>(enemy->GetState()));
	/*DrawFormatString(0, 100, GetColor(255, 255, 255), "gameclear: %d", isGameClear);
	DrawFormatString(0, 120, GetColor(255, 255, 255), "gameclear: %f", enemy->dyingTimer);*/

	// ↓ null チェックを追加
	if (enemy != nullptr) {
		enemy->Draw();
	}
	//printfDx("EnemyPhase: %d, Enemy HP: %d\n", EnemyPhase, enemy->GetHP());
	if (isGameClear && clearTimer >= 30) {  // 少し経ってから表示
		DrawFormatString(500, 300, GetColor(255, 255, 0), "GAME CLEAR!");
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
