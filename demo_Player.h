#pragma once
#include "Player_Shot.h"
#include"Bullet.h"
#include <vector>
class demo_Player
{
public:

	bool Alive;			//��������
	bool Respawn;		//���X�|�[��
	int RespawnTimer;	//���X�|�[�����̖��G����

	
	//Player_Shot* P_SHOT;
	float x, y;
	float radius;
	demo_Player();
	void Update(const std::vector <Bullet::BulletInstance> & bullets);
	void Draw();
	void move();

	//void fire(Player_Shot*P_SHOT);
	void Hit();
	bool CheckHit(float x1, float y1, float r1, float x2, float y2, float r2);

private:
	int lastShotTime = 0;
	const int shotInterval = 500; // �~���b�B�K�v�ɉ����Ē���
};

