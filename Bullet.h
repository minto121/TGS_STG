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
		bool Homing = false; // �ǔ����ǂ����̃t���O
		//float vx, vy;
	};

	BulletInstance bi;

	float px, py;
	int Bullet_img;
	int nowtime = 0;
	std::vector<BulletInstance>& GetBullets();


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

