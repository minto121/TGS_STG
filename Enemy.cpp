#include "Enemy.h"
#include <DxLib.h>
#include <cstdlib>

Enemy::Enemy(float x, float y)
    : enemy_X(x), enemy_Y(y), baseX(x), baseY(y),
    e_angle(PI / 2), dashSpeed(4.0f), zigzagOffset(0.0f),
    frameCount(0), stateTimer(0), state(EnemyState::Enter),
    hp(10), radius(16.0f)
{
    enemy_img = LoadGraph("Resource/image/Enemy_image/pipo-boss003.png");
}

Enemy::~Enemy() {}

void Enemy::Update()
{
    frameCount++;
    stateTimer++;

    switch (state)
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
    }

}

void Enemy::EnteringBehavior()
{
    if (enemy_Y > 240.0f) enemy_Y = 240.0f;
    if (enemy_Y >= 100.0f)
    {
        enemy_Y = 100.0f;
        state = EnemyState::Wait;
        stateTimer = 0;
    }
}

void Enemy::WaitingBehavior()
{
    if (stateTimer >= 240) // 1秒待機
    {
        ChangeToRandomState();
    }
}

void Enemy::DashingBehavior()
{
    enemy_X += cos(e_angle) * dashSpeed;
    enemy_Y += sin(e_angle) * dashSpeed;

    if (enemy_Y > 240.0f) enemy_Y = 240.0f;
    if (enemy_X < 0 || enemy_X > 640 || enemy_Y < 0 || enemy_Y > 480 || stateTimer > 60)
    {
        state = EnemyState::Wait;
        stateTimer = 0;
    }
}

void Enemy::ZigZagBehavior()
{
    float baseSpeed = 2.0f;
    zigzagOffset += 0.2f;
    enemy_X += cos(e_angle + sin(zigzagOffset) * 0.5f) * baseSpeed;
    enemy_Y += sin(e_angle + sin(zigzagOffset) * 0.5f) * baseSpeed;

    if (enemy_Y > 240.0f) enemy_Y = 240.0f;

    if (stateTimer > 90)
    {
        state = EnemyState::Wait;
        stateTimer = 0;
        zigzagOffset = 0.0f;
    }
}

void Enemy::TeleportingBehavior()
{
    if (stateTimer == 1)
    {
        enemy_X = static_cast<float>(rand() % 600 + 20);
        enemy_Y = static_cast<float>(rand() % 200 + 20);
    }

    if (enemy_Y > 240.0f) enemy_Y = 240.0f;
    if (stateTimer >= 30)
    {
        state = EnemyState::Wait;
        stateTimer = 0;
    }
}

void Enemy::ChangeToRandomState()
{
    int next = rand() % 3;
    switch (next)
    {
    case 0: state = EnemyState::Dash; break;
    case 1: state = EnemyState::Zigzag; break;
    case 2: state = EnemyState::Teleport; break;
    }
    stateTimer = 0;
    e_angle = (float)(rand() % 360) * (PI / 180.0f);
}

bool Enemy::CheckCollision(float bulletX, float bulletY, bool isPlayerBullet) const
{
    if (!isPlayerBullet) {
        return false;  // プレイヤーの弾でない場合は衝突しない
    }

    float dx = bulletX - enemy_X;
    float dy = bulletY - enemy_Y;
    float distanceSq = dx * dx + dy * dy;
    return distanceSq <= (radius + 8.0f) * (radius + 8.0f);  // 弾の半径も加算（8.0fはプレイヤー弾の半径）
}

void Enemy::OnHit()
{
    hp--;
    if (hp <= 0)
    {
        // 死亡処理（爆発演出とか）
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
    //// HP表示（敵の上に表示）
    //DrawFormatString(
    //    static_cast<int>(enemy_X - 10),
    //    static_cast<int>(enemy_Y - size / 2 - 16),
    //    GetColor(255, 255, 255),
    //    "HP: %d", hp
    //);
    int maxHP = 10;  // 最大HP（後で変数にしてもOK）
    int barX = 50;   // バーの左端位置
    int barY = 20;   // バーの上位置
    int barWidth = 540; // 全体のバーの長さ（画面幅640の約85%）
    int barHeight = 20;

    // 背景（枠のような見た目に）
    DrawBox(barX - 2, barY - 2, barX + barWidth + 2, barY + barHeight + 2, GetColor(255, 255, 255), FALSE); // 白枠
    DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(100, 100, 100), TRUE); // グレー背景

    // 現在のHP割合
    float hpRatio = static_cast<float>(hp) / maxHP;
    int currentWidth = static_cast<int>(barWidth * hpRatio);

    // 残HP（赤）
    DrawBox(barX, barY, barX + currentWidth, barY + barHeight, GetColor(255, 0, 0), TRUE);

    // 数値表示（任意）
    DrawFormatString(barX + barWidth / 2 - 20, barY + barHeight + 4, GetColor(255, 255, 255), "HP: %d", hp);

    //敵画像
    DrawGraph(enemy_X - 125, enemy_Y - 65, enemy_img, TRUE);
}
