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


public:

    Player_Shot();

    void FireBullet(float playerX, float playerY);

    void Update(float playerX, float playerY);
    void init();
    void Draw();


};

