#include "demo_Player.h"
#include"Player_Shot.h"
#include"DxLib.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

demo_Player::demo_Player()
{
    x = SCREEN_WIDTH / 2.0f;
    y = SCREEN_HEIGHT / 2.0f;
}

void demo_Player::move()
{
    bool slow = PAD_INPUT::OnHold(XINPUT_BUTTON_A)|| CheckHitKey(KEY_INPUT_SPACE);

    // ��  0.2f
    if (CheckHitKey(KEY_INPUT_W) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_UP) == 1) { 
        if (slow == true) {
            y -= 2.0f;
        }
        else {
            y -= 4.0f;
        }
    }

    // ��
    if (CheckHitKey(KEY_INPUT_S) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_DOWN) == 1) {
        if (slow == true) {
            y += 2.0f;
        }
        else {
            y += 4.0f;
        }
    }  

    // ��
    if (CheckHitKey(KEY_INPUT_A) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_LEFT) == 1) {
        if (slow == true) {
            x -= 2.0f;
        }
        else {
            x -= 4.0f;
        }
    }

    // �E
    if (CheckHitKey(KEY_INPUT_D) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_RIGHT) == 1) {
        if (slow == true) {
            x += 2.0f;
        }
        else {
            x += 4.0f;
        }
    }  
}

//void demo_Player::fire(Player_Shot*P_SHOT)
//{
//    int now = GetNowCount();
//    if (now - lastShotTime >= shotInterval) {
//        P_SHOT->FireBullet(x, y);
//        lastShotTime = now;
//    }
//}


void demo_Player::Draw()
{
    // �v���C���[�𔒂��l�p�ŕ`��
    DrawBox((int)(x - 10), (int)(y - 10), (int)(x + 10), (int)(y + 10), GetColor(255, 255, 255), TRUE);
}

