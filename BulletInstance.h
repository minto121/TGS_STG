#pragma once
struct BulletInstance {
	float x, y;
	float vx, vy;
	bool isAlive;
	bool active;
	bool reflect;
	bool CheckReflect;
	int reflectFrameCnt;
	int reflectCnt = 0;

	float angle = 0.0f;   // ���݂̌����i���W�A���j
	float speed = 0.0f;   // �e���i�z�[�~���O���ɕK�v�j
	bool homing = false;  // �z�[�~���O�e���ǂ����̃t���O
};