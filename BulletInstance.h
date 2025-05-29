#pragma once
struct BulletInstance {
	float x, y;
	float vx, vy;
	bool isAlive;
	bool active;
	bool reflect;
	bool homing = false;  // ホーミング弾かどうかのフラグ
	bool CheckReflect;
	int reflectFrameCnt;
	int reflectCnt = 0;

	float homingStrength = 0.05f;
	float angle = 0.0f;   // 現在の向き（ラジアン）
	float speed = 0.0f;   // 弾速（ホーミング時に必要）

	 // 弾幕削除用
	bool rippleEffect = false;
	int rippleFrame = 0;
	float rippleVx = 0;
	float rippleVy = 0;
	int rippleLife = 30; // フレーム数で寿命
};