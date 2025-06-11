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

	//������
	select = 0;
	interval = 0;
	button_wait = 50;

	//�摜�Ǎ�
	TitleImg = LoadGraph("Resource/image/Title_img.jpg");
	cursor_img = LoadGraph("Resource/image/cursor_img.png");
}

Title::~Title()
{
}

AbstractScene* Title::Update()
{
	if (interval < 20)interval++;
	if (button_wait > 0)button_wait--;

	//�J�[�\���ړ�
	if (PAD_INPUT::OnButton(XINPUT_BUTTON_DPAD_UP) && interval >= 20) {
		select--;
		interval = 0;
	}
	else if (PAD_INPUT::OnButton(XINPUT_BUTTON_DPAD_DOWN) && interval >= 20) {
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
	DrawGraph(0, 0, TitleImg, TRUE);

	SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "�^�C�g��");

	SetFontSize(50);
	DrawFormatString(550, 300, 0xffffff, "�X�^�[�g");
	DrawFormatString(550, 400, 0xffffff, "�w���v");
	DrawFormatString(550, 500, 0xffffff, "�G���h");

	DrawGraph(240 , 190+ select * 100, cursor_img, TRUE);

}
