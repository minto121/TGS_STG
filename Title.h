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

	int select;
	int interval;
	int button_wait;

	int Title_BGM;
	int Select_SE;
	int CursorMove_SE;
	int Title_Img;

	int cursor;
	int cursor_img;

public:
	Title();
	virtual ~Title();

	virtual AbstractScene* Update() override;
	virtual void Draw() const override;
};