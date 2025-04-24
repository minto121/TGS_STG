#pragma once
#include "AbstractScene.h"
#include <vector>


class Enemy
{
public:
	Enemy(float x =320.0f, float y=0.0f);

	~Enemy();

private:
	float enemy_X, enemy_Y;
	float e_Speed;
	float e_angle;
	


	float baseX, baseY;			//�����̒��S�ʒu
	int frameCount=0;				//�o�ߎ��Ԃ̃J�E���g
public:
	// 
	void Update();

	// 
	void Draw() const;

};
