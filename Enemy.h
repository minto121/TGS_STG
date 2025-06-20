#pragma once
#include "AbstractScene.h"
#include "Bullet.h"
#include <cmath>
#include <vector>
class Bullet;

namespace EnemyConstants {
    constexpr float PI = 3.14159265f;
}

enum class EnemyState {
    Enter,
    Dash,
    Wait,
    Zigzag,
    Teleport,
};

enum class EnemyLifeState {
    ALIVE,
    DYING,   // 死亡演出中
    DEAD     // 完全に死亡（削除対象）
};

class Enemy
{
public:
    Bullet* BULLET_DATE;

    Enemy(float x = 320.0f, float y = 0.0f);

    ~Enemy();
    int GetHP() const;


    float GetX() const { return enemy_X; }
    float GetY() const { return enemy_Y; }

    void UpdateDying();
    bool IsDyingFinished() const;

    void RequestDying();
    bool IsRequestingDying() const;
    bool requestDying;
private:
    float enemy_X, enemy_Y;
    float baseX, baseY;
    float e_angle;      // 進行方向（ラジアン）
    float dashSpeed;
    float zigzagOffset;

    int frameCount;
    int stateTimer;
    EnemyState state;
    EnemyLifeState L_STATE;

    int hp;
    float radius;       //当たり判定用半径

    int enemy_img;

    //bool isDeadFlag = false;

    void EnteringBehavior();
    void WaitingBehavior();
    void DashingBehavior();
    void ZigZagBehavior();
    void TeleportingBehavior();



    void ChangeToRandomState();

public:
    // 
    void Update();

    // 
    void Draw() const;
    void StartDying();
    bool CheckCollision(float bulletX, float bulletY, bool isPlayerBullet) const;
    void OnHit();
    EnemyLifeState GetState() const { return L_STATE; }
    void SetState(EnemyLifeState newState) { L_STATE = newState; }
    bool IsDead() const;

    bool isDying = false;       // 死亡演出中か
    int dyingTimer = 0;         // 死亡演出タイマー
    float dyingAlpha = 255.0f;  // 透明度
};

