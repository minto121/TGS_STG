#pragma once
#include "AbstractScene.h"

class Title :public AbstractScene
{
private:

	int TitleBGM;
	int TitleSE;

public:
	Title();
	virtual ~Title();

	virtual AbstractScene* Update() override;
	virtual void Draw() const override;
};