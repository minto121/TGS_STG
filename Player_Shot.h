#pragma once
#define PSHOT_MAX 200

#include "PadInput.h"
class Bullet;

class Player_Shot
{
private:
    Bullet* bulletManager = nullptr;
public:
    //キャラクターショットに関する構造体
    typedef struct {
        int flag, power, cnt, knd;//フラグ、パワー、カウンタ、種類
        double x, y, angle, spd;//座標、角度、スピード
        bool active;

    }pshot_t;

    pshot_t bullets[PSHOT_MAX] = {};

    struct Bomb {
        bool active;
        float x, y;
        int timer;        // ボムの寿命（フレーム）
        float radius;     // 現在の半径
        float growthRate; // 1フレームあたりの拡大速度
    };
    //int ShotTime;
    int lastShotTime;
    //int Bullet_img;
    int P_Shot_img[8];
    float x = 0.0f;
    float y = 0.0f;


    static const int MAX_BOMBS = 1;  // 同時に出せるボム数
    Bomb bombs[MAX_BOMBS];
    int bombStock = 3;  // ボム残数
    //int bombCooldown = 0;

    int Shot_SE;


    void SetBulletManager(Bullet* manager) { bulletManager = manager; }

public:

    Player_Shot();

    void FireBullet(float playerX, float playerY);
    void StopAllBullets();
    void Update(float playerX, float playerY, bool canFire);
    void UseBomb(float px, float py);
    void SetBombStock(int stock);
    void init();
    void Draw();


};

