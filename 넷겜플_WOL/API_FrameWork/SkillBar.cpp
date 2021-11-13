#include "stdafx.h"
#include "SkillBar.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "Player.h"
CSkillBar::CSkillBar()
{
}


CSkillBar::~CSkillBar()
{
	Release();
}

void CSkillBar::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SKILLBAR.bmp", L"UI_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIRE_DRAGON_SKILLBAR.bmp", L"FIRE_DRAGON_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_KRYSTAL_SKILLBAR.bmp", L"ICE_KRYSTAL_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_BLAST_SKILLBAR.bmp", L"ICE_BLAST_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GAIA_ARMOR_SKILLBAR.bmp", L"GAIA_ARMOR_SKILLBAR");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MONEY.bmp", L"UI_MONEY");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/COOLTIME_SHADOW.bmp", L"COOLTIME_SHADOW");

	m_tInfo.iCX = 120;
	m_tInfo.iCY = 60;

	//m_tInfo.fX = 50.f;
	//m_tInfo.fY = 50.f;

	m_iQSpeed = 0.f;
	m_iESpeed = 0.f;
	m_iRSpeed = 0.f;
	m_eGroup = GROUPID::UI;

	m_bRight_Draw = false;
	m_bQ_Draw = false;
	m_bE_Draw = false;
	m_bR_Draw = false;
}

int CSkillBar::Update()
{
	//m_tInfo.fX = m_pTarget->Get_Info().fX - 280.f;
	//m_tInfo.fY = m_pTarget->Get_Info().fY + 230.f;
	m_tInfo.fX = 100.f;
	m_tInfo.fY = 600.f;
	if (true == m_bRight_Draw || true == static_cast<CPlayer*>(m_pTarget)->Get_Right())
	{
		m_bRight_Draw = true;
	}
	if (true == m_bQ_Draw || true == static_cast<CPlayer*>(m_pTarget)->Get_Q())
	{
		m_bQ_Draw = true;
	}
	if (true == m_bE_Draw || true == static_cast<CPlayer*>(m_pTarget)->Get_E())
	{
		m_bE_Draw = true;
	}
	if (true == m_bR_Draw || true == static_cast<CPlayer*>(m_pTarget)->Get_R())
	{
		m_bR_Draw = true;
	}

	return OBJ_NOEVENT;
}

void CSkillBar::Late_Update()
{
}

void CSkillBar::Render(HDC _DC)
{
	Update_Rect();

	//int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	int iScrollX = 0.f;
	int iScrollY = 0.f;

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_SKILLBAR");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		350, 80,
		hMemDC,
		0, 0,
		640, 160,
		RGB(255, 0, 255));


	////////////////////////////°ñµå
	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_MONEY");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 400, m_tRect.top + iScrollY + 30,
		25, 25,
		hMemDC,
		0, 0,
		31, 31,
		RGB(255, 0, 255));

	//HFONT myFont     = CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	//HFONT oldFont    = (HFONT)SelectObject(_DC, myFont);
	TCHAR szBuff[32] = L"";
	swprintf_s(szBuff, L" %d", m_pTarget->Get_Gold());
	SetBkMode(_DC, 1); //Åõ¸í
	SetTextColor(_DC, RGB(255,255,0));
	TextOut(_DC, m_tRect.left + iScrollX + 430, m_tRect.top + iScrollY + 35, szBuff, lstrlen(szBuff));

	if (true == static_cast<CPlayer*>(m_pTarget)->Get_Right())
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"FIRE_DRAGON_SKILLBAR");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 88, m_tRect.top + iScrollY + 30,
			30, 30,
			hMemDC,
			0, 0,
			54, 54,
			RGB(255, 0, 255));

		swprintf_s(szBuff, L" %d", static_cast<CPlayer*>(m_pTarget)->Get_Right_Remain());
		SetTextColor(_DC, RGB(255, 255, 255));
		TextOut(_DC, m_tRect.left + iScrollX + 95, m_tRect.top + iScrollY + 30, szBuff, lstrlen(szBuff));
	}
	//DeleteObject(myFont);


	if (true == m_bQ_Draw)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ICE_KRYSTAL_SKILLBAR");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 130, m_tRect.top + iScrollY + 30,  //+42
			30, 30,
			hMemDC,
			0, 0,
			54, 54,
			RGB(255, 0, 255));

		//ÄðÅ¸ÀÓ
		if (true == static_cast<CPlayer*>(m_pTarget)->Get_QCool())
		{
			m_iQSpeed += 0.015f;
			hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"COOLTIME_SHADOW");
			GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 130, m_tRect.top + iScrollY + 30,  //+42
				30, 30- m_iQSpeed *5,
				hMemDC,
				0, 0,
				54, 54,
				RGB(255, 0, 255));
		}
		else
		{
			m_iQSpeed = 0.f;
		}



	}


	if (true == m_bE_Draw)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ICE_BLAST_SKILLBAR");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 172, m_tRect.top + iScrollY + 30,
			30, 30,
			hMemDC,
			0, 0,
			54, 54,

			RGB(255, 0, 255));
		//ÄðÅ¸ÀÓ
		if (true == static_cast<CPlayer*>(m_pTarget)->Get_ECool())
		{
			m_iESpeed += 0.015f;
			hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"COOLTIME_SHADOW");
			GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 172, m_tRect.top + iScrollY + 30,  //+42
				30, 30 - m_iESpeed * 10,
				hMemDC,
				0, 0,
				54, 54,
				RGB(255, 0, 255));
		}
		else
		{
			m_iESpeed = 0.f;
		}
	}


	if (true == m_bR_Draw)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"GAIA_ARMOR_SKILLBAR");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 212, m_tRect.top + iScrollY + 30,
			30, 30,
			hMemDC,
			0, 0,
			54, 54,
			RGB(255, 0, 255));

		//ÄðÅ¸ÀÓ
		if (true == static_cast<CPlayer*>(m_pTarget)->Get_RCool())
		{
			m_iRSpeed += 0.015f;
			hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"COOLTIME_SHADOW");
			GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 212, m_tRect.top + iScrollY + 30,  //+42
				30, 30 - m_iRSpeed * 5,
				hMemDC,
				0, 0,
				54, 54,
				RGB(255, 0, 255));
		}
		else
		{
			m_iRSpeed = 0.f;
		}
	}

}

void CSkillBar::Release()
{
}
