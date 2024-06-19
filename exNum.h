#pragma once
#include <iostream>
#include <algorithm>


class ExNum
{
private:
	float m_Max; // 最大値
	float m_Min; // 最小値
	float m_Current; // 現在値

public:
	// コンストラクタで初期化
	ExNum(float max = 0, float min = 0, float current = 0)
		:m_Max(max), m_Min(min), m_Current(current) {}

	// 最大値、最小値、現在値のゲッター
	float GetMax() { return m_Max; }
	float GetMin() { return m_Min; }
	float Get() { return m_Current; }

	// 最大値をセットする時は最小値より小さくならないようにする。
	// また現在値が最大値を超えているなら、最大値と同じにする。
	void SetMax(float value);
	
	// 最小値をセットする時は最大値より大きくならないようにする。
	// あと、現在値が最小値未満になるなら、最小値と同じにする。
	void SetMin(float value);

	// 現在値をセットする時は最小値以上、最大値以下になるようにする。
	void Set(float value);
	
	// 現在値を追加、戻り値で最終的な値を返すと便利かな？
	float Add(float value);

	// 最大値を追加
	float AddMax(float value);

	// 最小値を追加
	float AddMin(float value);

	// HPが空です。
	bool isEmpty() {
		return (Get() <= GetMin());
	}

	// HPが満タンです
	bool isFull() {
		return (GetMax() <= Get()); 
	}

	// 割合を取得
	float rate();
};