#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CMyButton::CMyButton()
	
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{

	m_eGroup = GROUPID::UI;

}

int CMyButton::Update()
{
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (400 > pt.y)
		m_pFrameKey = L"MAIN_MENU";
	else if(500 < pt.y)
		m_pFrameKey = L"QUIT_MENU";
	else
		m_pFrameKey = L"OPTION_MENU";


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (!lstrcmp(L"MAIN_MENU", m_pFrameKey))
		{
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
			return;
		}
		//else if (!lstrcmp(L"OPTION_MENU", m_pFrameKey))
		//{
		//	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_EDIT);
		//	return;
		//}
		//else if (!lstrcmp(L"QUIT_MENU", m_pFrameKey))
		//{
		//	DestroyWindow(g_hWnd);
		//	return;
		//}
	}
	

}

void CMyButton::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	
	GdiTransparentBlt(_DC, 0, 0
		, WINCX, WINCY
		, hMemDC
		, 0, 0
		, 1920, 1080
		, RGB(255, 255, 255));
}

void CMyButton::Release()
{
}
