#define _USE_MATH_DEFINES
#include "Player_Shot.h"
#include"FpsControl.h"
#include"DxLib.h"
#include<math.h>

Player_Shot::Player_Shot() {
    //ShotTime = 0;
    lastShotTime = GetNowCount();
    Bullet_img = LoadGraph("Resource/image/defalte_Bullet.png");
}

void Player_Shot::FireBullet(float playerX, float playerY)
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (!bullets[i].active) {
            bullets[i].x = playerX;
            bullets[i].y = playerY;
            bullets[i].spd = 60.0f;
            bullets[i].angle = -90.0f; // �^�����
            bullets[i].active = true;
            break;
        }
    }
}

void Player_Shot::Update(float playerX, float playerY)
{
    const int shotInterval = 200; // �~���b

    int now = GetNowCount();
    if (now - lastShotTime >= shotInterval) {
        FireBullet(playerX, playerY);
        lastShotTime = now;
    }

    for (int i = 0; i < PSHOT_MAX; i++) {
        if (bullets[i].active) {
            // ���W�A���ɕϊ����Ĉړ�
            float dt = 1.0f / 60.0f;;//FpsControl_GetDeltaTime();  // 1�t���[���̎��ԁi�b�j

            double rad = bullets[i].angle * (M_PI / 180.0);
            bullets[i].x += cos(rad) * bullets[i].spd * dt;
            bullets[i].y += sin(rad) * bullets[i].spd * dt;
            bullets[i].cnt++;

            // ��ʊO�ɏo�����A�N�e�B�u���i����800x600�̉�ʂƂ��āj
            if (bullets[i].x < 0 || bullets[i].x > 700 || bullets[i].y < 0 || bullets[i].y > 720) {
                bullets[i].active = false;
            }
        }
    }
}

void Player_Shot::init()
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        bullets[i].active = false;
    }
}

void Player_Shot::Draw()
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (bullets[i].active) {
            // ��F�����Ȕ����ۂ�`��i�֐��͎��� or ���C�u�����ˑ��j
            //DrawCircle((int)bullets[i].x, (int)bullets[i].y, 5, 0xFF0000);
            DrawGraph(bullets[i].x, bullets[i].y, Bullet_img,TRUE);
        }
    }
    
    //DrawFormatString(0, 0, 0xFFFFFF, "%d",);
}
