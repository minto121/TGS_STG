#pragma once
#include <vector>
#include"demo_Player.h"
#include"BulletInstance.h"

class demo_Player;

class Bullet
{
public:

	struct B_State {
		int time;       // ���˃^�C�~���O�ims�j
		float x, y;     // ���ˈʒu
		float S_angle;    // �J�n�p�x�ideg�j
		float E_angle;    // �I���p�x�ideg�j
		int cnt;
		float spd;    // �e��
		//bool active = false;
		bool used = true;
		bool homing = false; // �ǔ����ǂ����̃t���O
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

