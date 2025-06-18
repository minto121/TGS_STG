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

template<typename T>
T Clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}


#define PLAY_AREA_LEFT   0.0f
#define PLAY_AREA_RIGHT  850.0f
#define PLAY_AREA_TOP    0.0f
#define PLAY_AREA_BOTTOM 720.0f

const float PI = 3.14159265f;


//std::vector<BulletInstance>& Bullet::GetBullets()
//{
//    return bullets;
//}

Bullet::Bullet()
{
    //Bullet_img = LoadGraph("Resource/image/defalte_Bullet.png");
    LoadDivGraph("Resource/image/�ʏ�e��.png", 8, 8, 1, 64, 32, Bullet_img);

    D_PLAYER = new demo_Player;
    px = 0.0f;
    py = 0.0f;

    SetEnemyPosition(ex,ey);
    
    int patternRepeatInterval = 120;
    int lastPatternTime = 0;
    int currentPatternType = -1; // 0: �ʏ�, 1: ����, 2: �ǔ�
    bool repeatWhileAlive = false;
}

Bullet::~Bullet()
{
    delete D_PLAYER;
}


void Bullet::SetPlayer(demo_Player* player)
{
    D_PLAYER = player;
}

void Bullet::Update(int nowtime/*,float playerX,float playerY*/)
{

    if (!enemyAlive) return;

    //if (enemyRef != nullptr && enemyRef->IsDead()) {
    //    return;  // ���S���Ȃ�e���X�V���Ȃ�
    //}

    //bool isPlayerAlive = D_PLAYER && D_PLAYER->IsAlive();

    if (D_PLAYER) {
        px = D_PLAYER->GetX();
        py = D_PLAYER->GetY();
    }

    //if (D_PLAYER && D_PLAYER->IsAlive()) {
    //    if (D_PLAYER && D_PLAYER->IsAlive() && !D_PLAYER->IsRespawn()) {
            //�e���̐���
            printf("nowtime: %d\n", nowtime);
            for (auto& pattern : patterns) {
                if (!pattern.used && nowtime >= pattern.time) {
                    float angleStep = (pattern.cnt > 1) ? (pattern.E_angle - pattern.S_angle) / (pattern.cnt - 1) : 0;

                    for (int i = 0; i < pattern.cnt; ++i) {
                        float angleDeg = pattern.S_angle + angleStep * i;
                        float angleRad = angleDeg * (BulletConstants::PI / 180.0f);

                        BulletInstance bi;
                        bi.x = ex;
                        bi.y = ey;
                        bi.speed = pattern.spd;
                        bi.ay = 0.0f;
                        bi.vx = cosf(angleRad) * pattern.spd;
                        bi.vy = sinf(angleRad) * pattern.spd;

                        if (!pattern.homing && pattern.vx != 0.0f && pattern.vy != 0.0f) {
                            BulletInstance tail;
                            tail.x = ex;
                            tail.y = ey;
                            tail.vx = pattern.vx;
                            tail.vy = pattern.vy;
                            tail.speed = pattern.spd;
                            tail.active = true;

                            bullets.push_back(tail);
                            pattern.used = true;
                        }

                        // fall����
                        if (pattern.fall == true) {
                            bi.ay = 0.05f; // �d�͂ŗ���
                            //printf("fall bullet generated at time: %d\n", nowtime);

                        }
                        //�ǔ��e����
                        if (pattern.homing && !pattern.used) {
                            float dx = px - ex;
                            float dy = py - ey;
                            float angle = atan2f(dy, dx);
                            float cosA = cosf(angle);
                            float sinA = sinf(angle);


                            bi.vx = cosA * pattern.spd;
                            bi.vy = sinA * pattern.spd;
                            bi.homing = true;
                            bi.homingStrength = 0.5f;

                            // ���ɔ��e�𕡐����ˁi�Ⴆ��3�j
                            const int tailCount = 3;
                            const float tailSpacing = 20.0f; // �����i�t���[���P�ʁj
                            const int tailDelay = 16; // 2�t���[������


                            if (!pattern.homing) {
                                BulletInstance tail;
                                tail.x = ex;
                                tail.y = ey;
                                tail.speed = pattern.spd;
                                tail.vx = cosf(angleRad) * pattern.spd;
                                tail.vy = sinf(angleRad) * pattern.spd;
                                tail.active = true;
                                tail.reflect = pattern.reflect;
                                tail.fall = pattern.fall;

                                bullets.push_back(tail);
                            }
                        }
                        bi.fall = pattern.fall;
                        bi.homing = pattern.homing;
                        bi.reflect = pattern.reflect;
                        bi.active = true;
                        bullets.push_back(bi);

                    }
                    pattern.used = true;
                }
            }

            //�e�̈ړ�
            for (auto& b : bullets) {
                if (!b.active) continue;

                // �ʏ�ړ�
                b.vy += b.ay; // �����x�i�d�́j
                b.x += b.vx;
                b.y += b.vy;

                if (b.reflect == true) {
                    // �ő�1��܂Ŕ��˂�����
                    if (b.reflectCount == 0) {
                        //bool reflected = false;

                        if (b.x < PLAY_AREA_LEFT || b.x > PLAY_AREA_RIGHT) {
                            b.vx *= -1;
                            b.reflected = true;
                        }
                        if (b.y < PLAY_AREA_TOP || b.y > PLAY_AREA_BOTTOM) {
                            b.vy *= -1;
                            b.reflected = true;
                        }

                        if (b.reflected) {
                            b.reflectCount = 1;
                        }
                    }else {

                        // ��ʊO�ɏo����폜
                        if (b.x < PLAY_AREA_LEFT || b.x > PLAY_AREA_RIGHT
                            || b.y < PLAY_AREA_TOP || b.y > PLAY_AREA_BOTTOM) {
                            b.isAlive = false;
                        }
                    }
                }

                //// �z�[�~���O�e�̒����i�C�Ӂj
                //if (b.homing == true) {
                //    float dx = px - ex;
                //    float dy = py - ey;
                //    float angle = atan2f(dy, dx);
                //    float targetVx = cosf(angle) * b.speed;
                //    float targetVy = sinf(angle) * b.speed;
                //    b.vx += (targetVx - b.vx) * b.homingStrength;
                //    b.vy += (targetVy - b.vy) * b.homingStrength;
                //}

                //// ��ʊO�Ŗ������i�C�Ӂj
                //if (b.x < PLAY_AREA_LEFT || b.x > PLAY_AREA_RIGHT ||
                //    b.y < PLAY_AREA_TOP || b.y > PLAY_AREA_BOTTOM) {
                //    b.active = false;
                //}

                // �g��e�̎�������
                if (b.rippleEffect) {
                    b.rippleFrame++;
                    b.vx = b.rippleVx;
                    b.vy = b.rippleVy;
                    if (b.rippleFrame > b.rippleLife) {
                        b.active = false;
                    }
                }
            }

            if (nowtime - lastPatternLoopTime >= patternLoopInterval) {
                for (auto& p : basePatterns) {
                    B_State newP = p;
                    newP.time = nowtime + (p.time % patternLoopInterval);  // ���Ύ��Ԃōė��p
                    newP.used = false;
                    //if (p.homing && nowtime > 0) continue; // �z�[�~���O�e��1�񂾂��ɂ������ꍇ
                    patterns.push_back(newP);
                }
                lastPatternLoopTime = nowtime;
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
            // �����l
 /*           b.used = false;*/
  /*          b.fall = false;
            b.homing = false;
            b.reflect = false;
            b.ripple = false;*/
            //fall
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.fall = (value == "true" || value == "1");
            }
            else {
                b.fall = false; // �Â�CSV�p�Ƀf�t�H���g
            }
            //homing
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.homing = (value == "true" || value == "1");
            }
            else {
                b.homing = false; // �Â�CSV�p�Ƀf�t�H���g
            }
  
            // reflect
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.reflect = (value == "true" || value == "1");
            }

            // ripple
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.ripple = (value == "true" || value == "1");
            }
            basePatterns.push_back(b);
        }
        catch (...) {
            printf("CSV�ǂݍ��݃G���[: %s\n", line.c_str());
        }
    }

    // �J��Ԃ��ǉ��i5��j
    //const int interval = 120; // �J��Ԃ��Ԋu�i�t���[���P�ʁA��: 2�b�j
    //for (int i = 0; i < 5; i++) {
    //    for (auto& p : basePatterns) {
    //        B_State newP = p;
    //        newP.time += i * interval;
    //        newP.used = false;
    //        patterns.push_back(newP);
    //    }
    //}


    //// �p�^�[���o�^
    //for (int i = 0; i < 5; i++) {
    //    for (auto& p : basePatterns) {
    //        if (p.homing && i > 0) continue;
    //        B_State newP = p;
    //        newP.time += i * interval;
    //        newP.used = false;
    //        patterns.push_back(newP);
    //    }
    //}


    this->basePatterns = basePatterns;
    // �ŏ��̃��[�v���������[�h
    for (auto& p : basePatterns) {
        B_State newP = p;
        newP.time += 0;
        newP.used = false;
        patterns.push_back(newP);
    }

    ////�m�F�p
    //int homingCount = 0;
    //int totalCount = 0;
    //bool lastHoming = false;
    //for (auto& bi : bullets) {
    //    if (bi.active) {
    //        if (bi.homing) homingCount++;
    //        totalCount++;
    //        lastHoming = bi.homing;
    //    }
    //}
    //printf("active bullets: %d, homing bullets: %d\n", totalCount, homingCount);

}

