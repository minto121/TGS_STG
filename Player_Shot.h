#pragma once
#define PSHOT_MAX 200

#include "PadInput.h"

class Player_Shot
{
public:
    //�L�����N�^�[�V���b�g�Ɋւ���\����
    typedef struct {
        int flag, power, cnt, knd;//�t���O�A�p���[�A�J�E���^�A���
        double x, y, angle, spd;//���W�A�p�x�A�X�s�[�h
        bool active;

    }pshot_t;

    pshot_t bullets[PSHOT_MAX];
    //int ShotTime;
    int lastShotTime;
    //int Bullet_img;
    int P_Shot_img[8];
    float x, y;


public:

    Player_Shot();

    void FireBullet(float playerX, float playerY);
    void StopAllBullets();
    void Update(float playerX, float playerY, bool canFire);
    void init();
    void Draw();


};

