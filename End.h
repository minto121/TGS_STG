#pragma once
#include "AbstractScene.h"

class End:public AbstractScene
{
private:

public:
	//コンストラクタ
	End();
	//デストラクタ
	~End();

	// 更新処理
	virtual AbstractScene* Update() override;

	// 描画処理
	virtual void Draw() const override;

};

