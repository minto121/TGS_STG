#include"DxLib.h"
#include "GameMain.h"
#include"Player_Shot.h"
#include"FpsControl.h"
#include"PadInput.h"
#include"Bullet.h"
#include "Enemy.h"
#include "Title.h"
#include "Result.h"


//#define PI 3.1415926f


GameMain::GameMain()
{
	D_PLAYER = new demo_Player;
	P_SHOT = new Player_Shot;
	BULLET_DATE = new Bullet;
	BULLET_DATE->SetPlayer(D_PLAYER);
	BULLET_DATE->LoadCSV("Resource/date/danmaku_date.csv",5,120); // ・ｽ・ｽ CSV・ｽﾇみ搾ｿｽ・ｽ・ｽ
	D_PLAYER->SetBulletManager(BULLET_DATE);
	P_SHOT->SetBulletManager(BULLET_DATE);



	//BGM・ｽESE・ｽﾇ搾ｿｽ
	GameMain_BGM = LoadSoundMem("Resource/bgm/GameMain_BGM.mp3");
	Hit_SE = LoadSoundMem("Resource/bgm/hit_SE.wav");

	//・ｽ鞫懶ｿｽﾇみ搾ｿｽ・ｽ・ｽ
	UI_Img[0] = LoadGraph("Resource/image/score_img.png");
	UI_Img[1] = LoadGraph("Resource/image/highscore_img.png");
	UI_Img[2] = LoadGraph("Resource/image/plyerlife.png");
	UI_Img[3] = LoadGraph("Resource/image/time_img.png");
	UI_Img[4] = LoadGraph("Resource/image/bomb_img.png");

	LifeImg = LoadGraph("Resource/image/life_img.png");
	bom_Img = LoadGraph("Resource/image/bom.png");
	BackGroundImg = LoadGraph("Resource/image/kuraimori.jpg");

	// 逕ｻ蜒丞・蜑ｲ隱ｭ縺ｿ霎ｼ縺ｿ
	LoadDivGraph("Resource/image/Score_image/suuji16x32_03.png", 13, 13, 1, 208/13, 32, TextImg);



	enemy = new Enemy(320.0f, 100.0f);
	int EnemyPhase = 0;
	nowtime = 0;
	currentPattern = 0;
	bool isCKeyPressed = false;//・ｽm・ｽF・ｽp

	score = 200;

	timelimit = 180*60;	//180*60・灘・

}

GameMain::~GameMain()
{
	//BGM・ｽ尞・
	DeleteSoundMem(GameMain_BGM);
	StopSoundMem(GameMain_BGM);

	//蜑企勁
	delete P_SHOT;
	delete D_PLAYER;
	delete BULLET_DATE;
	delete enemy;
}

