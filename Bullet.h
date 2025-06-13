#pragma once
#include <vector>
#include<string>
#include"demo_Player.h"
#include"BulletInstance.h"

class demo_Player;

class Bullet
{
public:

	struct B_State {
		int time;       // 発射タイミング（ms）
		float x = 0.0f;
		float y = 0.0f;     
		float vx, vy;
		float ay;
		float S_angle;    // 開始角度（deg）
		float E_angle;    // 終了角度（deg）
		int cnt;
		float spd;    // 弾速

		std::string type;
		//bool active = false;
		bool used = true;
		bool active = false; 
		bool reflect = false;
		bool fall = false;
		bool homing = false; // 追尾かどうかのフラグ
		bool ripple = false;
		//float vx, vy;
		int firedCount = 0;
	};

	BulletInstance bi;


	std::vector<B_State> basePatterns; // ←ヘッダーにも追加
	int patternLoopInterval = 120; // ループ間隔（フレーム）
	int lastPatternLoopTime = 0;

	float px, py;
	//float dy, dx;
	int Bullet_img[8];
	int nowtime = 0;
	bool homingFiredOnce = false;
	std::vector<BulletInstance>& GetBullets();


	Bullet();
	~Bullet();
	void SetReflectEnable(bool enable);
	void Update(int nowtime/*, float playerX, float playerY*/);
	void Draw();
	void LoadCSV(const char* filePath, int repeatCnt, int Interval);
	void ChangePattern(const char* filePath, int repeatCnt, int Interval);
	void SetPlayer(demo_Player* player);
	void TriggerRippleEffect(float cx, float cy, float radius);
	void StopAllBullets();
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

