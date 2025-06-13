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
		int time;       // ���˃^�C�~���O�ims�j
		float x = 0.0f;
		float y = 0.0f;     
		float vx, vy;
		float ay;
		float S_angle;    // �J�n�p�x�ideg�j
		float E_angle;    // �I���p�x�ideg�j
		int cnt;
		float spd;    // �e��

		std::string type;
		//bool active = false;
		bool used = true;
		bool active = false; 
		bool reflect = false;
		bool fall = false;
		bool homing = false; // �ǔ����ǂ����̃t���O
		bool ripple = false;
		//float vx, vy;
		int firedCount = 0;
	};

	BulletInstance bi;


	std::vector<B_State> basePatterns; // ���w�b�_�[�ɂ��ǉ�
	int patternLoopInterval = 120; // ���[�v�Ԋu�i�t���[���j
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

