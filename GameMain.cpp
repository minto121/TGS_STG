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
	int EnemyPhase = 0;
	nowtime = 0;
	currentPattern = 0;
	bool isCKeyPressed = false;//確認用

}

GameMain::~GameMain()
{
	//BGM削除
	DeleteSoundMem(GameMain_BGM);
	StopSoundMem(GameMain_BGM);

	//削除
	delete P_SHOT;
	delete D_PLAYER;
	delete BULLET_DATE;
	delete enemy;
}

AbstractScene* GameMain::Update()
{
	//BGM
	if (CheckSoundMem(GameMain_BGM) == 0)
	{
		PlaySoundMem(GameMain_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}

	nowtime++;
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
	bool canFire = !(D_PLAYER->GameOver() && D_PLAYER->Zanki == 0);
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y,canFire);
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
						isGameClear = true;
						BULLET_DATE->ClearAllBullets();
						printfDx("WIN!! 最終形態撃破\n");
					}
				}
				if (enemy != nullptr && enemy->GetHP() <= 0 && enemy->IsDead()) {
					delete enemy;
					enemy = nullptr;
					printfDx("WIN");

					isGameClear = true;
					clearTimer = 0;
					BULLET_DATE->StopAllBullets();
					//P_SHOT->StopAllBullets();
					return this;  // ← return しないで次フレームでタイマーを進める
				}
			}
		}
	}

	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // 約2秒（60FPS想定）
			//BGM削除
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
			return new Title();
		}
	}
	if (clearTimer >= 200) {
		if (result == nullptr) {
			result = new Result();
			// ここでスコアを渡したい場合は：result->SetScore(score);
		}

		result->UpdateInput();
		int selected = result->GetSelected();
		if (selected == 1) {
			// リトライ：GameMain を新しく作って返す
			delete result;
			result = nullptr;
			return new GameMain();
		}
		else if (selected == 2) {
			// タイトル：Title へ戻る
			delete result;
			result = nullptr;
			return new Title();
		}
		return this;
	}
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

	if (result != nullptr) {
		result->Draw();
	}

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
