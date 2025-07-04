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
	bool homing = false;  // ホーミング弾かどうかのフラグ
	bool CheckReflect;
	int time;
	int reflectCount = 0;

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