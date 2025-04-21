#pragma once
#include "Player_Shot.h"
class demo_Player
{
public:
	
	Player_Shot* P_SHOT;
	float x, y;
	demo_Player();
	void Draw();
	void move();
	void fire(Player_Shot*P_SHOT);

private:
	int lastShotTime = 0;
	const int shotInterval = 500; // ミリ秒。必要に応じて調整
};