AbstractScene* GameMain::Update()
{
	// 髻ｳ驥上・險ｭ螳・
	ChangeVolumeSoundMem(255 * 70 / 100, Hit_SE);

	//BGM
	if (CheckSoundMem(GameMain_BGM) == 0)
	{
		PlaySoundMem(GameMain_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}
	//繝懊Β
	if (CheckHitKey(KEY_INPUT_X)||PAD_INPUT::OnButton(XINPUT_BUTTON_B)) {
		P_SHOT->UseBomb(D_PLAYER->x, D_PLAYER->y);
	}

	nowtime++;

	//spiral縺ｮ蝗櫁ｻ｢繧帝・↓縺吶ｋ
	if (nowtime % 300 == 0) {
		BULLET_DATE->ReverseSpiralDirection(); 
	}
	//繧ｲ繝ｼ繝繧ｯ繝ｪ繧｢蜃ｦ逅・
	if (isGameClear) {
		clearTimer++;

		if (!clearBulletStopped) {
			BULLET_DATE->StopAllBullets();  // ・ｽS・ｽﾄの弾・ｽ・ｽ・ｽ~・ｽ・ｽ・ｽ・ｽﾖ撰ｿｽ・ｽi・ｽ・ｽ・ｽ・ｷ・ｽ・ｽj
			clearBulletStopped = true;
		}

		// 100・ｽt・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽi・ｽ・ｽ1.6・ｽb・ｽj・ｽﾒゑｿｽ・ｽ・ｽ・ｽ・ｽ^・ｽC・ｽg・ｽ・ｽ・ｽﾉ戻ゑｿｽ・ｽA・ｽﾜゑｿｽ・ｽﾍ会ｿｽ・ｽo・ｽ・ｽ・ｽs
		if (clearTimer >= 200) {
			//BGM蜑企勁
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
			//return new Title(); // 繧ｿ繧､繝医Ν縺ｸ謌ｻ繧・
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
				BULLET_DATE->SetReflectEnable(false); // 騾壼ｸｸ蠑ｾ縺ｯ蜿榊ｰ・＠縺ｪ縺・
				BULLET_DATE->bi.fall = true;
			}
			else if (currentPattern == 1) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(true); // ・ｽﾊ擾ｿｽe・ｽﾍ費ｿｽ・ｽﾋゑｿｽ・ｽﾈゑｿｽ
			}
			else if (currentPattern == 2) {
				BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
				BULLET_DATE->SetReflectEnable(false); // ・ｽﾇ費ｿｽ・ｽe・ｽﾍ費ｿｽ・ｽﾋゑｿｽ・ｽ・ｽ・ｽﾈゑｿｽ・ｽ・ｽ・ｽ・ｽ・ｽﾇゑｿｽ
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
	//D_PLAYER->fire(P_SHOT);  // ・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽe・ｽｭ趣ｿｽ

	//if (enemy != nullptr) {
	//	if (!enemy->IsDead()) {
	//		enemy->Update();  // 豁ｻ繧薙〒縺ｪ縺代ｌ縺ｰ譖ｴ譁ｰ
	//		BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
	//	}
	//}

	// 蠑ｾ縺ｨ謨ｵ縺ｮ蠖薙◆繧雁愛螳壼・逅・ｼ医す繝ｳ繝励Ν縺ｫ・・
	if (enemy != nullptr && enemy->GetState() == EnemyLifeState::ALIVE) {
		for (auto& b : P_SHOT->bullets) {
			if (b.active && enemy->CheckCollision(b.x, b.y, true)) {
				PlaySoundMem(Hit_SE, DX_PLAYTYPE_BACK, TRUE);
				enemy->OnHit(); // HP繧呈ｸ帙ｉ縺・
				b.active = false; // 蠑ｾ繧呈ｶ医☆
			}
		}
	}

	// 謨ｵ縺ｮ迥ｶ諷句挨蜃ｦ逅・ｼ域ｯ弱ヵ繝ｬ繝ｼ繝・・
	if (enemy != nullptr) {
		switch (enemy->GetState()) {
		case EnemyLifeState::ALIVE:
			enemy->Update();
			BULLET_DATE->SetEnemyPosition(enemy->GetX(), enemy->GetY());
			if (enemy->GetHP() <= 0) {
				if (EnemyPhase < MaxEnemyPhase) {
					// 谺｡縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｸ遘ｻ陦・
					EnemyPhase++;
					delete enemy;
					enemy = new Enemy(320.0f, 100.0f);

					// 蠑ｾ蟷募・繧頑崛縺・
					if (EnemyPhase == 1) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_hansya.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(true);
						currentPattern = 1;
					}
					else if (EnemyPhase == 2) {
						BULLET_DATE->ChangePattern("Resource/date/danmaku_tuibi.csv", 5, 120);
						BULLET_DATE->SetReflectEnable(false);

						BULLET_DATE->EnableSpiral(0.0f, 50.0f, 1);  // 莉ｻ諢上・隗貞ｺｦ繝ｻ騾溷ｺｦ繝ｻ髢馴囈
						BULLET_DATE->SetPhase(2);
						//BULLET_DATE->CreateSatelliteBullets(4, 60.0f, 0.05f);  // 萓具ｼ・蛟九・0px蜊雁ｾ・√ｆ縺｣縺上ｊ蝗櫁ｻ｢
						currentPattern = 2;
					}
					return this;
				}
				else {
					// 譛邨ゅヵ繧ｧ繝ｼ繧ｺ邨ゆｺ・竊・DYING縺ｸ遘ｻ陦・
					enemy->RequestDying();
				}
			}
			// DYING莠育ｴ・′蜈･縺｣縺ｦ縺・◆繧画ｬ｡縺ｮ繝輔Ξ繝ｼ繝縺九ｉ遘ｻ陦・
			if (enemy->IsRequestingDying()) {
				enemy->StartDying();

				score += 5000;
			}

			score = score + enemy->GetAddScore();

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
	int currentZanki = D_PLAYER->Zanki;

	// 笘・谿区ｩ溘′貂帙▲縺溽椪髢薙□縺第､懃衍
	if (previousZanki != -1 && currentZanki < previousZanki) {
		P_SHOT->SetBombStock(3);  // 竊・譛螟ｧ繝懊Β謨ｰ縺ｫ蠢懊§縺ｦ隱ｿ謨ｴ
	}

	previousZanki = currentZanki;

	if (enemy != nullptr && enemy->GetState() == EnemyLifeState::ALIVE) {
		if (!D_PLAYER->GameOver()) {
			timelimit--;
		}
		
	}
	else {
		if (timelimit > 60) {
			timelimit -= 60;
			score += 20;
		}
	}

	Score_math();

	if (D_PLAYER->GameOver()) {
		if (!isGameOver && D_PLAYER->Zanki == 0) {
			////BGM蜑企勁
			//DeleteSoundMem(GameMain_BGM);
			//StopSoundMem(GameMain_BGM);

			isGameOver = true;
			gameOverTimer = 0;
		}
		gameOverTimer++;

		if (gameOverTimer >= 120) { // ・ｽ・ｽ2・ｽb・ｽi60FPS・ｽz・ｽ・ｽj
			//BGM・ｽ尞・
			DeleteSoundMem(GameMain_BGM);
			StopSoundMem(GameMain_BGM);
			return new Result();
		}
	}
	return this;
	
}
void GameMain::Draw() const
{
	DrawGraph(0, -600, BackGroundImg, FALSE);

	// ・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽ`・ｽ・ｽi・ｽ・ｽ・ｽF・ｽ・ｽ・ｽ・ｽ・ｽl・ｽp・ｽj
	//DrawBox((int)(player.x - 10), (int)(player.y - 10), (int)(player.x + 10), (int)(player.y + 10), GetColor(255, 255, 255), TRUE);
	P_SHOT->Draw();
	D_PLAYER->Draw();
	BULLET_DATE->Draw();
	//FpsControl_Draw();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "Frame: %f", nowtime);

	// ・ｽ・ｽ null ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽﾇ会ｿｽ
	if (enemy != nullptr) {
		enemy->Draw();
	}

	/*if (result != nullptr) {
		result->Draw();
	}*/

	if (isGameClear && clearTimer >= 30) {  // 蟆代＠邨後▲縺ｦ縺九ｉ陦ｨ遉ｺ
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
		DrawBox(0, 0, 1200, 720, 0x00000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetFontSize(49);
		DrawFormatString(350, 300, GetColor(255, 255, 0), "GAME CLEAR!");

	}



	DrawBox(850, 0, 1280, 720, GetColor(125, 125, 125)/*0xffff00*/, TRUE);		//UI・ｽ\・ｽ・ｽ・ｽ・ｽ・ｽW

	DrawGraph(760, 30, UI_Img[0], TRUE);	//スコア
	DrawGraph(795, 130, UI_Img[1], TRUE);	//ハイスコア
	DrawGraph(830, 230, UI_Img[2], TRUE);	//プレイヤー残機
	DrawGraph(770, 330, UI_Img[3], TRUE);	//タイム
	DrawGraph(780, 430, UI_Img[4], TRUE);	//ボム数

	

	//繝励Ξ繧､繝､繝ｼ谿区ｩ溽判蜒・
	for (int i = 0; i < D_PLAYER->Zanki; i++) {
		int drawX = 1050 + i * 50;  // 50px 髢馴囈縺ｧ陦ｨ遉ｺ・郁ｪｿ謨ｴ蜿ｯ・・
		DrawGraph(drawX, 260, LifeImg, TRUE);
	}

	//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽc・ｽ@・ｽ鞫・
	DrawGraph(1050, 260, LifeImg, TRUE);
	DrawGraph(1100, 260, LifeImg, TRUE);
	DrawGraph(1150, 260, LifeImg, TRUE);


	DrawNumber(1100, 107, score);

	DrawNumber(1100, 407, timelimit/60/60);
	DrawGraph(1120, 405, TextImg[12], TRUE);
	DrawGraph(1120, 385, TextImg[12], TRUE);
	DrawNumber(1140, 407, timelimit/60%60);

	DrawFormatString(0, 40, 0xffffff, "timelimit:%d", timelimit/60);
	//繝励Ξ繧､繝､繝ｼ谿区ｩ溽判蜒・
	for (int i = 0; i < P_SHOT->bombStock; i++) {
		int drawX = 1100 + i * 50;  // 50px 髢馴囈縺ｧ陦ｨ遉ｺ・郁ｪｿ謨ｴ蜿ｯ・・
		DrawGraph(drawX - 75, 500, bom_Img, TRUE);
	}
}



void GameMain::Score_math()
{
	

	/*score = score + enemy->GetAddScore();*/

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
	//繝励Ξ繧､繝､繝ｼ谿区ｩ溽判蜒・
	//for (int i = 0; i < P_SHOT->bombStock; i++) {
	//	int drawX = 1100 + i * 50;  // 50px 髢馴囈縺ｧ陦ｨ遉ｺ・郁ｪｿ謨ｴ蜿ｯ・・
	//	DrawGraph(drawX-75, 500, bom_Img, TRUE);
	//}
}
