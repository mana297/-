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

	//キーを押している間判定
	static bool GetKeyPress(BYTE KeyCode);
	//キーを押した瞬間を判定
	static bool GetKeyTrigger(BYTE KeyCode);
	static bool GetKeyRelease(BYTE keyCode);

};
