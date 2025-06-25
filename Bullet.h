#pragma once
#include <vector>
#include<string>
#include"demo_Player.h"
#include"Enemy.h"
#include"BulletInstance.h"

class demo_Player;
class Enemy;

namespace BulletConstants {
	constexpr float PI = 3.14159265f;
}

class Bullet
{
public:

	demo_Player* D_PLAYER;
	Enemy* enemyRef;

	struct B_State {
		int time;       // ���˃^�C�~���O�ims�j
		float x = 0.0f;
		float y = 0.0f;     
		float vx = 0.0f;
		float vy = 0.0f;
		float ay = 0.0f;
		float S_angle= 0.0f;    // �J�n�p�x�ideg�j
		float E_angle = 0.0f;    // �I���p�x�ideg�j
		int cnt;
		bool spiral;  
		float spd = 0.0f;    // �e��

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

	struct DelayedBullet {
		int delay;
		BulletInstance instance;
	};

	std::vector<DelayedBullet> delayedBullets;//���e
	std::vector<BulletInstance> satelliteBullets;//�q���e

	std::vector<B_State> basePatterns; // ���w�b�_�[�ɂ��ǉ�
	int patternLoopInterval = 120; // ���[�v�Ԋu�i�t���[���j
	int lastPatternLoopTime = 0;

	float px, py;
	//float dy, dx;
	int Bullet_img[8];
	int HomingBulletImg[8];
	int Kusabi_img[8];
	int nowtime = 0;
	bool homingFiredOnce = false;
	bool enemyAlive = true;
	bool isSpiralActive = false;
	std::vector<BulletInstance>& GetBullets();

	float spiralAngle = 0.0f;  // �����p�x
	float spiralSpeed = 5.0f;  // ���t���[����]����p�x�i�x���j
	int spiralInterval = 5;    // ���ˊԊu�i�t���[���j
	int spiralLastTime = 0;


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
	void SetEnemyRef(Enemy* e);
	void SetEnemyAlive(bool isAlive);
	void CreateSatelliteBullets(int count, float radius, float speed);
	void EnableSpiral(float angle = 0.0f, float speed = 6.0f, int interval = 4);
	void ReverseSpiralDirection();
	void SetPhase(int phase) { currentPhase = phase; }

	std::vector<B_State>patterns;
	std::vector<BulletInstance> bullets;
private:
	int currentPhase = 0;  // �t�F�[�Y�ԍ��iGameMain����n���j
	bool globalReflectEnable = false;
	float ex=0.0f;
	float ey=0.0f;
	

};

