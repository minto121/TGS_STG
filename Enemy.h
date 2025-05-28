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
    float e_angle;      // 進行方向（ラジアン）
    float dashSpeed;
    float zigzagOffset;
    float Spiral;

    int frameCount;
    int stateTimer;
    EnemyState state;

    int hp;
    float radius;       //当たり判定用半径
    


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
   
