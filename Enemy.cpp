#include "Enemy.h"
#include "GameMain.h"
#include "Bullet.h"
#include <cmath>
#include <DxLib.h>
#include <cstdlib>

Enemy::Enemy(float x, float y)
    : enemy_X(x), enemy_Y(y), baseX(x), baseY(y),
    e_angle(PI / 2), dashSpeed(4.0f), zigzagOffset(0.0f),
    frameCount(0), stateTimer(0), state(EnemyState::Enter),
    hp(10), radius(18.0f)
{
    BULLET_DATE = new Bullet;
    enemy_img = LoadGraph("Resource/image/Enemy_image/pipo-boss001.png");
}

Enemy::~Enemy() {}

void Enemy::Update()
{
    frameCount++;
    stateTimer++;

    //if (isDying) {
    //    dyingTimer++;

    //    // 震え効果：小刻みにランダム移動
    //    enemy_X += (rand() % 7) - 3; // -2〜2の範囲で揺らす
    //    enemy_Y += (rand() % 7) - 3;

    //    // 徐々に透明に（5秒 = 300フレームで 255 → 0）
    //    dyingAlpha = 255.0f * (1.0f - dyingTimer / 300.0f);

    //    if (dyingTimer >= 300) {
    //        L_STATE = EnemyLifeState::DEAD;  // ここで明示的にDEADへ
    //        //isDeadFlag = true;  // 死亡完了を通知
    //    }

    //    return; // 死亡中は動かさない
    //}

    if (L_STATE != EnemyLifeState::ALIVE) return;

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

void Enemy::UpdateDying() {
    dyingTimer++;
    dyingAlpha = 255.0f * (1.0f - dyingTimer / 300.0f);
    enemy_X += (rand() % 7) - 3;
    enemy_Y += (rand() % 7) - 3;
    if (dyingTimer >= 300) {
        L_STATE = EnemyLifeState::DEAD;
    }
}

bool Enemy::IsRequestingDying() const {
    return requestDying;

}

void Enemy::RequestDying() {
    requestDying = true;
}

bool Enemy::IsDyingFinished() const {
    return dyingTimer >= 300;
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
    if (L_STATE != EnemyLifeState::ALIVE) return false;  // 死亡した敵は当たり判定なし

    if (!isPlayerBullet) {
        return false;  // プレイヤーの弾でない場合は衝突しない
    }

    float dx = bulletX - (enemy_X-10.0f);
    float dy = bulletY - enemy_Y;
    float distanceSq = dx * dx + dy * dy;

    return distanceSq <= (radius + 25.0f) * (radius + 25.0f);  // �e�̔��a����Z�i8.0f�̓v���C���[�e�̔��a�j

}

void Enemy::OnHit()
{
    // if (isDying) return;  // 死亡中はダメージ受けない
    //hp--;
  /*  if (hp <= 0) {
        isDying = true;
        dyingTimer = 0;
    }*/

    if (L_STATE != EnemyLifeState::ALIVE) return;

    hp--;

    add_score = 0;
    add_score = add_score + 100;

    //if (hp <= 0) StartDying();
}

bool Enemy::IsDead() const
{
    //return isDeadFlag;;
    return L_STATE == EnemyLifeState::DEAD;
}
int Enemy::GetHP()const {
    return hp;
}

void Enemy::StartDying() {
    if (L_STATE == EnemyLifeState::ALIVE) {
        L_STATE = EnemyLifeState::DYING;
        dyingTimer = 0;
        dyingAlpha = 255.0f;
    }
}

void Enemy::Draw() const
{
    int size = 32;

    //DrawBox(
    //    static_cast<int>(enemy_X - size / 2),
    //    static_cast<int>(enemy_Y - size / 2),
    //    static_cast<int>(enemy_X + size / 2),
    //    static_cast<int>(enemy_Y + size / 2),
    //    GetColor(255, 0, 0),
    //    TRUE
    //);
    //DrawCircle(static_cast<int>(enemy_X), static_cast<int>(enemy_Y), static_cast<int>(radius + 8.0f), GetColor(0, 255, 255), FALSE);

    int maxHP = 10;  // �ő�HP�i��ŕϐ��ɂ��Ă�OK�j
    int barX = 50;   // �o�[�̍��[�ʒu
    int barY = 20;   // �o�[�̏�ʒu
    int barWidth = 540; // �S�̂̃o�[�̒����i��ʕ�640�̖�85%�j

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
    //DrawGraph(enemy_X - 125, enemy_Y - 65, enemy_img, TRUE);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)dyingAlpha); // 透明度指定
    DrawGraph(static_cast<int>(enemy_X) - 125, static_cast<int>(enemy_Y) - 65, enemy_img, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // リセット
}

int Enemy::GetAddScore() {
    int return_score = 0;
    return_score = add_score;
    add_score = 0;
    return return_score;
}
