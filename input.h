#pragma once


class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void UnInit();
	static void Update();

	//�L�[�������Ă���Ԕ���
	static bool GetKeyPress(BYTE KeyCode);
	//�L�[���������u�Ԃ𔻒�
	static bool GetKeyTrigger(BYTE KeyCode);
	static bool GetKeyRelease(BYTE keyCode);

};
