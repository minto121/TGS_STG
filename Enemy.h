#pragma once
#include "AbstractScene.h"
#include "Bullet.h"
#include <cmath>
#include <vector>

#define PI 3.1415926f

enum class EnemyState {
    Spiral
};

class Enemy
{
public:
	Enemy(float x =320.0f, float y=0.0f);
    bool IsDead()const;
	~Enemy();
    int GetHP() const;

    
    float GetX() const { return enemy_X; }
    float GetY() const { return enemy_Y; }
private:
    float enemy_X, enemy_Y;
    float baseX, baseY;
    float e_angle;      // �i�s�����i���W�A���j
    float dashSpeed;
    float zigzagOffset;
    float Spiral;

    int frameCount;
    int stateTimer;
    EnemyState state;

    int hp;
    float radius;       //�����蔻��p���a
    


    /*void EnteringBehavior();
    void WaitingBehavior();
    void DashingBehavior();
    void ZigZagBehavior();
    void TeleportingBehavior()*/
    void SpiralBehavior();

    /*void ChangeToRandomState();*/

public:
	// 
	void Update();

	// 
	void Draw() const;

    bool CheckCollision(float bulletX, float bulletY,bool isPlayerBullet) const;
    void OnHit();
};
   
