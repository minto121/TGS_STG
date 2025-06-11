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

	//�摜�ǂݍ���
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
	bool isCKeyPressed = false;//�m�F�p

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
	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y);
	BULLET_DATE->Update(nowtime);
	//D_PLAYER->fire(P_SHOT);  // �v���C���[���e�𔭎�

	if (enemy != nullptr) {
		enemy->Update();
		// �G�̌��݈ʒu��Bullet�ɋ�����
		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	}

	// �e�ƓG�̓����蔻��
	for (auto& b : P_SHOT->bullets) {  // P_SHOT�̒e���`�F�b�N
		if (b.active && enemy != nullptr) {
			if (enemy->CheckCollision(b.x, b.y, true)) {
				enemy->OnHit(); // HP�����炷
				b.active = false;  // �e������

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

						BULLET_DATE->ClearAllBullets();
						printfDx("WIN!! �ŏI�`�Ԍ��j\n");
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
