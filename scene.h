#pragma once
#include <list>
#include <typeinfo>
#include <vector>

#include "gameobject.h"
#include "camera.h"

class Scene {
protected:
	//STL���X�g�\��
	std::list<GameObject*> m_Gameobject[3];

public:
	virtual void Init()
	{
			
	}

	virtual void UnInit()
	{
		//�͈�for���[�v
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->UnInit();
				delete gameobject;
			}
			//��ɂȂ������X�g�\���̓��ꕨ���폜����
			m_Gameobject[i].clear();
		}
	}
	
	virtual void Update()
	{
		//�͈�for���[�v
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->Update();
			}

			//�����_��
			m_Gameobject[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}
	}

	virtual void DepthDraw()
	{
		for (int i = 0; i < 2; i++) {
			for (GameObject* gameobject : m_Gameobject[i])
			{
				if (typeid(*gameobject) != typeid(Camera))
				{ 
					gameobject->Draw();
				}
			}
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++) {

		
		/*Z�\�[�g������Ȃ炱��
		�J��������̏��Ԃɂ���Ƃ���
		std::�̕W���R���e�i���g���̂���������
		�N�C�b�N�\�[�g��o�u���\�[�g�Ȃǂ�����
		*/

			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->Draw();
			}
		}
	}

	template <typename T>//�e���v���[�g�֐�
	T* AddGameObject(int layer)
	{
		T* gameobject = new T;
		m_Gameobject[layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>//�e���v���[�g�֐�
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_Gameobject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}


	template <typename T>//�e���v���[�g�֐�
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++) {
			for (GameObject* object : m_Gameobject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};