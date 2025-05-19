#include "DxLib.h"
#include "PadInput.h"
#include "Help.h"
#include "Title.h"
#include "End.h"


Help::Help()
{

}

Help::~Help()
{

}

AbstractScene* Help::Update()
{
	return this;
}

void Help::Draw() const
{
	SetFontSize(100);
	DrawFormatString(350, 100, 0xffffff, "ƒwƒ‹ƒv‰æ–Ê");
}