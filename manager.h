#pragma once
#include "scene.h"

class Manager
{
private:
	//インスタンス生成を必要としない
	static Scene* m_Scene;

	//次のシーンを保存
	static Scene* m_NextScene;

public:
	static void Init();
	static void UnInit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}

};