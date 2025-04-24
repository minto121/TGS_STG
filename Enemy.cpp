#include "Enemy.h"
#include <DxLib.h>
#define PI 3.141592


Enemy::Enemy(float x, float y)
    : enemy_X(x), enemy_Y(y), baseX(x), baseY(y),
    e_Speed(0.2f), e_angle(PI / 2), frameCount(0)
{
}
Enemy::~Enemy() {

}

void Enemy::Update() {
    frameCount++;

    float angle = frameCount * (PI / 60.0f);

    enemy_X = baseX + sin(angle) * 50;
    enemy_Y = baseY + frameCount * 0.3f;
}

void Enemy::Draw()const {
    //デバッグ用
    int size = 32;
    DrawBox(
        static_cast<int>(enemy_X - size / 2),
        static_cast<int>(enemy_Y - size / 2),
        static_cast<int>(enemy_X + size / 2),
        static_cast<int>(enemy_Y + size / 2),
        GetColor(255, 0, 0),
        TRUE
    );
}