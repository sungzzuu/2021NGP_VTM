#include "stdafx.h"
#include "FireAttack.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
CFireAttack::CFireAttack()
	: m_fRotAngle(0.f), m_fRotDis(0.f), m_fRotSpeed(0.f), m_bStart(true)
{
	ZeroMemory(&m_tRotPos, sizeof(m_tRotPos));
}


CFireAttack::~CFireAttack()
{
}

void CFireAttack::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/SKILL_FIREDRAGON.bmp", L"SKILL_FIREDRAGON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/FIRE_PARTICLE.bmp", L"FIRE_PARTICLE");


	m_tInfo.iCX = 20;
	m_tInfo.iCY = 20;

	m_iImageCX = 70;
	m_iImageCY = 70;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	m_iParti_ImageCX = 40;
	m_iParti_ImageCY = 40;
	Parti_Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iParti_ImageCX >> 1);
	Parti_Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iParti_ImageCY >> 1);


	m_fRotAngle = 0.f;
	m_fRotDis = 20.f;
	m_fRotSpeed = 15.f;

	m_fSpeed = 10.f;

	m_pFrameKey = L"SKILL_FIREDRAGON";
	m_pParticleKey = L"FIRE_PARTICLE";
	m_bScene = true;

	m_eGroup = GROUPID::EFFECT;
	m_bFire = true;

	m_dDead_Time = GetTickCount();

	m_eAttackName = ATTACK::FIRE;

}

int CFireAttack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pParti_Pos.x = (LONG)m_tInfo.fX;
		m_pParti_Pos.y = (LONG)m_tInfo.fY;
		m_bStart = false;
	
	}
	m_pParti_Pos.x = (LONG)m_tInfo.fX;
	m_pParti_Pos.y = (LONG)m_tInfo.fY;

	m_tInfo.fX += (LONG)(cosf(m_fAngle * PI / 180.f) * m_fSpeed);
	m_tInfo.fY -= (LONG)(sinf(m_fAngle * PI / 180.f) * m_fSpeed);

	//m_pParti_Pos.x += (LONG)(cosf(m_fAngle * PI / 180.f) * 8.f);
	//m_pParti_Pos.y -= (LONG)(sinf(m_fAngle * PI / 180.f) * 8.f);

	m_tParti_Rect.left = (LONG)(m_pParti_Pos.x - (m_tInfo.iCX >> 1));
	m_tParti_Rect.top = (LONG)(m_pParti_Pos.y - (m_tInfo.iCY >> 1));

	//cout << m_tInfo.fX << "    " << m_pParti_Pos.x << endl;
	Scene_Change();
	//Frame_Move();

	Update_Rect();

	return OBJ_NOEVENT;
}

void CFireAttack::Late_Update()
{
	if (m_dDead_Time + 2000 < GetTickCount())
		m_bDead = true;

}

void CFireAttack::Render(HDC _DC)
{
	//19*2

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX + Image_Dif_X, m_tRect.top + iScrollY + Image_Dif_Y
		, m_iImageCX, m_iImageCY
		, hMemDC
		, m_tFrame.iFrameStart * 180, m_tFrame.iFrameScene * 180	//시작좌표
		, 180, 180													//길이
		, RGB(255, 255, 255));

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	//70*70
	//HDC hMemDC1 = CBmpMgr::Get_Instance()->Find_Bmp(m_pParticleKey);

	//GdiTransparentBlt(_DC
	//	, m_tParti_Rect.left + iScrollX + Parti_Image_Dif_X, m_tParti_Rect.top + iScrollY + Parti_Image_Dif_Y
	//	, 40, 40
	//	, hMemDC1
	//	, 0, 0													//시작좌표
	//	, 70, 70													//길이
	//	, RGB(255, 0, 255));
}

void CFireAttack::Release()
{
}

void CFireAttack::Scene_Change()
{
	if (true == m_bScene)
	{
		//m_tFrame.iFrameStart = 0;
		//m_tFrame.iFrameEnd = 18;
		//m_tFrame.iFrameScene = 0;
		//m_tFrame.dwFrameSpeed = 100;
		//m_tFrame.dwFrameTime = GetTickCount();

		if (m_fAngle > -22.5 && m_fAngle < 22.5)
		{
			m_tFrame.iFrameStart = 9;
			m_tFrame.iFrameScene = 0;
		}
		else if (m_fAngle > 22.5 && m_fAngle < 67.5)
		{
			m_tFrame.iFrameStart = 12;
			m_tFrame.iFrameScene = 0;
		}
		else if (m_fAngle > 67.5 && m_fAngle < 112.5)
		{
			m_tFrame.iFrameStart = 15;
			m_tFrame.iFrameScene = 0;
		}
		else if (m_fAngle > 112.5 && m_fAngle < 157.5)
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameScene = 1;
		}
		else if (m_fAngle > 157.5 || m_fAngle < -157.5)
		{
			m_tFrame.iFrameStart = 9;
			m_tFrame.iFrameScene = 1;
		}
		else if (m_fAngle > -157.5 && m_fAngle < -112.5)
		{
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameScene = 0;
		}
		else if (m_fAngle > -112.5 && m_fAngle < -67.5)
		{
			m_tFrame.iFrameStart = 3;
			m_tFrame.iFrameScene = 0;
		}
		else if (m_fAngle > -67.5 && m_fAngle < -22.5)
		{
			m_tFrame.iFrameStart = 6;
			m_tFrame.iFrameScene = 0;
		}

		m_bScene = false;
	}
}
