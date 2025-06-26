#define _USE_MATH_DEFINES
#include "Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include"DxLib.h"
#include<math.h>

#define MAX_BOMB_STOCK 3


Player_Shot::Player_Shot() {
    //ShotTime = 0;
    lastShotTime = GetNowCount();
    LoadDivGraph("Resource/image/�ʏ�e��.png", 8, 8, 1, 64, 32, P_Shot_img);

    //BGM�ESE�Ǎ�
    Shot_SE = LoadSoundMem("Resource/bgm/hassya_SE.wav");

}

void Player_Shot::FireBullet(float playerX, float playerY)
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (!bullets[i].active) {
            bullets[i].x = playerX - 15;
            bullets[i].y = playerY;
            bullets[i].spd = 800.0f;
            bullets[i].angle = -90.0f; // �^�����
            bullets[i].active = true;
            break;
        }
    }

}

void Player_Shot::Update(float playerX, float playerY,bool canFire)
{ 
    // ���ʂ̐ݒ�
    ChangeVolumeSoundMem(255 * 60 / 100, Shot_SE);

    const int shotInterval = 150; // �~���b

    int now = GetNowCount();
   

    if (canFire && now - lastShotTime >= shotInterval) {
        PlaySoundMem(Shot_SE, DX_PLAYTYPE_BACK, TRUE);
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
            if (bullets[i].x < -15 || bullets[i].x > 850 || bullets[i].y < 0 || bullets[i].y > 720) {
                bullets[i].active = false;
            }
        }
    }

    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].active) {
            bombs[i].x = playerX; // �ǔ��i���t���[���v���C���[�̍��W�Ɂj
            bombs[i].y = playerY;
            bombs[i].radius += bombs[i].growthRate;
            bombs[i].timer--;

            if (bulletManager != nullptr) {
                bulletManager->ClearAllBulletsInRange(bombs[i].x, bombs[i].y, bombs[i].radius);
            }

            if (bombs[i].timer <= 0) {
                bombs[i].active = false;
            }
        }
    }
}


void Player_Shot::StopAllBullets() {
    for (int i = 0; i < PSHOT_MAX; i++) {
        bullets[i].active = false;
    }
}
void Player_Shot::init()
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        bullets[i].active = false;
    }
}
void Player_Shot::SetBombStock(int stock) {
    bombStock = stock;
}

void Player_Shot::UseBomb(float px, float py) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (!bombs[i].active && bombStock > 0) {
            bombs[i].active = true;
            bombs[i].x = px;
            bombs[i].y = py;
            bombs[i].radius = 0.0f;
            bombs[i].growthRate = 3.0f; // ���a10��������
            bombs[i].timer = 180;         // 1�b�����i60�t���[���j
            bombStock--;
            // PlaySoundMem(�{��SE, DX_PLAYTYPE_BACK, TRUE); // �C��
            break;
        }
    }
}

void Player_Shot::Draw()
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (bullets[i].active) {
            // ��F�����Ȕ����ۂ�`��i�֐��͎��� or ���C�u�����ˑ��j
            //DrawCircle((int)bullets[i].x, (int)bullets[i].y, 5, 0xFF0000);
            DrawGraph(bullets[i].x, bullets[i].y, P_Shot_img[4], TRUE);
        }
    }

    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].active) {
            int color = GetColor(255, 200, 100);
            DrawCircle((int)bombs[i].x, (int)bombs[i].y, (int)bombs[i].radius, color, FALSE);  // �g�̂�
            
            SetDrawBlendMode(DX_BLENDMODE_ADD,150);
            DrawCircle((int)bombs[i].x, (int)bombs[i].y, (int)(bombs[i].radius * 0.8), color, TRUE); // ���������ۂ�
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
    //DrawFormatString(0,70,0xffffff,"Player pos: %f, %f\n", bullets[i].x, bullets[i].y);

    //DrawFormatString(0, 0, 0xFFFFFF, "%d",);
}
