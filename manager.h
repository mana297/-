#pragma once
#include "scene.h"

class Manager
{
private:
	//�C���X�^���X������K�v�Ƃ��Ȃ�
	static Scene* m_Scene;

	//���̃V�[����ۑ�
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