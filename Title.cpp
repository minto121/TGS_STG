#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"
#include "GameMain.h"
#include "End.h"
#include "Help.h"

Title::Title()
{
	//初期化
	select = 0;
	interval = 0;
	button_wait = 50;

	//画像読込
	Title_Img = LoadGraph("Resource/image/Title_img.jpg");
	cursor_img = LoadGraph("Resource/image/cursor_img.png");

	//BGM・SE読込
	Title_BGM = LoadSoundMem("Resource/bgm/Title_BGM.wav");
	Select_SE = LoadSoundMem("Resource/bgm/Cursor_SE.wav");
	CursorMove_SE = LoadSoundMem("Resource/bgm/CursorMove_SE.wav");
}

Title::~Title()
{
	//BGM削除
	DeleteSoundMem(Title_BGM);
	StopSoundMem(Title_BGM);
}

AbstractScene* Title::Update()
{
	//BGM
	if (CheckSoundMem(Title_BGM) == 0)
	{
		PlaySoundMem(Title_BGM, DX_PLAYTYPE_LOOP, TRUE);
	}

	if (interval < 10)interval++;
	if (button_wait > 0)button_wait--;

	//カーソル移動
	if (PAD_INPUT::OnButton(XINPUT_BUTTON_DPAD_UP) && interval >= 10) {
		PlaySoundMem(Select_SE, DX_PLAYTYPE_BACK, TRUE);
		select--;
		interval = 0;
	}
	else if (PAD_INPUT::OnButton(XINPUT_BUTTON_DPAD_DOWN) && interval >= 10) {
		PlaySoundMem(Select_SE, DX_PLAYTYPE_BACK, TRUE);
		select++;
		interval = 0;
		
	}

	if (select == 0) Menu_Number1 = TITLE_MENU::START;
	if (select == 1) Menu_Number1 = TITLE_MENU::HELP;
	if (select == 2) Menu_Number1 = TITLE_MENU::END;

	if (select < 0)select = 2;
	if (select > 2)select = 0;

	//遷移先
	if (button_wait == 0) {
		if (PAD_INPUT::OnButton(XINPUT_BUTTON_A) == PRESS) {
			PlaySoundMem(CursorMove_SE, DX_PLAYTYPE_BACK, TRUE);
			////各シーン
			if (TITLE_MENU::START == Menu_Number1) return new GameMain();
			if (TITLE_MENU::HELP == Menu_Number1) return new Help();
			if (TITLE_MENU::END == Menu_Number1) return new End();

			interval = 0;
		}
	}
	return this;
}

void Title::Draw() const
{
	//タイトル画像
	DrawGraph(0, 0, Title_Img, TRUE);

	SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "タイトル");

	SetFontSize(50);
	DrawFormatString(550, 300, 0xffffff, "スタート");
	DrawFormatString(550, 400, 0xffffff, "ヘルプ");
	DrawFormatString(550, 500, 0xffffff, "エンド");

	DrawGraph(240 , 190+ select * 100, cursor_img, TRUE);

}
