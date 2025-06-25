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

	//�摜�ǂݍ���
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
	bool isCKeyPressed = false;//�m�F�p

}

GameMain::~GameMain()
{
	delete P_SHOT;
	delete D_PLAYER;
	delete BULLET_DATE;
	delete enemy;

	//BGM�폜
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

	//spiral�̉�]���t�ɂ���
	if (nowtime % 300 == 0) {
		BULLET_DATE->ReverseSpiralDirection(); 
	}
	//�Q�[���N���A����
	if (isGameClear) {
		clearTimer++;

		if (!clearBulletStopped) {
			BULLET_DATE->StopAllBullets();  // �S�Ă̒e���~����֐��i���삷��j
			clearBulletStopped = true;
		}

		// 100�t���[���i��1.6�b�j�҂�����^�C�g���ɖ߂��A�܂��͉��o���s
		if (clearTimer >= 200) {
			 return new Title(); // �^�C�g���֖߂�
		}
		return this;
	}

	if (CheckHitKey(KEY_INPUT_C)) {
		if (!isCKeyPressed) {
			isCKeyPressed = true;

			currentPattern = (currentPattern + 1) % 3;

			if (currentPattern == 0) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_date.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // �ʏ�e�͔��˂��Ȃ�
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
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y,canFire);
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // �v���C���[���e�𔭎�

	//if (enemy != nullptr) {
	//	if (!enemy->IsDead()) {
	//		enemy->Update();  // ����łȂ���΍X�V
	//		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	//	}
	//}

	// �e�ƓG�̓����蔻�菈���i�V���v���Ɂj
	if (enemy != nullptr && enemy->GetState() == EnemyLifeState::ALIVE) {
		for (auto& b : P_SHOT->bullets) {
			if (b.active && enemy->CheckCollision(b.x, b.y, true)) {
				enemy->OnHit(); // HP�����炷
				b.active = false; // �e������
			}
		}
	}

	// �G�̏�ԕʏ����i���t���[���j
	if (enemy != nullptr) {
		switch (enemy->GetState()) {
		case EnemyLifeState::ALIVE:
			enemy->Update();
			BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
			if (enemy->GetHP() <= 0) {
				if (EnemyPhase < MaxEnemyPhase) {
					// ���̃t�F�[�Y�ֈڍs
					EnemyPhase++;
					delete enemy;
					enemy = new Enemy(320.0f, 100.0f);

					// �e���؂�ւ�
					if (EnemyPhase == 1) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(true);
						currentPattern = 1;
					}
					else if (EnemyPhase == 2) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(false);

						BULLET_DATE->EnableSpiral(0.0f, 50.0f, 1);  // �C�ӂ̊p�x�E���x�E�Ԋu
						BULLET_DATE->SetPhase(2);
						//BULLET_DATE->CreateSatelliteBullets(4, 60.0f, 0.05f);  // ��F4�A60px���a�A��������]
						currentPattern = 2;
					}
					return this;
				}else {
					// �ŏI�t�F�[�Y�I�� �� DYING�ֈڍs
					enemy->RequestDying();
				}
			}
			// DYING�\�񂪓����Ă����玟�̃t���[������ڍs
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
					//	return this;  // �� return ���Ȃ��Ŏ��t���[���Ń^�C�}�[��i�߂�
					//}
		
	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // ��2�b�i60FPS�z��j
			return new Title();
		}
	}
	return this;
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

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %d", nowtime);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "State: %d", static_cast<int>(enemy->GetState()));
	/*DrawFormatString(0, 100, GetColor(255, 255, 255), "gameclear: %d", isGameClear);
	DrawFormatString(0, 120, GetColor(255, 255, 255), "gameclear: %f", enemy->dyingTimer);*/

	// �� null �`�F�b�N��ǉ�
	if (enemy != nullptr) {
		enemy->Draw();
	}
	//printfDx("EnemyPhase: %d, Enemy HP: %d\n", EnemyPhase, enemy->GetHP());
	if (isGameClear && clearTimer >= 30) {  // �����o���Ă���\��
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
}
