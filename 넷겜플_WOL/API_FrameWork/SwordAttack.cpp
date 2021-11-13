#include "stdafx.h"
#include "SwordAttack.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CSwordAttack::CSwordAttack()
{
}


CSwordAttack::~CSwordAttack()
{
	Release();
}

void CSwordAttack::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_iImageCX = 100;
	m_iImageCY = 100;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	dwTime = GetTickCount();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SWORDMAN_ATTACK.bmp", L"SWORDMAN_ATTACK");

	m_pFrameKey = L"SWORDMAN_ATTACK";

	m_bScene = true;
	//m_eDir = UP;

	m_eGroup = GROUPID::EFFECT;

}

int CSwordAttack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (dwTime + 230 < GetTickCount())
		m_bDead = true;

	Scene_Change();
	Frame_Move();

	Update_Rect();

	return OBJ_NOEVENT;
}

void CSwordAttack::Late_Update()
{
}

void CSwordAttack::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX + Image_Dif_X, m_tRect.top + iScrollY + Image_Dif_Y
		, CHAR_CX, CHAR_CY
		, hMemDC
		, m_tFrame.iFrameStart * 200, m_tFrame.iFrameScene * 200	//시작좌표
		, 200, 200													//길이
		, RGB(255, 0, 255));

}

void CSwordAttack::Release()
{
}

void CSwordAttack::Scene_Change()
{
	if (true == m_bScene)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 3;
		//m_tFrame.iFrameScene = 4;
		m_tFrame.dwFrameSpeed = 60;
		m_tFrame.dwFrameTime = GetTickCount();
		switch (m_eDir)
		{
		case CObj::LEFT:
			m_tFrame.iFrameScene = 0;
			break;
		case CObj::RIGHT:
			m_tFrame.iFrameScene = 1;
			break;
		case CObj::UP:
			m_tFrame.iFrameScene = 2;
			break;
		case CObj::DOWN:
			m_tFrame.iFrameScene = 4;
			break;
		}

		m_bScene = false;
	}
}
