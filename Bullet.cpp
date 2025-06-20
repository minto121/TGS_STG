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
    LoadDivGraph("Resource/image/通常弾幕.png", 8, 8, 1, 64, 32, Bullet_img);

    D_PLAYER = new demo_Player;
    px = 0.0f;
    py = 0.0f;

    SetEnemyPosition(ex,ey);
    
    int patternRepeatInterval = 120;
    int lastPatternTime = 0;
    int currentPatternType = -1; // 0: 通常, 1: 反射, 2: 追尾
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
    //    return;  // 死亡中なら弾幕更新しない
    //}

    //bool isPlayerAlive = D_PLAYER && D_PLAYER->IsAlive();

    if (D_PLAYER) {
        px = D_PLAYER->GetX();
        py = D_PLAYER->GetY();
    }

    //if (D_PLAYER && D_PLAYER->IsAlive()) {
    //    if (D_PLAYER && D_PLAYER->IsAlive() && !D_PLAYER->IsRespawn()) {
            //弾幕の生成
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

                        // fall処理
                        if (pattern.fall == true) {
                            bi.ay = 0.05f; // 重力で落下
                            //printf("fall bullet generated at time: %d\n", nowtime);

                        }
                        //追尾弾処理
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

                            // 後ろに尾弾を複数発射（例えば3つ）
                            const int tailCount = 3;
                            const float tailSpacing = 20.0f; // 距離（フレーム単位）
                            const int tailDelay = 16; // 2フレームごと


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

            //弾の移動
            for (auto& b : bullets) {
                if (!b.active) continue;

                // 通常移動
                b.vy += b.ay; // 加速度（重力）
                b.x += b.vx;
                b.y += b.vy;

                if (b.reflect == true) {
                    // 最大1回まで反射させる
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

                        // 画面外に出たら削除
                        if (b.x < PLAY_AREA_LEFT || b.x > PLAY_AREA_RIGHT
                            || b.y < PLAY_AREA_TOP || b.y > PLAY_AREA_BOTTOM) {
                            b.isAlive = false;
                        }
                    }
                }

                //// ホーミング弾の調整（任意）
                //if (b.homing == true) {
                //    float dx = px - ex;
                //    float dy = py - ey;
                //    float angle = atan2f(dy, dx);
                //    float targetVx = cosf(angle) * b.speed;
                //    float targetVy = sinf(angle) * b.speed;
                //    b.vx += (targetVx - b.vx) * b.homingStrength;
                //    b.vy += (targetVy - b.vy) * b.homingStrength;
                //}

                //// 画面外で無効化（任意）
                //if (b.x < PLAY_AREA_LEFT || b.x > PLAY_AREA_RIGHT ||
                //    b.y < PLAY_AREA_TOP || b.y > PLAY_AREA_BOTTOM) {
                //    b.active = false;
                //}

                // 波紋弾の寿命処理
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
                    newP.time = nowtime + (p.time % patternLoopInterval);  // 相対時間で再利用
                    newP.used = false;
                    //if (p.homing && nowtime > 0) continue; // ホーミング弾は1回だけにしたい場合
                    patterns.push_back(newP);
                }
                lastPatternLoopTime = nowtime;
            }
}

void Bullet::LoadCSV(const char* filePath, int repeatCnt, int Interval)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        printf("ファイルのオープンに失敗しました: %s\n", filePath);
        return;
    }

    std::vector<B_State> basePatterns;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // 空行はスキップ

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
            // 初期値
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
                b.fall = false; // 古いCSV用にデフォルト
            }
            //homing
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.homing = (value == "true" || value == "1");
            }
            else {
                b.homing = false; // 古いCSV用にデフォルト
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
            printf("CSV読み込みエラー: %s\n", line.c_str());
        }
    }

    // 繰り返し追加（5回）
    //const int interval = 120; // 繰り返し間隔（フレーム単位、例: 2秒）
    //for (int i = 0; i < 5; i++) {
    //    for (auto& p : basePatterns) {
    //        B_State newP = p;
    //        newP.time += i * interval;
    //        newP.used = false;
    //        patterns.push_back(newP);
    //    }
    //}


    //// パターン登録
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
    // 最初のループ分だけロード
    for (auto& p : basePatterns) {
        B_State newP = p;
        newP.time += 0;
        newP.used = false;
        patterns.push_back(newP);
    }

    ////確認用
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
            float speed = 4.0f; // 波紋の飛散速度

            b.rippleEffect = true;
            b.rippleFrame = 0;
            b.rippleVx = cosf(angle) * speed;
            b.rippleVy = sinf(angle) * speed;
            b.rippleLife = 30; // 波紋弾の寿命（フレーム）
        }
    }
}


void Bullet::Draw()
{
    int bulletW, bulletH;
    GetGraphSize(Bullet_img[7], &bulletW, &bulletH);

     for (auto& b : bullets) {
        if (!b.active) continue;

        int index = 0; // 画像インデックス
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
            index = 0; // それ以外の通常弾など
        }

        int bulletW, bulletH;
        GetGraphSize(Bullet_img[index], &bulletW, &bulletH);

        // 弾の座標は中心座標として描画
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