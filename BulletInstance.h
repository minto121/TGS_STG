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

	float angle = 0.0f;   // 現在の向き（ラジアン）
	float speed = 0.0f;   // 弾速（ホーミング時に必要）
	bool homing = false;  // ホーミング弾かどうかのフラグ
};