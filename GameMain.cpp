#include"DxLib.h"
#include "GameMain.h"
#include"Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include "Enemy.h"
#include "Title.h"
#include "Mob_Enemy.h"


GameMain::GameMain()
{
	P_SHOT = new Player_Shot;
	D_PLAYER = new demo_Player;
	BULLET_DATE = new Bullet;
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // �� CSV�ǂݍ���
	MOB_ENEMY = new Mob_Enemy;
	enemy = new Enemy(320.0f, 100.0f);
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
	delete MOB_ENEMY;
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

	P_SHOT->Update(D_PLAYER->x, D_PLAYER->y);
	D_PLAYER->move();
	D_PLAYER->Update(BULLET_DATE->GetBullets());
	BULLET_DATE->Update(nowtime);
	MOB_ENEMY->Update();
	//D_PLAYER->fire(P_SHOT);  // �v���C���[���e�𔭎�

	if (enemy != nullptr) {
		enemy->Update();
	}

	// �e�ƓG�̓����蔻��
	for (auto& b : P_SHOT->bullets) {  // P_SHOT�̒e��`�F�b�N
		if (b.active && enemy != nullptr) {
			// �v���C���[�̒e�ł��邱�Ƃ�m�F���ďՓ˔���
			if (enemy->CheckCollision(b.x, b.y, true)) {  // true�Ńv���C���[�̒e�Ɣ���
				enemy->OnHit();  // HP����炷
				b.active = false;  // �e�����

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
	return this;
}

void GameMain::Draw() const
{
	// �v���C���[�`��i���F�����l�p�j
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	MOB_ENEMY->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %d", nowtime);

	// �� null �`�F�b�N��ǉ�
	if (enemy != nullptr) {
		enemy->Draw();
	}
}
