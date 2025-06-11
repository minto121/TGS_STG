#include"DxLib.h"
#include"SceneManager.h"
#include"FPS.h"

//AbstractScene* SceneManager::Update()
//{
//	AbstractScene* NextScene;
//	try
//	{
//		NextScene = mScene->Update();
//	}
//	catch (const char* err)
//	{
//		FILE* fp = NULL;
//
//		DATEDATA data;
//
//		GetDateTime(&data);
//
//		//�t�@�C���I�[�v��
//		fopen_s(&fp, "ErrLog.txt", "a");
//		//�G���[�f�[�^�̏�������
//		fprintf_s(fp, "%02d�N %02d�� %02d�� %02d�� %02d�� %02d�b : %s������܂���B\n", data.Year, data.Mon, data.Day, data.Hour, data.Min, data.Sec, err);
//
//		return nullptr;
//	}
//
//	if (NextScene != mScene)
//	{
//		delete mScene;
//		mScene = NextScene;
//	}
//	return mScene;
//}
//
//void SceneManager::Draw() const {
//	//���̃V�[���̕`��
//	mScene->Draw();
//}

AbstractScene* SceneManager::Update() {
    AbstractScene* NextScene = nullptr;

    if (!mScene) {
        throw "Scene が初期化されていません";
    }

    try {
        NextScene = mScene->Update();
    }
    catch (const char* err) {
        FILE* fp = nullptr;
        DATEDATA data;
        GetDateTime(&data);

        if (fopen_s(&fp, "ErrLog.txt", "a") == 0 && fp) {
            fprintf_s(fp, "%04d/%02d/%02d %02d:%02d:%02d : %s\n",
                data.Year, data.Mon, data.Day, data.Hour, data.Min, data.Sec,
                err ? err : "不明なエラー");
            fclose(fp);
        }
        return nullptr;
    }

    if (NextScene && NextScene != mScene) {
        delete mScene;
        mScene = NextScene;
    }


    return mScene;
}

void SceneManager::Draw() const {
    	//���̃V�[���̕`��
    	mScene->Draw();
}

