#include "Result.h"


void Result::UpdateInput()
{
    if (inputCooldown > 0) inputCooldown--;

    // ���E�L�[���͂őI����؂�ւ��i�N�[���_�E������j
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

        // ����
        if (CheckHitKey(KEY_INPUT_Z) || GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1)
        {
            decisionMade = true;
        }
    }
}

void Result::Draw()
{
    DrawString(240, 150, "RESULT", GetColor(255, 255, 255));

    int retryColor = selectedOption == ResultOption::Retry ? GetColor(255, 255, 0) : GetColor(150, 150, 150);
    int titleColor = selectedOption == ResultOption::Title ? GetColor(255, 255, 0) : GetColor(150, 150, 150);

    DrawString(180, 220, "RETRY", retryColor);
    DrawString(340, 220, "TITLE", titleColor);
}

int Result::GetSelected()
{
    if (!decisionMade) return 0;
    return selectedOption == ResultOption::Retry ? 1 : 2;
}