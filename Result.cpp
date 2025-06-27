#include "Result.h"
#include "GameMain.h"
#include "Title.h"

Result::Result()
{
    BackImg = LoadGraph("Resource/image/mangetsu.png");
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
    DrawGraph(0, 0, BackImg, TRUE);

    SetFontSize(49);
    DrawString(540,80, "リザルト", GetColor(255, 255, 255));
    DrawString(390, 230, "あなたのスコアは", GetColor(255, 255, 255));

    int retryColor = selectedOption == ResultOption::Retry ? GetColor(255, 255, 0) : GetColor(150, 150, 150);
    int titleColor = selectedOption == ResultOption::Title ? GetColor(255, 255, 0) : GetColor(150, 150, 150);

    DrawString(280, 520, "リトライ", retryColor);
    DrawString(860, 520, "タイトル", titleColor);


    
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