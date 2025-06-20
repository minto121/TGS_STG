#define _USE_MATH_DEFINES
#include "Player_Shot.h"
#include"FpsControl.h"
#include"Bullet.h"
#include"DxLib.h"
#include<math.h>

Player_Shot::Player_Shot() {
    //ShotTime = 0;
    lastShotTime = GetNowCount();
    LoadDivGraph("Resource/image/通常弾幕.png", 8, 8, 1, 64, 32, P_Shot_img);

    //BGM・SE読込
    Shot_SE = LoadSoundMem("Resource/bgm/hassya_SE.wav");
}

void Player_Shot::FireBullet(float playerX, float playerY)
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (!bullets[i].active) {
            bullets[i].x = playerX - 15;
            bullets[i].y = playerY;
            bullets[i].spd = 800.0f;
            bullets[i].angle = -90.0f; // 真上方向
            bullets[i].active = true;
            break;
        }
    }

}

void Player_Shot::Update(float playerX, float playerY,bool canFire)
{ 
    // 音量の設定
    ChangeVolumeSoundMem(255 * 60 / 100, Shot_SE);

    const int shotInterval = 150; // ミリ秒

    int now = GetNowCount();
   

    if (canFire && now - lastShotTime >= shotInterval) {
        PlaySoundMem(Shot_SE, DX_PLAYTYPE_BACK, TRUE);
        FireBullet(playerX, playerY);
        lastShotTime = now;
    }

    for (int i = 0; i < PSHOT_MAX; i++) {
        if (bullets[i].active) {
            // ラジアンに変換して移動
            float dt = 1.0f / 60.0f;;//FpsControl_GetDeltaTime();  // 1フレームの時間（秒）

            double rad = bullets[i].angle * (M_PI / 180.0);
            bullets[i].x += cos(rad) * bullets[i].spd * dt;
            bullets[i].y += sin(rad) * bullets[i].spd * dt;
            bullets[i].cnt++;

            // 画面外に出たら非アクティブ化（仮に800x600の画面として）
            if (bullets[i].x < 0 || bullets[i].x > 850 || bullets[i].y < 0 || bullets[i].y > 720) {
                bullets[i].active = false;
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

void Player_Shot::Draw()
{
    for (int i = 0; i < PSHOT_MAX; i++) {
        if (bullets[i].active) {
            // 例：小さな白い丸を描画（関数は自作 or ライブラリ依存）
            //DrawCircle((int)bullets[i].x, (int)bullets[i].y, 5, 0xFF0000);
            DrawGraph(bullets[i].x, bullets[i].y, P_Shot_img[4], TRUE);
        }
    }
    //DrawFormatString(0,70,0xffffff,"Player pos: %f, %f\n", bullets[i].x, bullets[i].y);

    //DrawFormatString(0, 0, 0xFFFFFF, "%d",);
}
