#pragma once
#include "AbstractScene.h"

class Title :public AbstractScene
{
private:
	enum class TITLE_MENU {
		START,
		HELP,
		END
	};

	TITLE_MENU Menu_Number1;
	TITLE_MENU Menu_Number2;

	int TitleBGM;
	int TitleSE;
	int UI_Img[6];
	int LifeImg;
	int FireImg;
	int WaterImg;
	int LeafImg;

public:
	Title();
	virtual ~Title();

	virtual AbstractScene* Update() override;
	virtual void Draw() const override;
};