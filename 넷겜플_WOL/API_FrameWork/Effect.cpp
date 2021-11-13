#include "stdafx.h"
#include "Effect.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CEffect::CEffect()
{
}


CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/HITEFFECT.bmp", L"HITEFFECT");
	m_tInfo.iCX = 40.f;
	m_tInfo.iCY = 40.f;
	m_eGroup = GROUPID::EFFECT;
	dwTime = GetTickCount();
	m_bScene = true;
}

int CEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	Update_Rect();

	if (dwTime + 300 < GetTickCount())
		m_bDead = true;

	if (true == m_bScene)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 80;
		m_tFrame.dwFrameTime = GetTickCount();
		m_bScene = false;
	}

	Frame_Move();
	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
}

void CEffect::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"HITEFFECT");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		m_tFrame.iFrameStart * 230, 0,
		230, 230,
		RGB(255, 250, 255));

}

void CEffect::Release()
{
}
