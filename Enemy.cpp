#include "Enemy.h"
#include <DxLib.h>
#include <cstdlib>

Enemy::Enemy(float x, float y)
    : enemy_X(x), enemy_Y(y), baseX(x), baseY(y),
    e_angle(PI / 2), dashSpeed(4.0f), zigzagOffset(0.0f),
    frameCount(0), stateTimer(0), state(EnemyState::Spiral),
    hp(10), radius(16.0f)
{
}

Enemy::~Enemy() {}

void Enemy::Update()
{
    frameCount++;
    stateTimer++;

    if (state == EnemyState::Spiral)
    {
        SpiralBehavior();
    }
   /* switch (state)
    {
    case EnemyState::Enter:
        EnteringBehavior(); break;
    case EnemyState::Wait:
        WaitingBehavior(); break;
    case EnemyState::Dash:
        DashingBehavior(); break;
    case EnemyState::Zigzag:
        ZigZagBehavior(); break;
    case EnemyState::Teleport:
        TeleportingBehavior(); break;
    case EnemyState::Spiral:
        Spiral(); break;
    }*/
}

//void Enemy::EnteringBehavior()
//{
//    enemy_Y += 1.5f;
//    if (enemy_Y >= 100.0f)
//    {
//        enemy_Y = 100.0f;
//        state = EnemyState::Wait;
//        stateTimer = 0;
//    }
//}

//void Enemy::WaitingBehavior()
//{
//    if (stateTimer >= 60) // 1•b‘Ò‹@
//    {
//        ChangeToRandomState();
//    }
//}

//void Enemy::DashingBehavior()
//{
//    enemy_X += cos(e_angle) * dashSpeed;
//    enemy_Y += sin(e_angle) * dashSpeed;
//
//    if (enemy_X < 0 || enemy_X > 640 || enemy_Y < 0 || enemy_Y > 480 || stateTimer > 60)
//    {
//        state = EnemyState::Wait;
//        stateTimer = 0;
//    }
//}

//void Enemy::ZigZagBehavior()
//{
//    float baseSpeed = 2.0f;
//    zigzagOffset += 0.2f;
//    enemy_X += cos(e_angle + sin(zigzagOffset) * 0.5f) * baseSpeed;
//    enemy_Y += sin(e_angle + sin(zigzagOffset) * 0.5f) * baseSpeed;
//
//    if (stateTimer > 90)
//    {
//        state = EnemyState::Wait;
//        stateTimer = 0;
//        zigzagOffset = 0.0f;
//    }
//}

//void Enemy::TeleportingBehavior()
//{
//    if (stateTimer == 1)
//    {
//        enemy_X = static_cast<float>(rand() % 600 + 20);
//        enemy_Y = static_cast<float>(rand() % 200 + 20);
//    }
//
//    if (stateTimer >= 30)
//    {
//        state = EnemyState::Wait;
//        stateTimer = 0;
//    }
//}

void Enemy::SpiralBehavior()
{
    float angle = frameCount * 0.05f; // ‰ñ“]‘¬“x
    float radius = 50.0f + frameCount * 0.5f; // ‰Q‚Ì”¼Œa‚ª™X‚É‘‰Á

    enemy_X = baseX + cosf(angle) * radius;
    enemy_Y = baseY + sinf(angle) * radius;
}
//void Enemy::ChangeToRandomState()
//{
//    int next = rand() % 3;
//    switch (next)
//    {
//    case 0: state = EnemyState::Dash; break;
//    case 1: state = EnemyState::Zigzag; break;
//    case 2: state = EnemyState::Teleport; break;
//    }
//    stateTimer = 0;
//    e_angle = (float)(rand() % 360) * (PI / 180.0f);
//}

bool Enemy::CheckCollision(float bulletX, float bulletY, bool isPlayerBullet) const
{
    if (!isPlayerBullet) {
        return false;  // ƒvƒŒƒCƒ„[‚Ì’e‚Å‚È‚¢ê‡‚ÍÕ“Ë‚µ‚È‚¢
    }

    float dx = bulletX - enemy_X;
    float dy = bulletY - enemy_Y;
    float distanceSq = dx * dx + dy * dy;
    return distanceSq <= (radius + 8.0f) * (radius + 8.0f);  // ’e‚Ì”¼Œa‚à‰ÁZi8.0f‚ÍƒvƒŒƒCƒ„[’e‚Ì”¼Œaj
}

void Enemy::OnHit()
{
    hp--;
    if (hp <= 0)
    {
        // €–Sˆ—i”š”­‰‰o‚Æ‚©j
    }
}

bool Enemy::IsDead() const
{
    return hp <= 0;
}

int Enemy::GetHP()const {
    return hp;
}

void Enemy::Draw() const
{
    int size = 32;
    DrawBox(
        static_cast<int>(enemy_X - size / 2),
        static_cast<int>(enemy_Y - size / 2),
        static_cast<int>(enemy_X + size / 2),
        static_cast<int>(enemy_Y + size / 2),
        GetColor(255, 0, 0),
        TRUE
    );
    // HP•\¦i“G‚Ìã‚É•\¦j
    DrawFormatString(
        static_cast<int>(enemy_X - 10),
        static_cast<int>(enemy_Y - size / 2 - 16),
        GetColor(255, 255, 255),
        "HP: %d", hp
    );
}