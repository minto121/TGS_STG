#pragma once
#include "AbstractScene.h"
#include <cmath>

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
	Enemy(float x =320.0f, float y=0.0f);
    bool IsDead()const;
	~Enemy();

private:
    float enemy_X, enemy_Y;
    float baseX, baseY;
    float e_angle;      // �i�s�����i���W�A���j
    float dashSpeed;
    float zigzagOffset;

    int frameCount;
    int stateTimer;
    EnemyState state;

    int hp;
    float radius;       //�����蔻��p���a
    


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

    bool CheckCollision(float bulletX, float bulletY,bool isPlayerBullet) const;
    void OnHit();
};
   
