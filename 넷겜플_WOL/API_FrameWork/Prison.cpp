#include "stdafx.h"
#include "Prison.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CPrison::CPrison()
	
{
}


CPrison::~CPrison()
{
	Release();

}

void CPrison::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/PRISON_HOR.bmp", L"PRISON_HOR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/PRISON_VER.bmp", L"PRISON_VER");

	m_eGroup = GROUPID::OBJECT;

}

int CPrison::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (false == m_bBar)
	{
		m_tInfo.iCX = 140;
		m_tInfo.iCY = 80;
	}
	else
	{
		m_tInfo.iCX = 10;
		m_tInfo.iCY = 170;
	}

	Update_Rect();

	return OBJ_NOEVENT;
}

void CPrison::Late_Update()
{
}

void CPrison::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (false == m_bBar)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"PRISON_HOR");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
			146, 141,
			hMemDC,
			0, 0,
			292, 283,
			RGB(255, 0, 255));
	}

	else
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"PRISON_VER");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
			10, 170,
			hMemDC,
			0, 0,
			23, 427,
			RGB(255, 0, 255));
		//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
	}

}

void CPrison::Release()
{
}
