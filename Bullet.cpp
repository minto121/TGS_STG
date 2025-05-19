#define _USE_MATH_DEFINES
#define NOMINMAX  
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include "Bullet.h"
#include"DxLib.h"
#include"FpsControl.h"
#include"demo_player.h"

#define PLAY_AREA_LEFT   0
#define PLAY_AREA_RIGHT  850
#define PLAY_AREA_TOP    0
#define PLAY_AREA_BOTTOM 720


template<typename T>
T Clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}

Bullet::Bullet() 
{
    Bullet_img = LoadGraph("Resource/image/defalte_Bullet.png");
    D_PLAYER = new demo_Player;
    px = 0.0f;
    py = 0.0f;

}

Bullet::~Bullet()
{
    delete D_PLAYER;

}

void Bullet::Update(int nowtime)
{
    if (D_PLAYER) {
        px = D_PLAYER->GetX();
        py = D_PLAYER->GetY();
    }


    printf("nowtime: %d\n", nowtime);
    for (auto& pattern : patterns) {
        if (!pattern.used && nowtime >= pattern.time) {

            float angleStep = 0;
            if (pattern.cnt > 1) {
                angleStep = (pattern.E_angle - pattern.S_angle) / (pattern.cnt - 1);
            }


            for (int i = 0; i < pattern.cnt; i++) {
                float angleDeg = pattern.S_angle + angleStep * i;
                float angleRad = angleDeg * (M_PI / 180.0f);

                BulletInstance bi;
                bi.x = pattern.x;
                bi.y = pattern.y;

                if (pattern.Homing) {
                    float dx = px - bi.x;
                    float dy = py - bi.y;
                    float len = sqrt(dx * dx + dy * dy);
                    if (len != 0) {
                        dx /= len;
                        dy /= len;
                    }

                    float angleRad = atan2f(dy, dx);

                    bi.angle = angleRad;
                    bi.speed = pattern.spd;
                    bi.vx = cosf(angleRad) * pattern.spd;
                    bi.vy = sinf(angleRad) * pattern.spd;
                    bi.homing = true;
                }
                else {
                    bi.vx = cos(angleRad) * pattern.spd;
                    bi.vy = sin(angleRad) * pattern.spd;
                    bi.homing = false;
                }

                bi.active = true; // �����K�v�I
                bi.reflect = globalReflectEnable; // ���ːݒ������ő��

                bullets.push_back(bi);  // �������Y�ꂸ�ɁI
            }
            pattern.used = true;
        }
    }

    // �e�̈ړ�
    float dt = 1.0f / 60.0f;//FpsControl_GetDeltaTime();
    const float maxTurn = 0.087f;  // �ő��]���x�i���W�A���Ŗ�5�x�j

    const int MAX_REFLECT_LIFETIME = 60 * 5; // ���ˌ�ő�5�b�ō폜
    for (auto& bi : bullets) {
        if (bi.active) {
            bi.x += bi.vx * dt;
            bi.y += bi.vy * dt;

            if (bi.reflect) {
                bool reflected = false;

                // ���E�̕ǂɔ���
                if (bi.x <= PLAY_AREA_LEFT || bi.x >= PLAY_AREA_RIGHT) {
                    bi.vx *= -1;
                    bi.x = Clamp<float>(bi.x, PLAY_AREA_LEFT, PLAY_AREA_RIGHT); // �͂ݏo���Ȃ��悤�ɏC��
                    reflected = true;
                }

                // �㉺�̕ǂɔ���
                if (bi.y <= PLAY_AREA_TOP || bi.y >= PLAY_AREA_BOTTOM) {
                    bi.vy *= -1;
                    bi.y = Clamp<float>(bi.y, PLAY_AREA_TOP, PLAY_AREA_BOTTOM); // �͂ݏo���Ȃ��悤�ɏC��
                    reflected = true;
                }

                // ���˂�����t���O�𗧂Ă�
                if (reflected) {
                    bi.reflectCnt++;
                    bi.CheckReflect = true;
                    if (bi.reflectCnt >= 2) {
                        bi.active = false;  // 2��ڂ̔��˂ō폜
                        continue;
                    }
                }

                // ���ˍς݂ŁA�͈͊O�ɏo����폜
                if (bi.CheckReflect) {
                    bi.reflectFrameCnt++;
                    if((bi.x < PLAY_AREA_LEFT || bi.x > PLAY_AREA_RIGHT ||
                        bi.y < PLAY_AREA_TOP || bi.y > PLAY_AREA_BOTTOM) ||
                        bi.reflectFrameCnt >= MAX_REFLECT_LIFETIME) {
                        bi.active = false;
                    }
                }
            }
            else {
                // �ʏ�̒e�F1�����˂��Ȃ��^�C�v
                if (bi.x < PLAY_AREA_LEFT || bi.x > PLAY_AREA_RIGHT ||
                    bi.y < PLAY_AREA_TOP || bi.y > PLAY_AREA_BOTTOM) {
                    bi.active = false;
                }
            }
        }
    }
}

