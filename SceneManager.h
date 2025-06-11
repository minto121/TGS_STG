#pragma once
#pragma once
#include"AbstractScene.h"

class SceneManager :
    public AbstractScene
{
private:
    AbstractScene* mScene;
public:

    //?R???X?g???N?^
    SceneManager(AbstractScene* scene) :mScene(scene) {};

    //?f?X?g???N?^
    ~SceneManager()
    {
        delete mScene;

    }

    //?`???O??X?V????s
    AbstractScene* Update() override;

    //?`??????ç·??????
    void Draw() const override;
};



