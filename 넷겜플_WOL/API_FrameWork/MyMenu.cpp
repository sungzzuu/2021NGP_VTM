#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "MyButton.h"
#include "ObjMgr.h"


CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/MAIN_MENU.bmp", L"MAIN_MENU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/OPTION_MENU.bmp", L"OPTION_MENU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/QUIT_MENU.bmp", L"QUIT_MENU");


	CObj* pObj = CAbstractFactory<CMyButton>::Create();
	pObj->Set_FrameKey(L"MAIN_MENU");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

}

void CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render(HDC _DC)
{
	CObjMgr::Get_Instance()->Render(_DC);
}

void CMyMenu::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
}
