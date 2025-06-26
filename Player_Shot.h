#pragma once
#define PSHOT_MAX 200

#include "PadInput.h"
class Bullet;

class Player_Shot
{
private:
    Bullet* bulletManager = nullptr;
public:
    //�L�����N�^�[�V���b�g�Ɋւ���\����
    typedef struct {
        int flag, power, cnt, knd;//�t���O�A�p���[�A�J�E���^�A���
        double x, y, angle, spd;//���W�A�p�x�A�X�s�[�h
        bool active;

    }pshot_t;

    pshot_t bullets[PSHOT_MAX] = {};

    struct Bomb {
        bool active;
        float x, y;
        int timer;        // �{���̎����i�t���[���j
        float radius;     // ���݂̔��a
        float growthRate; // 1�t���[��������̊g�呬�x
    };
    //int ShotTime;
    int lastShotTime;
    //int Bullet_img;
    int P_Shot_img[8];
    float x = 0.0f;
    float y = 0.0f;


    static const int MAX_BOMBS = 1;  // �����ɏo����{����
    Bomb bombs[MAX_BOMBS];
    int bombStock = 3;  // �{���c��
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

