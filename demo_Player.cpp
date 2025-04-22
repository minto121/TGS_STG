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

    // 上  0.2f
    if (CheckHitKey(KEY_INPUT_W) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_UP) == 1) { 
        if (slow == true) {
            y -= 0.1f;
        }
        else {
            y -= 0.3f;
        }
    }

    // 下
    if (CheckHitKey(KEY_INPUT_S) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_DOWN) == 1) {
        if (slow == true) {
            y += 0.1f;
        }
        else {
            y += 0.3f;
        }
    }  

    // 左
    if (CheckHitKey(KEY_INPUT_A) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_LEFT) == 1) {
        if (slow == true) {
            x -= 0.1f;
        }
        else {
            x -= 0.3f;
        }
    }

    // 右
    if (CheckHitKey(KEY_INPUT_D) || PAD_INPUT::OnHold(XINPUT_BUTTON_DPAD_RIGHT) == 1) {
        if (slow == true) {
            x += 0.1f;
        }
        else {
            x += 0.3f;
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
    // プレイヤーを白い四角で描画
    DrawBox((int)(x - 10), (int)(y - 10), (int)(x + 10), (int)(y + 10), GetColor(255, 255, 255), TRUE);
}

