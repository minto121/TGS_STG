#pragma once
#include"Bullet.h"
#include"BulletInstance.h"
#include "Player_Shot.h"
#include "AbstractScene.h"
#include <vector>
class Bullet;

class demo_Player :public AbstractScene
{
public:

	bool Alive;			//生存中か
	bool Respawn;		//リスポーン
	int RespawnTimer;	//リスポーン中の無敵時間
	int Zanki;
	int player_img[12];

	float GetX()const { return x; }
	float GetY()const { return y; }
	
	//Player_Shot* P_SHOT;
	float x, y;
	float radius;
	demo_Player();
	void Update(const std::vector <BulletInstance> & bullets);
	void Draw()const override;
	void move();

	//void fire(Player_Shot*P_SHOT);
	void Hit();
	bool CheckHit(float x1, float y1, float r1, float x2, float y2, float r2);
	bool GameOver()const;
	bool IsAlive() const;
	bool IsRespawn() const;

	void SetBulletManager(Bullet* manager);


	AbstractScene* Update() override;

private:
	int lastShotTime = 0;
	const int shotInterval = 500; // ミリ秒。必要に応じて調整

};