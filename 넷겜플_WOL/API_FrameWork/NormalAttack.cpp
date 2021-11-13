#include "stdafx.h"
#include "NormalAttack.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
CNormalAttack::CNormalAttack()
{
}


CNormalAttack::~CNormalAttack()
{
	Release();
}

void CNormalAttack::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_iImageCX = 100;
	m_iImageCY = 100;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	dwTime = GetTickCount();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Normal_ATTACK.bmp", L"Normal_ATTACK");

	m_pFrameKey = L"Normal_ATTACK";

	m_bScene = true;
	//m_eDir = UP;

	m_eGroup = GROUPID::EFFECT;


}

int CNormalAttack::Update()
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

void CNormalAttack::Late_Update()
{

}

void CNormalAttack::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX + Image_Dif_X, m_tRect.top + iScrollY + Image_Dif_Y
		, CHAR_CX, CHAR_CY
		, hMemDC
		, m_tFrame.iFrameStart * 200, m_tFrame.iFrameScene * 200	//������ǥ
		, 200, 200													//����
		, RGB(255, 0, 255));

}

void CNormalAttack::Release()
{
}

void CNormalAttack::Scene_Change()
{
	if (true == m_bScene)
	{
		m_tFrame.iFrameStart = 0 + m_iFrameX;
		m_tFrame.iFrameEnd = 3 + m_iFrameX;
		//m_tFrame.iFrameScene = 4;
		m_tFrame.dwFrameSpeed = 60;
		m_tFrame.dwFrameTime = GetTickCount();
		switch (m_eDir)
		{
		case CObj::LEFT:
			m_tFrame.iFrameScene = 4;
			break;
		case CObj::RIGHT:
			m_tFrame.iFrameScene = 0;
			break;
		case CObj::UP:
			m_tFrame.iFrameScene = 2;
			break;
		case CObj::DOWN:
			m_tFrame.iFrameScene = 6;
			break;
		case CObj::LEFT_UP:
			m_tFrame.iFrameScene = 3;
			break;
		case CObj::LEFT_DOWN:
			m_tFrame.iFrameScene = 5;
			break;
		case CObj::RIGHT_UP:
			m_tFrame.iFrameScene = 1;
			break;
		case CObj::RIGHT_DOWN:
			m_tFrame.iFrameScene = 7;
			break;
		}

		m_bScene = false;
	}
	
}
