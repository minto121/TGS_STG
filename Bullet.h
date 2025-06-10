#pragma once
#include <vector>
#include"demo_Player.h"
#include"BulletInstance.h"

class demo_Player;

class Bullet
{
public:

	struct B_State {
		int time;       // 発射タイミング（ms）
		float x, y;     // 発射位置
		float S_angle;    // 開始角度（deg）
		float E_angle;    // 終了角度（deg）
		int cnt;
		float spd;    // 弾速
		//bool active = false;
		bool used = true;
		bool homing = false; // 追尾かどうかのフラグ
		//float vx, vy;
		int firedCount = 0;
	};



	BulletInstance bi;

	float px, py;
	float dy, dx;
	int Bullet_img;
	int nowtime = 0;
	std::vector<BulletInstance>& GetBullets();


	Bullet();
	~Bullet();
	void SetReflectEnable(bool enable);
	void Update(int nowtime/*, float playerX, float playerY*/);
	void Draw();
	void LoadCSV(const char* filePath, int repeatCnt, int Interval);
	void ChangePattern(const char* filePath, int repeatCnt, int Interval);
	void SetPlayer(demo_Player* player);

	void SetEnemyPosition(float x, float y);
	void ClearAllBullets();
	demo_Player* D_PLAYER;

	std::vector<B_State>patterns;
	std::vector<BulletInstance> bullets;



private:

	bool globalReflectEnable = false;
	float ex=0.0f;
	float ey=0.0f;
	

};