void Bullet::StopAllBullets() {
    for (auto& b : bullets) {
        b.speed = 0.0f;
    }
}

void Bullet::ChangePattern(const char* filePath, int repeatCnt, int Interval)
{
    patterns.clear();
    bullets.clear();
    LoadCSV(filePath, repeatCnt, Interval);
}


void Bullet::SetReflectEnable(bool enable)
{
    globalReflectEnable = enable;
}

//void Bullet::ClearBulletsAround(float cx, float cy, float radius)
//{
//    float radiusSq = radius * radius;
//
//    for (auto& b : bullets) {
//        if (!b.active) continue;
//
//        float dx = b.x - cx;
//        float dy = b.y - cy;
//
//        if ((dx * dx + dy * dy) <= radiusSq) {
//            b.active = false;
//        }
//    }
//}

void Bullet::SetEnemyAlive(bool isAlive) {
    enemyAlive = isAlive;
}

void Bullet::SetEnemyRef(Enemy* e) {
    enemyRef = e;
}
void Bullet::TriggerRippleEffect(float cx, float cy, float radius)
{
    float radiusSq = radius * radius;

    for (auto& b : bullets) {
        if (!b.active) continue;

        float dx = b.x - cx;
        float dy = b.y - cy;
        if ((dx * dx + dy * dy) <= radiusSq) {
            float angle = atan2f(dy, dx);
            float speed = 4.0f; // �g��̔�U���x

            b.rippleEffect = true;
            b.rippleFrame = 0;
            b.rippleVx = cosf(angle) * speed;
            b.rippleVy = sinf(angle) * speed;
            b.rippleLife = 30; // �g��e�̎����i�t���[���j
        }
    }
}


void Bullet::Draw()
{
    int bulletW, bulletH;
    GetGraphSize(Bullet_img[7], &bulletW, &bulletH);

     for (auto& b : bullets) {
        if (!b.active) continue;

        int index = 0; // �摜�C���f�b�N�X
        if (b.fall == true) {
            index = 6;
        }
        else if (b.reflect == true) {
            index = 0;
        }
        else if (b.homing == true) {
            index = 2;
        }
        else {
            index = 0; // ����ȊO�̒ʏ�e�Ȃ�
        }

        int bulletW, bulletH;
        GetGraphSize(Bullet_img[index], &bulletW, &bulletH);

        // �e�̍��W�͒��S���W�Ƃ��ĕ`��
        DrawGraph((int)(b.x - bulletW / 2), (int)(b.y - bulletH / 2), Bullet_img[index], TRUE);
    }

}

std::vector<BulletInstance>& Bullet::GetBullets()  {
    return bullets;
 }
void Bullet::SetEnemyPosition(float x, float y) {
    ex = x;
    ey = y;
}

void Bullet::ClearAllBullets() {
    for (auto& b : bullets) {
        b.active = false;
    }
}