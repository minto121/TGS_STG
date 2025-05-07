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
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // © CSV“Ç‚Ýž‚Ý

	enemy = new Enemy(320.0f, 100.0f);
	nowtime = 0;
	currentPattern = 0;
	bool isCKeyPressed = false;//Šm”F—p

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
				BULLET_DATE->SetReflectEnable(false); // ’Êí’e‚Í”½ŽË‚µ‚È‚¢
			}
			else if (currentPattern == 1) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(true); // ’Êí’e‚Í”½ŽË‚µ‚È‚¢
			}
			else if (currentPattern == 2) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // ’Ç”ö’e‚Í”½ŽË‚³‚¹‚È‚¢•û‚ª—Ç‚¢
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
	//D_PLAYER->fire(P_SHOT);  // ƒvƒŒƒCƒ„[‚ª’e‚ð”­ŽË

	if (D_PLAYER->GameOver()) {
		return new Title();
	}
	enemy->Update();
	return this;
}

void GameMain::Draw() const
{
	// ƒvƒŒƒCƒ„[•`‰æi‰¼F”’‚¢ŽlŠpj
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %d", nowtime);

	//FpsControl_Draw();
	enemy->Draw();

}
