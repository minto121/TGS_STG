#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"
#include "GameMain.h"
#include "End.h"
#include "Help.h"

Title::Title()
{
	//������
	select = 0;
	interval = 0;
	button_wait = 50;

	//�摜�Ǎ�
	Title_Img = LoadGraph("Resource/image/Title_img.jpg");
	cursor_img = LoadGraph("Resource/image/cursor_img.png");

	//BGM�ESE�Ǎ�
	Title_BGM = LoadSoundMem("Resource/bgm/Title_BGM.wav");
	Select_SE = LoadSoundMem("Resource/bgm/Cursor_SE.wav");
	CursorMove_SE = LoadSoundMem("Resource/bgm/CursorMove_SE.wav");
}

Title::~Title()
{
	//BGM�폜
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

	//�J�[�\���ړ�
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

	//�J�ڐ�
	if (button_wait == 0) {
		if (PAD_INPUT::OnButton(XINPUT_BUTTON_A) == PRESS) {
			PlaySoundMem(CursorMove_SE, DX_PLAYTYPE_BACK, TRUE);
			////�e�V�[��
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
	//�^�C�g���摜
	DrawGraph(0, 0, Title_Img, TRUE);

	SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "�^�C�g��");

	SetFontSize(50);
	DrawFormatString(550, 300, 0xffffff, "�X�^�[�g");
	DrawFormatString(550, 400, 0xffffff, "�w���v");
	DrawFormatString(550, 500, 0xffffff, "�G���h");

	DrawGraph(240 , 190+ select * 100, cursor_img, TRUE);

}
