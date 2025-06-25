#include "Result.h"
#include "GameMain.h"
#include "Title.h"

Result::Result()
{
}

Result::~Result()
{
}

AbstractScene* Result::Update()
{
    UpdateInput();

    int selected = GetSelected();
    if (selected == 1) {
        // リトライ：GameMain を新しく作って返す
        return new GameMain();
    }
    else if (selected == 2) {
        // タイトル：Title へ戻る
        return new Title();
    }
    return this;
}

void Result::Draw() const
{
    //DrawFormatString(350, 100, 0xffffff, "タイトル");
    SetFontSize(49);
    DrawString(540,150, "RESULT", GetColor(255, 255, 255));

    int retryColor = selectedOption == ResultOption::Retry ? GetColor(255, 255, 0) : GetColor(150, 150, 150);
    int titleColor = selectedOption == ResultOption::Title ? GetColor(255, 255, 0) : GetColor(150, 150, 150);

    DrawString(380, 420, "RETRY", retryColor);
    DrawString(740, 420, "TITLE", titleColor);

}

void Result::UpdateInput()
{
    if (inputCooldown > 0) inputCooldown--;

    // 左右キー入力で選択を切り替え（クールダウンあり）
    if (inputCooldown == 0 && !decisionMade)
    {
        if (CheckHitKey(KEY_INPUT_LEFT) || GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT)
        {
            selectedOption = ResultOption::Retry;
            inputCooldown = 10;
        }
        else if (CheckHitKey(KEY_INPUT_RIGHT) || GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT)
        {
            selectedOption = ResultOption::Title;
            inputCooldown = 10;
        }

        // 決定
        if (CheckHitKey(KEY_INPUT_Z) || GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1)
        {
            decisionMade = true;
        }
    }
}



int Result::GetSelected()
{
    if (!decisionMade) return 0;
    return selectedOption == ResultOption::Retry ? 1 : 2;
}