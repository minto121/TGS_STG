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
				BULLET_DATE->bi.fall == true;
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

	if (enemy != nullptr) {
		enemy->Update();
		// �G��HP��������؂�����e�p�^�[����ς���
		if (enemy->GetHP() <= 5 && currentPattern != 99) {
			BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120); // �D���Ȓe�p�^�[���ɕύX
			BULLET_DATE->SetReflectEnable(false); // �K�v�ɉ����Ĕ��˂�ݒ�
			currentPattern = 99; // �t���O����F1�񂵂��؂�ւ��Ȃ��悤��
		}
		
		// �G�̌��݈ʒu��Bullet�ɋ�����

		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	}

	// �e�ƓG�̓����蔻��
	for (auto& b : P_SHOT->bullets) {  // P_SHOT�̒e��`�F�b�N
		if (b.active && enemy != nullptr) {
			if (enemy->CheckCollision(b.x, b.y, true)) {
				enemy->OnHit(); // HP����炷
				b.active = false;  // �e�����
				if (enemy->IsDead()) { // �� HP��0�ȉ��Ȃ�|��
					EnemyPhase++;

					if (EnemyPhase < 3) {
						delete enemy;
						enemy = new Enemy(320.0f, 100.0f); // �G����

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
						printfDx("WIN!! �ŏI�`�Ԍ��j\n");
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
					return this;  // �� return ���Ȃ��Ŏ��t���[���Ń^�C�}�[��i�߂�
				}
			}
		}
	}

	Score_math();

	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // ��2�b�i60FPS�z��j
			//BGM�폜
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
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

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %f", nowtime);

	// �� null �`�F�b�N��ǉ�
	if (enemy != nullptr) {
		enemy->Draw();
	}


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
