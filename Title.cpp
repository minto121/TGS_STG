#include "DxLib.h"
#include "PadInput.h"
#include "Title.h"
#include "GameMain.h"
#include "End.h"

Title::Title()
{
	

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

	return this;
}

void Title::Draw() const
{
	//SetBackgroundColor(255, 255, 255);

	SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "�^�C�g�����");

	SetFontSize(50);
	DrawFormatString(550, 300, 0xffffff, "�X�^�[�g");
	DrawFormatString(550, 400, 0xffffff, "�w���v");
	DrawFormatString(550, 500, 0xffffff, "�G���h");
}
