#include "stdafx.h"
#include "Mouse.h"
#include "BmpMgr.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MOUSE.bmp", L"Mouse");

	m_tInfo.iCX = 60;
	m_tInfo.iCY = 60;
	m_eGroup = GROUPID::UI;

}

int CMouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	ShowCursor(FALSE);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Mouse");

	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		RGB(255, 0, 255));
	//Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMouse::Release()
{
}
