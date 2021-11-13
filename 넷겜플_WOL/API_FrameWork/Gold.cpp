#include "stdafx.h"
#include "Gold.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CGold::CGold()
{
}


CGold::~CGold()
{
	Release();
}

void CGold::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/MONEY.bmp", L"MONEY");

	m_tInfo.iCX = 15;
	m_tInfo.iCY = 15;

	m_eGroup = GROUPID::OBJECT;
}

int CGold::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CGold::Late_Update()
{
}

void CGold::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"MONEY");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		15, 15,
		hMemDC,
		0, 0,
		24, 23,
		RGB(255, 0, 255));
}

void CGold::Release()
{
}
