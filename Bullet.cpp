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
    LoadDivGraph("Resource/image/大弾弾幕.png", 8, 8, 1, 128, 64, HomingBulletImg);
    LoadDivGraph("Resource/image/通常弾幕.png", 8, 8, 1, 64, 32, Bullet_img);
    LoadDivGraph("Resource/image/楔弾.png", 8, 8, 1, 32, 16, Kusabi_img);
    LoadDivGraph("Resource/image/炎弾.png", 16, 4, 4, 40, 40, (int*)Fire_img);


    D_PLAYER = new demo_Player;
    px = 0.0f;
    py = 0.0f;
    isSpiralActive = false;
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
    //    if (D_PLAYER && D_PLAYER->IsAlive() && !D_PLAYER-continue>IsRespawn()) {
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
                        bi.fall = pattern.fall;
                        bi.homing = pattern.homing;
                        bi.reflect = pattern.reflect;
                        bi.active = true;

                        // fall時に炎エフェクトを適用
                        if (pattern.fall) {
                            bi.ay = 0.03f; // 重力で落下
                            bi.fireEffect = true;
                            bi.fireColorIndex = 0; // 赤

                            //bi.active = true;
                        }
                    

                   /*     if (!pattern.homing && pattern.vx != 0.0f && pattern.vy != 0.0f) {
                            BulletInstance tail;
                            tail.x = ex;
                            tail.y = ey;
                            tail.vx = pattern.vx;
                            tail.vy = pattern.vy;
                            tail.speed = pattern.spd;
                            tail.active = true;
                            tail.homing = true;

                            bullets.push_back(tail);
                            pattern.used = true;
                        }*/
      

                        //// スパイラル弾の発射タイミング
                        //if (nowtime - spiralLastTime >= spiralInterval) {
                        //    spiralLastTime = nowtime;

                        //    BulletInstance b;
                        //    float angleRad = spiralAngle * (PI / 180.0f);
                        //    b.x = ex;
                        //    b.y = ey;
                        //    b.speed = 4.0f;
                        //    b.vx = cosf(angleRad) * b.speed;
                        //    b.vy = sinf(angleRad) * b.speed;
                        //    b.active = true;
                        //    b.homing = false;
                        //    b.reflect = false;
                        //    b.fall = false;

                        //    bullets.push_back(b);

                        //    spiralAngle += spiralSpeed;
                        //    if (spiralAngle >= 360.0f) spiralAngle -= 360.0f;
                        //}

                        if (pattern.spiral && !pattern.used) {
                            pattern.used = true; // 発射トリガーは一度だけ

                            spiralAngle = pattern.S_angle;
                            spiralSpeed = 6.0f; // 任意
                            spiralInterval = 4;
                            spiralLastTime = nowtime;

                            isSpiralActive = true;
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
                            bi.fall = pattern.fall;
                            bi.reflect = pattern.reflect;
                            bi.active = true;

                       
                            bullets.push_back(bi);


                            // 尾弾：まばらな delay と速度調整
                            const int tailCount = 5;
                            const float tailSpacing = 50.0f;

                            // 任意のまばらな発射遅延（例：ランダム or 固定）
                            int delayOffsets[3] = { 0, 10, 20 };  // まばらにしたい場合はここを工夫！

                            for (int i = 0; i < tailCount; ++i) {
                                float speedRatio = 1.0f - 0.1f * (i + 1); // 0.9, 0.8, 0.7...
                                float tailSpeed = pattern.spd * speedRatio;

                                BulletInstance tail;
                                tail.x = ex - cosA * tailSpacing * (i + 1);
                                tail.y = ey - sinA * tailSpacing * (i + 1);
                                tail.vx = cosA * tailSpeed;
                                tail.vy = sinA * tailSpeed;
                                tail.speed = tailSpeed;
                                tail.active = true;
                                tail.reflect = pattern.reflect;
                                tail.fall = pattern.fall;
                                tail.homing = false;
                                tail.isTail = true;

                                DelayedBullet db;
                                db.delay = delayOffsets[i];  // ← ここが「まばら」発射の決め手
                                db.instance = tail;

                                delayedBullets.push_back(db);
                            }
                        }
    
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

                // 炎アニメーションの進行
                if (b.fireEffect) {
                    b.fireAnimCounter++;
                    if (b.fireAnimCounter >= 6) {  // 6フレームごとに更新（任意）
                        b.fireAnimCounter = 0;
                        b.fireFrame = (b.fireFrame + 1) % 4;
                    }
                }

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

                //// 波紋弾の寿命処理
                //if (b.rippleEffect) {
                //    b.rippleFrame++;
                //    b.vx = b.rippleVx;
                //    b.vy = b.rippleVy;
                //    if (b.rippleFrame > b.rippleLife) {
                //        b.active = false;
                //    }
                //}
            }

            if (isSpiralActive && nowtime - spiralLastTime >= spiralInterval) {
                spiralLastTime = nowtime;

                BulletInstance b;
                float angleRad = spiralAngle * (PI / 180.0f);
                b.x = ex;
                b.y = ey;
                b.speed = 4.0f;
                b.vx = cosf(angleRad) * b.speed;
                b.vy = sinf(angleRad) * b.speed;
                b.active = true;
                b.angleDeg = spiralAngle;
                bullets.push_back(b);

                spiralAngle += spiralSpeed;
                if (spiralAngle >= 360.0f) spiralAngle -= 360.0f;
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

            // 遅延弾処理
            for (auto it = delayedBullets.begin(); it != delayedBullets.end(); ) {
                it->delay--;
                if (it->delay <= 0) {
                    bullets.push_back(it->instance);
                    it = delayedBullets.erase(it);
                }
                else {
                    ++it;
                }
            }
            ////衛星弾処理
            //for (auto& b : satelliteBullets) {
            //    if (!b.active) continue;

            //    b.orbitAngle += b.orbitSpeed;
            //    b.x = ex + cosf(b.orbitAngle) * b.orbitRadius;
            //    b.y = ey + sinf(b.orbitAngle) * b.orbitRadius;
            //}

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
            // spiral
            if (std::getline(ss, value, ',')) {
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                b.spiral = (value == "true" || value == "1");
            }
            else {
                b.spiral = false;
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

//void Bullet::CreateSatelliteBullets(int count, float radius, float speed) {
//    satelliteBullets.clear();
//
//    for (int i = 0; i < count; ++i) {
//        BulletInstance b;
//        b.orbitAngle = (2 * PI / count) * i;
//        b.orbitRadius = radius;
//        b.orbitSpeed = speed;
//        b.active = true;
//        b.isSatellite = true; // ← 新フラグ
//        satelliteBullets.push_back(b);
//    }
//}

void Bullet::EnableSpiral(float angle, float speed, int interval)
{
    spiralAngle = angle;
    spiralSpeed = speed;
    spiralInterval = interval;
    spiralLastTime = 0;
    isSpiralActive = true;
}

void Bullet::ReverseSpiralDirection()
{
    spiralSpeed *= -1.0f;
}


void Bullet::Draw()
{
    int bulletW, bulletH;
    GetGraphSize(Bullet_img[7], &bulletW, &bulletH);

    for (auto& b : bullets) {
        if (!b.active) continue;

        int index = 0; // 画像インデックス

           // 通常弾（フェーズ2以外）
        if (b.fireEffect) {
            int fw, fh;
            GetGraphSize(Fire_img[0][0], &fw, &fh);

            int color = Clamp(b.fireColorIndex, 3, 3);  // 0〜3 行目（赤・橙・青など）
            int frame = Clamp(b.fireFrame, 0, 2);       // 0〜2 フレーム（アニメーション）

            float scale = 2.0f;

            // vx, vy による向き + 上向き画像の補正（+90度）
            float angleRad = (b.vx != 0.0f || b.vy != 0.0f)
                ? atan2f(b.vy, b.vx) + DX_PI_F / 2  // ←ここが向きの補正部分
                : 0.0f;

            SetDrawBlendMode(DX_BLENDMODE_ADD,240);
            DrawRotaGraph2(
                (int)b.x,
                (int)b.y,
                fw / 2,
                fh / 2,
                scale,
                angleRad,                     // 回転角度（ラジアン）
                Fire_img[color][frame],
                TRUE
            );
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            continue;
        }

        // 優先度：homing > tail > kusabi > normal
        if (b.homing == true) {
            index = 0;
            GetGraphSize(HomingBulletImg[index], &bulletW, &bulletH);
            float scale = 2.0f;
            int drawX1 = (int)(b.x - (bulletW * scale) / 2);
            int drawY1 = (int)(b.y - (bulletH * scale) / 2);
            int drawX2 = drawX1 + (int)(bulletW * scale);
            int drawY2 = drawY1 + (int)(bulletH * scale);
            DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, HomingBulletImg[index], TRUE);
            continue;
        }
        else if (b.isTail) {
            index = GetRand(7);  // ランダムな尾弾色
            GetGraphSize(Bullet_img[index], &bulletW, &bulletH);
            float scale = 2.0f;
            int drawX1 = (int)(b.x - (bulletW * scale) / 2);
            int drawY1 = (int)(b.y - (bulletH * scale) / 2);
            int drawX2 = drawX1 + (int)(bulletW * scale);
            int drawY2 = drawY1 + (int)(bulletH * scale);

            SetDrawBlendMode(DX_BLENDMODE_ADD, 160);
            DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, Bullet_img[index], TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        }
        else if (currentPhase == 2) {
            printf("vx: %.2f, vy: %.2f\n", b.vx, b.vy);
            int index = 6;
            GetGraphSize(Kusabi_img[index], &bulletW, &bulletH);

            float scale = 2.5f;

            // vx, vy による向き + 上向き画像の補正（+90度）
            float angleRad = (b.vx != 0.0f || b.vy != 0.0f)
                ? atan2f(b.vy, b.vx) + DX_PI_F / 2
                : 0.0f;

            // 回転描画
            DrawRotaGraph2(
                (int)b.x,
                (int)b.y,
                bulletW / 2,
                bulletH / 2,
                scale,
                angleRad,               // 回転角ラジアン（vx, vy方向）
                Kusabi_img[index],
                TRUE
            );
        }
        else {
         

            if (b.reflected) {
                index = 5;
            }
            else {
                index = 0;
            }

            GetGraphSize(Bullet_img[index], &bulletW, &bulletH);
            DrawGraph((int)(b.x - bulletW / 2), (int)(b.y - bulletH / 2), Bullet_img[index], TRUE);
        }
    }

     //for (auto& b : satelliteBullets) {
     //    if (!b.active) continue;
     //    DrawGraph((int)(b.x - 16), (int)(b.y - 16), Bullet_img[4], TRUE); // 適当な画像
     //}

}

std::vector<BulletInstance>& Bullet::GetBullets()  {
    return bullets;
 }
void Bullet::SetEnemyPosition(float x, float y) {
    ex = x + 40;
    ey = y;
}

void Bullet::ClearAllBulletsInRange(float cx, float cy, float radius) {
    float r2 = radius * radius; // 距離の2乗で比較（sqrtを避けて高速化）
    for (auto& b : bullets) {
        if (!b.active) continue;

        float dx = b.x - cx;
        float dy = b.y - cy;
        if (dx * dx + dy * dy <= r2) {
            b.active = false;
        }
    }
}

void Bullet::ClearAllBullets() {
    for (auto& b : bullets) {
        b.active = false;
    }
}