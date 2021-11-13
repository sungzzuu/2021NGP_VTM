#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/READY_MENU.bmp", L"READY_MENU");

}

void CLogo::Update()
{
	
}

void CLogo::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
		return;
	}
}

void CLogo::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"READY_MENU");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	GdiTransparentBlt(_DC, 0, 0,
		WINCX, WINCY,
		hMemDC,
		0, 0,
		1920, 1080,
		NULL);
}

void CLogo::Release()
{
}
