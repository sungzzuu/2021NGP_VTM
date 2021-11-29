#include "stdafx.h"
#include "BlastAttack.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CBlastAttack::CBlastAttack()
{
}


CBlastAttack::~CBlastAttack()
{
	Release();

}

void CBlastAttack::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_iImageCX = 100;
	m_iImageCY = 100;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	dwTime = GetTickCount();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/ICE_BLAST.bmp", L"ICE_BLAST");

	m_pFrameKey = L"ICE_BLAST";

	m_bScene = true;
	//m_eDir = UP;

	m_eGroup = GROUPID::EFFECT;
	m_iAttackName = 3;

}

int CBlastAttack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (dwTime + 500 < GetTickCount()) {
		m_bDead = true;
	}

	Scene_Change();
	Frame_Move();

	Update_Rect();

	return OBJ_NOEVENT;
}

void CBlastAttack::Late_Update()
{
}

void CBlastAttack::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX + Image_Dif_X, m_tRect.top + iScrollY + Image_Dif_Y
		, CHAR_CX, CHAR_CY
		, hMemDC
		, m_tFrame.iFrameStart * 200, m_tFrame.iFrameScene * 250	//시작좌표
		, 200, 250													//길이
		, RGB(255, 0, 255));
}

void CBlastAttack::Release()
{
}

void CBlastAttack::Scene_Change()
{
	if (true == m_bScene)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 100;
		m_tFrame.dwFrameTime = GetTickCount();
		m_bScene = false;
	}
}
