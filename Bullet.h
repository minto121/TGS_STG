#pragma once
#include <vector>
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
		bool used = true;
	};

	struct BulletInstance {
		float x, y;
		float vx, vy;
		bool active = true;
	};

	int Bullet_img;
	int nowtime = 0;
	std::vector<BulletInstance>& GetBullets();


	Bullet();
	~Bullet();
	void Update(int nowtime);
	void Draw();
	void LoadCSV(const char* filePath);

	std::vector<B_State>patterns;
	std::vector<BulletInstance> bullets;

private:

	

};

