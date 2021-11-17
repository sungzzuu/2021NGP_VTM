#include "stdafx.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "Player.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"

CMainGame::CMainGame()
	: m_iFPS(0), m_dwTime(GetTickCount())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");
	//CBmpMgr::Get_Instance()->Insert_Bmp(__T(""), __T(""));

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();

}

void CMainGame::Render()
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Back");
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Bmp(L"BackBuffer");
	BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackBuffer);

	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
}

void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();


	CObjMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}

void CMainGame::err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CMainGame::err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int CMainGame::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
