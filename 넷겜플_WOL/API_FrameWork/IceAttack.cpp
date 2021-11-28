#include "stdafx.h"
#include "IceAttack.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
CIceAttack::CIceAttack()
{
}


CIceAttack::~CIceAttack()
{
}

void CIceAttack::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ice_ATTACK.bmp", L"Ice_ATTACK");
	m_pFrameKey = L"Ice_ATTACK";

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_iImageCX = 100;
	m_iImageCY = 100;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	//m_fAngle = 0.f;
	m_fDis = 20.f;

	m_fSpeed = 1.f;

	m_bScene = true;

	dwTime = GetTickCount();

	m_eGroup = GROUPID::EFFECT;
	m_iAttackName = 2;

}

int CIceAttack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle += m_fSpeed;

	m_tInfo.fX = m_pTarget->Get_Info().fX + cosf(m_fAngle * PI / 180.f) * m_fDis;
	m_tInfo.fY = m_pTarget->Get_Info().fY - sinf(m_fAngle * PI / 180.f) * m_fDis;

	if (false == m_bUltimate)
	{
		if (dwTime + 2500 > GetTickCount())
		{
			m_fDis += 1.f;
			m_fSpeed += 0.05f;
		}
		if (dwTime + 2500 < GetTickCount())
		{
			m_fDis -= 1.f;
			m_fSpeed -= 0.05f;
		}
		if (dwTime + 5000 < GetTickCount())
		{
			m_bDead = true;
		}
	}
	else
	{
		if (dwTime + 5000 > GetTickCount())
		{
			m_fDis += 0.5f;
			m_fSpeed += 0.05f;
		}
		if (dwTime + 5000 < GetTickCount())
		{
			m_fDis -= 0.5f;
			m_fSpeed -= 0.05f;
		}
		if (dwTime + 9000 < GetTickCount())
		{
			m_bDead = true;
		}
	}

	Scene_Change();
	Frame_Move();

	Update_Rect();
	return OBJ_NOEVENT;
}

void CIceAttack::Late_Update()
{
}

void CIceAttack::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
	
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX + Image_Dif_X, m_tRect.top + iScrollY + Image_Dif_Y
		, CHAR_CX, CHAR_CY
		, hMemDC
		, m_tFrame.iFrameStart * 200, m_tFrame.iFrameScene * 200	//시작좌표
		, 200, 200													//길이
		, RGB(200, 230, 250)); 
	
}

void CIceAttack::Release()
{
}	

void CIceAttack::Scene_Change()
{
	if (true == m_bScene)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 17;
		m_tFrame.iFrameScene = 0;
		m_tFrame.dwFrameSpeed = 10;
		m_tFrame.dwFrameTime = GetTickCount();
		m_bScene = false;
	}
}
