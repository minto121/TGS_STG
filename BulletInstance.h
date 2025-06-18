#pragma once
struct BulletInstance {
	float x, y = 0.0f;
	float vx, vy = 0.0f;
	float ay = 0.0f;
	float scale;
	bool isAlive = true;
	bool active = false;
	bool reflect = false;
	bool reflected = false;
	bool fall = false;
	bool homing = false;  // �z�[�~���O�e���ǂ����̃t���O
	bool CheckReflect;
	int time;
	int reflectCount = 0;

	float homingStrength = 0.05f;
	float angle = 0.0f;   // ���݂̌����i���W�A���j
	float speed = 0.0f;   // �e���i�z�[�~���O���ɕK�v�j

	 // �e���폜�p
	bool rippleEffect = false;
	int rippleFrame = 0;
	float rippleVx = 0;
	float rippleVy = 0;
	int rippleLife = 30; // �t���[�����Ŏ���
};