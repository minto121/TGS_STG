#pragma once
#include <vector>
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

