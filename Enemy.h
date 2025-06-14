#pragma once
#include "AbstractScene.h"
#include "Bullet.h"
#include <cmath>
#include <vector>

#define PI 3.1415926f

enum class EnemyState {
    Enter,
    Dash,
    Wait,
    Zigzag,
    Teleport,
};

class Enemy
{
public:
    Enemy(float x = 320.0f, float y = 0.0f);
    bool IsDead()const;
    ~Enemy();
    int GetHP() const;


    float GetX() const { return enemy_X; }
    float GetY() const { return enemy_Y; }
private:
    float enemy_X, enemy_Y;
    float baseX, baseY;
    float e_angle;      // 進行方向（ラジアン）
    float dashSpeed;
    float zigzagOffset;

    int frameCount;
    int stateTimer;
    EnemyState state;

    int hp;
    float radius;       //当たり判定用半径


    int enemy_img;

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

    bool CheckCollision(float bulletX, float bulletY, bool isPlayerBullet) const;
    void OnHit();


    bool isDying = false;       // 死亡演出中か
    int dyingTimer = 0;         // 死亡演出タイマー
    float dyingAlpha = 255.0f;  // 透明度
};

