#pragma once
#include "AbstractScene.h"

class Help :public AbstractScene
{
private:

public:
	//コンストラクタ
	Help();
	//デストラクタ
	~Help();

	// 更新処理
	virtual AbstractScene* Update() override;

	// 描画処理
	virtual void Draw() const override;

};