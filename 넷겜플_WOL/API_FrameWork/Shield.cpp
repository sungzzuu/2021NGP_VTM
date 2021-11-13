#include "stdafx.h"
#include "Shield.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
CShield::CShield()
{
}


CShield::~CShield()
{
	Release();
}

void CShield::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/ARMOR.bmp", L"ARMOR");
	m_pFrameKey = L"ARMOR";

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	//m_fAngle = 0.f;
	m_fDis = 70.f;

	m_fSpeed = 2.0f;

	dwTime = GetTickCount();
	m_bScene = true;
	
	m_eGroup = GROUPID::EFFECT;

}

int CShield::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle += m_fSpeed;	
	m_tInfo.fX = m_pTarget->Get_Info().fX + cosf(m_fAngle * PI / 180.f) * m_fDis;
	m_tInfo.fY = m_pTarget->Get_Info().fY - sinf(m_fAngle * PI / 180.f) * m_fDis;
	

	if (dwTime + 5000 < GetTickCount())
	{
		m_bDead = true;
	}
	
	////////////////////각도
	float	fX = 0.f, fY = 0.f, fRad = 0.f;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	fX = m_tInfo.fX - (m_pTarget->Get_Info().fX);
	fY = (m_pTarget->Get_Info().fY) - m_tInfo.fY;
	fRad = acosf(fX / 70);

	float Angle = -(fRad * 180.f / PI);

	if (m_tInfo.fY < (m_pTarget->Get_Info().fY))
	{
		Angle *= -1.f;
	}


	if (Angle >= -30 && Angle < 30)
		m_tFrame.iFrameStart = 3;
	else if (Angle >= 30 && Angle < 60)
		m_tFrame.iFrameStart = 4;
	else if (Angle >= 60 && Angle < 120)
		m_tFrame.iFrameStart = 6;
	else if (Angle >= 120 && Angle < 150)
		m_tFrame.iFrameStart = 7;
	else if (Angle >= 150 || Angle < -150)
		m_tFrame.iFrameStart = 9;
	else if (Angle >= -150 && Angle < -120)
		m_tFrame.iFrameStart = 10;
	else if (Angle >= -120 && Angle < -60)
		m_tFrame.iFrameStart = 0;
	else if (Angle >= -60 && Angle < -30)
		m_tFrame.iFrameStart = 1;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CShield::Late_Update()
{
}

void CShield::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 100, m_tFrame.iFrameScene * 100	//시작좌표
		, 100, 100													//길이
		, RGB(255, 0, 255));

	//cout << m_tFrame.iFrameStart * 100 << endl;
}

void CShield::Release()
{
}
