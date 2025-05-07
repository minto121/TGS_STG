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
		bool Homing = false; // 追尾かどうかのフラグ
		//float vx, vy;
	};

	BulletInstance bi;

	float px, py;
	int Bullet_img;
	int nowtime = 0;
	const std::vector<BulletInstance>& GetBullets() const;

	Bullet();
	~Bullet();
	void SetReflectEnable(bool enable);
	void Update(int nowtime);
	void Draw();
	void LoadCSV(const char* filePath, int repeatCnt, int Interval);
	void ChangePattern(const char* filePath, int repeatCnt, int Interval);
	//void SetPlayer(demo_Player* player);

	demo_Player* D_PLAYER;

	std::vector<B_State>patterns;
	std::vector<BulletInstance> bullets;



private:

	bool globalReflectEnable = false;

	

};

