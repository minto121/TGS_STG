#define _USE_MATH_DEFINES
#include "Bullet.h"
#include"DxLib.h"
#include <vector>
#include <fstream>
#include <sstream>
#include"FpsControl.h"


Bullet::Bullet() 
{
    Bullet_img = LoadGraph("Resource/image/defalte_Bullet.png");
}

Bullet::~Bullet()
{
}

void Bullet::Update(int nowtime)
{

    printf("nowtime: %d\n", nowtime); // ← これを追加！
    for (auto& pattern : patterns) {
        if (!pattern.used && nowtime >= pattern.time) {
            float angleStep = (pattern.E_angle - pattern.S_angle) / (pattern.cnt - 1);

            if (pattern.cnt > 1) {
                angleStep = (pattern.E_angle - pattern.S_angle) / (pattern.cnt - 1);
            }

            for (int i = 0; i < pattern.cnt; i++) {
                float angleDeg = pattern.S_angle + angleStep * i;
                float angleRad = angleDeg * (M_PI / 180.0f);
                BulletInstance b;
                b.x = pattern.x;
                b.y = pattern.y;
                b.vx = cos(angleRad) * pattern.spd;
                b.vy = sin(angleRad) * pattern.spd;
                b.active = true;
                bullets.push_back(b);
            }
            pattern.used = true;
        }
    }

    // 弾の移動
    float dt = FpsControl_GetDeltaTime();
    for (auto& b : bullets) {
        if (b.active) {
            b.x += b.vx * dt;
            b.y += b.vy * dt;

            // 範囲外で非アクティブ
            if (b.x < 0 || b.x > 700 || b.y < 0 || b.y > 720) {
                b.active = false;
            }
        }
    }
}



void Bullet::LoadCSV(const char* filePath)
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
            b.used = false; // 初期状態で未使用とする

            basePatterns.push_back(b);
        }
        catch (...) {
            printf("CSV読み込みエラー: %s\n", line.c_str());
        }
    }

    // 繰り返し追加（5回）
    const int interval = 120; // 繰り返し間隔（フレーム単位、例: 2秒）
    for (int i = 0; i < 5; i++) {
        for (auto& p : basePatterns) {
            B_State newP = p;
            newP.time += i * interval;
            newP.used = false;
            patterns.push_back(newP);
        }
    }
}


void Bullet::Draw()
{
    for (auto& b : bullets) {
        if (b.active) {
            //DrawCircle((int)b.x, (int)b.y, 4, GetColor(255, 0, 0));
            DrawGraph((int)b.x, (int)b.y, Bullet_img, TRUE);

        }
    }

    //DrawFormatString(0, 0, GetColor(255, 255, 255), "nowtime: %d", );

}