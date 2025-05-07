#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"
#include "GameMain.h"
#include "End.h"
#include "Help.h"

Title::Title()
{

	TitleBGM = 0;
	TitleSE = 0;

	UI_Img[0] = LoadGraph("Resource/image/score_img.png");
	UI_Img[1] = LoadGraph("Resource/image/highscore_img.png");
	UI_Img[2] = LoadGraph("Resource/image/plyerlife.png");
	UI_Img[3] = LoadGraph("Resource/image/time_img.png");
	UI_Img[4] = LoadGraph("Resource/image/bomb_img.png");

	LifeImg = LoadGraph("Resource/image/life_img.png");

	FireImg = LoadGraph("Resource/image/FireBall_img.png");
	WaterImg = LoadGraph("Resource/image/WaterBall_img.png");
	LeafImg = LoadGraph("Resource/image/leaf_img.png");
}

Title::~Title()
{
}

AbstractScene* Title::Update()
{
	if (CheckHitKey(KEY_INPUT_Z))
	{
		return new GameMain();
	}
	
	if (CheckHitKey(KEY_INPUT_E))
	{
		return new End();
	}
	
	/*if (CheckHitKey(KEY_INPUT_H))
	{
		return new Help();
	}*/

	return this;
}

void Title::Draw() const
{
	//SetBackgroundColor(255, 255, 255);

	/*SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "タイトル画面");

	SetFontSize(50);
	DrawFormatString(550, 300, 0xffffff, "スタート");
	DrawFormatString(550, 400, 0xffffff, "ヘルプ");
	DrawFormatString(550, 500, 0xffffff, "エンド");*/

	DrawBox(800, 0, 1280, 700, 0xffffff, TRUE);

	DrawGraph(850, 50, UI_Img[0], TRUE);
	DrawGraph(850, 100, UI_Img[1], TRUE);
	DrawGraph(800, 150, UI_Img[2], TRUE);
	DrawGraph(850, 200, UI_Img[3], TRUE);
	DrawGraph(850, 250, UI_Img[4], TRUE);

	DrawGraph(1000, 170, LifeImg, TRUE);
	DrawGraph(1050, 170, LifeImg, TRUE);
	DrawGraph(1100, 170, LifeImg, TRUE);

	DrawGraph(100, 150, FireImg, TRUE);
	DrawGraph(200, 150, WaterImg, TRUE);
	DrawGraph(300, 150, LeafImg, TRUE);
}
