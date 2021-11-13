#pragma once

#ifndef __MAINGAME_H__
#define __MAINGAME_H__

class CObj;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC		m_DC;

	DWORD	m_dwTime;
	int		m_iFPS;
	TCHAR	m_szFPS[16];
};


#endif // !__MAINGAME_H__
