#include "DxLib.h"
#include "PadInput.h"
#include "Help.h"
#include "Title.h"
#include "End.h"
#include "GameMain.h"


Help::Help()
{
	//画像読込
	Help_img = LoadGraph("Resource/image/Help_img.png");

	//BGM・SE読込
	Title_BGM = LoadSoundMem("Resource/bgm/Title_BGM.wav");
	Select_SE = LoadSoundMem("Resource/bgm/Cursor_SE.wav");
}

Help::~Help()
{
	//BGM削除
	DeleteSoundMem(Title_BGM);
	StopSoundMem(Title_BGM);
}

AbstractScene* Help::Update()
{
	//BGM
	if (CheckSoundMem(Title_BGM) == 0)
	{
		PlaySoundMem(Title_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}

	if (PAD_INPUT::OnButton(XINPUT_BUTTON_A) == PRESS) {
		PlaySoundMem(Select_SE, DX_PLAYTYPE_BACK, TRUE);
		//BGM削除
		DeleteSoundMem(Title_BGM);
		StopSoundMem(Title_BGM);
		////各シーン
		return new GameMain();
		
	}
	if (PAD_INPUT::OnButton(XINPUT_BUTTON_B) == PRESS) {
		PlaySoundMem(Select_SE, DX_PLAYTYPE_BACK, TRUE);
		//BGM削除
		DeleteSoundMem(Title_BGM);
		StopSoundMem(Title_BGM);
		////各シーン
		return new Title();
		
	}
	return this;
}

void Help::Draw() const
{
	//タイトル画像
	DrawGraph(0, 0, Help_img, TRUE);
}