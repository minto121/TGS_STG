#pragma once
#define PSHOT_MAX 200

#include "PadInput.h"

class Player_Shot
{
public:
    //キャラクターショットに関する構造体
    typedef struct {
        int flag, power, cnt, knd;//フラグ、パワー、カウンタ、種類
        double x, y, angle, spd;//座標、角度、スピード
        bool active;

    }pshot_t;

    pshot_t bullets[PSHOT_MAX];
    //int ShotTime;
    int lastShotTime;
    //int Bullet_img;
    int P_Shot_img[8];


public:

    Player_Shot();

    void FireBullet(float playerX, float playerY);

    void Update(float playerX, float playerY);
    void init();
    void Draw();


};