void Bullet::LoadCSV(const char* filePath, int repeatCnt, int Interval)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        printf("�t�@�C���̃I�[�v���Ɏ��s���܂���: %s\n", filePath);
        return;
    }

    std::vector<B_State> basePatterns;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // ��s�̓X�L�b�v

        std::stringstream ss(line);
        std::string value;
        B_State b;

        try {
            std::getline(ss, value, ','); b.time = std::stoi(value);
            std::getline(ss, value, ','); b.x = std::stof(value);
            std::getline(ss, value, ','); b.y = std::stof(value);
            std::getline(ss, value, ','); b.S_angle = std::stof(value);
            std::getline(ss, value, ','); b.E_angle = std::stof(value);
            std::getline(ss, value, ','); b.cnt = std::stoi(value);
            std::getline(ss, value, ','); b.spd = std::stof(value);
            b.used = false; // ������ԂŖ��g�p�Ƃ���

            if (std::getline(ss, value, ',')) {
                b.Homing = (value == "true" || value == "1");
            }
            else {
                b.Homing = false; // �Â�CSV�p�Ƀf�t�H���g
            }

            basePatterns.push_back(b);
        }
        catch (...) {
            printf("CSV�ǂݍ��݃G���[: %s\n", line.c_str());
        }
    }

    // �J��Ԃ��ǉ��i5��j
    const int interval = 120; // �J��Ԃ��Ԋu�i�t���[���P�ʁA��: 2�b�j
    for (int i = 0; i < 5; i++) {
        for (auto& p : basePatterns) {
            B_State newP = p;
            newP.time += i * interval;
            newP.used = false;
            patterns.push_back(newP);
        }
    }

    for (int i = 0; i < repeatCnt; i++) {
        for (auto& p : basePatterns) {
            B_State newP = p;
            newP.time += i * interval;
            newP.used = false;
            patterns.push_back(newP);
        }
    }
}

void Bullet::ChangePattern(const char* filePath, int repeatCnt, int Interval)
{
    patterns.clear();
    bullets.clear();
    LoadCSV(filePath, repeatCnt, Interval);
}

//void Bullet::SetPlayer(demo_Player* player)
//{
//    D_PLAYER = player;
//}

void Bullet::SetReflectEnable(bool enable)
{
    globalReflectEnable = enable;
}


void Bullet::Draw()
{
    int bulletW, bulletH;
    GetGraphSize(Bullet_img, &bulletW, &bulletH);

    for (auto& b : bullets) {
        if (b.active) {
            DrawCircle((int)b.x, (int)b.y, 8, GetColor(255, 0, 0));
            DrawGraph((int)(b.x - bulletW / 2), (int)(b.y - bulletH / 2), Bullet_img, TRUE);
        }
    }

    //DrawFormatString(0, 0, GetColor(255, 255, 255), "nowtime: %d", );

}

std::vector<BulletInstance>& Bullet::GetBullets()  {
    return bullets;
 }