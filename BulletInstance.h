#pragma once
struct BulletInstance {
	float x, y;
	float vx, vy;
	bool isAlive;
	bool active;
	bool reflect;
	bool homing = false;  // �z�[�~���O�e���ǂ����̃t���O
	bool CheckReflect;
	int reflectFrameCnt;
	int reflectCnt = 0;

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