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

AbstractScene* SceneManager::Update()
{
	AbstractScene* NextScene;
	try
	{
		if (!mScene) {
			throw "mScene is nullptr";
		}
		NextScene = mScene->Update();
	}
	catch (const char* err)
	{
		FILE* fp = NULL;

		DATEDATA data;

		GetDateTime(&data);

		//�t�@�C���I�[�v��
		fopen_s(&fp, "ErrLog.txt", "a");
		//�G���[�f�[�^�̏�������
		fprintf_s(fp, "%02d�N %02d�� %02d�� %02d�� %02d�� %02d�b : %s������܂���B\n", data.Year, data.Mon, data.Day, data.Hour, data.Min, data.Sec, err);

		return nullptr;
	}

	if (NextScene && NextScene != mScene) {
		//delete mScene;
		mScene = nullptr;
		mScene = NextScene;
		mSceneChanged = true; // フラグを立てて通知
	}
	else
	{
		mSceneChanged = false;
	}

	return mScene;
}


void SceneManager::Draw() const {
	//���̃V�[���̕`��
	mScene->Draw();
}

