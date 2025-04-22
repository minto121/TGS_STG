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
    if (CheckHitKey(KEY_INPUT_W)) y -= 1.0f;  // ��
    if (CheckHitKey(KEY_INPUT_S)) y += 1.0f;  // ��
    if (CheckHitKey(KEY_INPUT_A)) x -= 1.0f;  // ��
    if (CheckHitKey(KEY_INPUT_D)) x += 1.0f;  // �E
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

