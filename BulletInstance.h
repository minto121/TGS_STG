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
	float S_angle = 0.0f;

	float homingStrength = 0.05f;
	float angle = 0.0f;   // 現在の向き（ラジアン）
	float speed = 0.0f;   // 弾速（ホーミング時に必要）
	bool isTail = false;
	float angleDeg = 0.0f;  

	 // 弾幕削除用
	bool rippleEffect = false;
	int rippleFrame = 0;
	float rippleVx = 0;
	float rippleVy = 0;
	int rippleLife = 30; // フレーム数で寿命

	////衛星弾
	//bool isOrbiting = false;
	//bool isSatellite = false;
	//float baseAngle = 0.0f;       // 進行方向（プレイヤー向き）
	//float orbitAngle = 0.0f;      // 現在の円運動角度
	//float orbitRadius = 10.0f;    // 回転半径
	//float orbitSpeed = 0.2f;      // 回転角速度

	//炎弾
	int fireFrame = 0;         // アニメーションフレーム（0〜3）
	int fireColorIndex = 0;    // 色：0〜3（赤〜紫）
	int fireAnimCounter = 0;   // アニメーション速度制御
	bool fireEffect = false;   // 炎エフェクトを使うか
};