#pragma once
#include <list>
#include <typeinfo>
#include <vector>

#include "gameobject.h"
#include "camera.h"

class Scene {
protected:
	//STLリスト構造
	std::list<GameObject*> m_Gameobject[3];

public:
	virtual void Init()
	{
			
	}

	virtual void UnInit()
	{
		//範囲forループ
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->UnInit();
				delete gameobject;
			}
			//空になったリスト構造の入れ物を削除する
			m_Gameobject[i].clear();
		}
	}
	
	virtual void Update()
	{
		//範囲forループ
		for (int i = 0; i < 3; i++) {
			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->Update();
			}

			//ラムダ式
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

		
		/*Zソートを入れるならここ
		カメラからの順番にするといい
		std::の標準コンテナを使うのがおすすめ
		クイックソートやバブルソートなどがある
		*/

			for (GameObject* gameobject : m_Gameobject[i])
			{
				gameobject->Draw();
			}
		}
	}

	template <typename T>//テンプレート関数
	T* AddGameObject(int layer)
	{
		T* gameobject = new T;
		m_Gameobject[layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>//テンプレート関数
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


	template <typename T>//テンプレート関数
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