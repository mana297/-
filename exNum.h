#pragma once
#include <iostream>
#include <algorithm>


class ExNum
{
private:
	float m_Max; // �ő�l
	float m_Min; // �ŏ��l
	float m_Current; // ���ݒl

public:
	// �R���X�g���N�^�ŏ�����
	ExNum(float max = 0, float min = 0, float current = 0)
		:m_Max(max), m_Min(min), m_Current(current) {}

	// �ő�l�A�ŏ��l�A���ݒl�̃Q�b�^�[
	float GetMax() { return m_Max; }
	float GetMin() { return m_Min; }
	float Get() { return m_Current; }

	// �ő�l���Z�b�g���鎞�͍ŏ��l��菬�����Ȃ�Ȃ��悤�ɂ���B
	// �܂����ݒl���ő�l�𒴂��Ă���Ȃ�A�ő�l�Ɠ����ɂ���B
	void SetMax(float value);
	
	// �ŏ��l���Z�b�g���鎞�͍ő�l���傫���Ȃ�Ȃ��悤�ɂ���B
	// ���ƁA���ݒl���ŏ��l�����ɂȂ�Ȃ�A�ŏ��l�Ɠ����ɂ���B
	void SetMin(float value);

	// ���ݒl���Z�b�g���鎞�͍ŏ��l�ȏ�A�ő�l�ȉ��ɂȂ�悤�ɂ���B
	void Set(float value);
	
	// ���ݒl��ǉ��A�߂�l�ōŏI�I�Ȓl��Ԃ��ƕ֗����ȁH
	float Add(float value);

	// �ő�l��ǉ�
	float AddMax(float value);

	// �ŏ��l��ǉ�
	float AddMin(float value);

	// HP����ł��B
	bool isEmpty() {
		return (Get() <= GetMin());
	}

	// HP�����^���ł�
	bool isFull() {
		return (GetMax() <= Get()); 
	}

	// �������擾
	float rate();
};