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
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // �� CSV�ǂݍ���
	D_PLAYER->SetBulletManager(BULLET_DATE);



	//BGM�ESE�Ǎ�
	GameMain_BGM = LoadSoundMem("Resource/bgm/GameMain_BGM.mp3");
	Hit_SE = LoadSoundMem("Resource/bgm/hit_SE.wav");

	//�摜�ǂݍ���
	UI_Img[0] = LoadGraph("Resource/image/score_img.png");
	UI_Img[1] = LoadGraph("Resource/image/highscore_img.png");
	UI_Img[2] = LoadGraph("Resource/image/plyerlife.png");
	UI_Img[3] = LoadGraph("Resource/image/time_img.png");
	UI_Img[4] = LoadGraph("Resource/image/bomb_img.png");

	LifeImg = LoadGraph("Resource/image/life_img.png");
	BackGroundImg = LoadGraph("Resource/image/kuraimori.jpg");

	// 画像分割読み込み
	LoadDivGraph("Resource/image/Score_image/suuji16x32_03.png", 13, 13, 1, 208/13, 32, TextImg);



	enemy = new Enemy(320.0f, 100.0f);
	int EnemyPhase = 0;
	nowtime = 0;
	currentPattern = 0;
	bool isCKeyPressed = false;//�m�F�p

	score = 0;

}

GameMain::~GameMain()
{
	//BGM�폜
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
	// 音量の設定
	ChangeVolumeSoundMem(255 * 70 / 100, Hit_SE);

	//BGM
	if (CheckSoundMem(GameMain_BGM) == 0)
	{
		PlaySoundMem(GameMain_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}

	nowtime++;
	//�Q�[���N���A����
	if (isGameClear) {
		clearTimer++;

		if (!clearBulletStopped) {
			BULLET_DATE->StopAllBullets();  // �S�Ă̒e���~����֐��i���삷��j
			clearBulletStopped = true;
		}

		// 100�t���[���i��1.6�b�j�҂�����^�C�g���ɖ߂��A�܂��͉��o���s
		if (clearTimer >= 200) {
			//BGM削除
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
			return new Title(); // タイトルへ戻る
			return new Result();
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
				BULLET_DATE->SetReflectEnable(true); // �ʏ�e�͔��˂��Ȃ�
			}
			else if (currentPattern == 2) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // �ǔ��e�͔��˂����Ȃ������ǂ�
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
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y, canFire);
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // �v���C���[���e�𔭎�

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
				PlaySoundMem(Hit_SE, DX_PLAYTYPE_BACK, TRUE);
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
						currentPattern = 2;
					}
					return this;
				}
				else {
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

	Score_math();

	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			////BGM削除
			//DeleteSoundMem(GameMain_BGM);
			//StopSoundMem(GameMain_BGM);

			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // ��2�b�i60FPS�z��j
			//BGM�폜
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
			return new Result();
		}
	}
	
}
void GameMain::Draw() const
{
	DrawGraph(0, -600, BackGroundImg, FALSE);

	// �v���C���[�`��i���F�����l�p�j
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %f", nowtime);

	// �� null �`�F�b�N��ǉ�
	if (enemy != nullptr) {
		enemy->Draw();
	}

	//if (result != nullptr) {
	//	result->Draw();
	//}

	if (isGameClear && clearTimer >= 30) {  // 少し経ってから表示
		DrawFormatString(500, 300, GetColor(255, 255, 0), "GAME CLEAR!");
	}

	DrawBox(850, 0, 1280, 720, 0xffffff, TRUE);		//UI�\�����W

	DrawGraph(850, 30, UI_Img[0], TRUE);	//�X�R�A
	DrawGraph(850, 130, UI_Img[1], TRUE);	//�n�C�X�R�A
	DrawGraph(840, 230, UI_Img[2], TRUE);	//�v���C���[�c�@
	DrawGraph(850, 330, UI_Img[3], TRUE);	//�^�C��
	DrawGraph(850, 430, UI_Img[4], TRUE);	//�{����

	//�v���C���[�c�@�摜
	DrawGraph(1050, 260, LifeImg, TRUE);
	DrawGraph(1100, 260, LifeImg, TRUE);
	DrawGraph(1150, 260, LifeImg, TRUE);


	DrawNumber(1100, 107, score);

	/*for (int i = 0; i < 13; i++) {
		DrawGraph(850+i*15, 30, TextImg[i], TRUE);
	}*/
}

void GameMain::Score_math()
{
	score = 7819932;


}

void GameMain::DrawNumber(int x, int y, int value) const
{
	std::string str = std::to_string(value);

	for (int i = 0; i < str.size(); i++) {
		int index = GetImageIndex(str[i]);
		if (index != -1) {
			DrawGraph(x + i * 17, y, TextImg[index], TRUE);
		}
	}
}

int GameMain::GetImageIndex(char ch) const
{
	if ('0' <= ch && ch <= '9') return ch - '0';
	if (ch == '+') return 10;
	if (ch == '-') return 11;
	if (ch == '.') return 12;
	return -1;
}
