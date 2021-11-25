#include "stdafx.h"
#include "UI.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

#include "DataMgr.h"

CUI::CUI()
{
}


CUI::~CUI()
{
	Release();
}

void CUI::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PLAYERBAR.bmp", L"UI_PLAYERBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_HPBAR.bmp", L"UI_HPBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MANABAR.bmp", L"UI_MANABAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/PLAYER_HPBAR.bmp", L"PLAYER_HPBAR");


	m_tInfo.iCX = 120;
	m_tInfo.iCY = 60;

	//m_tInfo.fX = 50.f;
	//m_tInfo.fY = 50.f;

	m_fSpeed = 5.f;

	m_eGroup = GROUPID::UI;
}

int CUI::Update()
{
	//m_tInfo.fX = m_pTarget->Get_Info().fX - 300.f;
	//m_tInfo.fY = m_pTarget->Get_Info().fY - 250.f;
	m_tInfo.fX = 100.f;
	m_tInfo.fY = 100.f;
	return OBJ_NOEVENT;
}

void CUI::Late_Update()
{
}

void CUI::Render(HDC _DC)
{
	Update_Rect();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_PLAYERBAR");

	//int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	int iScrollX = 0.f;
	int iScrollY = 0.f;
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		200, 60,
		hMemDC,
		0, 0,
		328, 80,
		RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"PLAYER_HPBAR");
	for (int i = 0; i < 4; ++i)
	{
		STORE_DATA tStoreData = CDataMgr::Get_Instance()->m_tStoreData;
		if (i != tStoreData.iClientIndex)
		{
			RECT	tRect;
			tRect.left = (LONG)(tStoreData.tPlayersPos[i].fX - (m_tInfo.iCX >> 1));
			tRect.top = (LONG)(tStoreData.tPlayersPos[i].fY - (m_tInfo.iCY >> 1));

			GdiTransparentBlt(_DC
				, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 70
				, 260, 41
				, hMemDC
				, 0, 0
				, 260, 41
				, RGB(255, 0, 255));
		}
	}

	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_HPBAR");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 45, m_tRect.top + iScrollY + 10,
		150 - (150 - m_pTarget->Get_Hp()), 22,
		hMemDC,
		0, 0,
		244, 32,
		RGB(255, 0, 255));

	for (int i = 0; i < 4; ++i)
	{
		STORE_DATA tStoreData = CDataMgr::Get_Instance()->m_tStoreData;
		if (i != tStoreData.iClientIndex)
		{
			RECT	tRect;
			tRect.left = (LONG)(tStoreData.tPlayersPos[i].fX - (m_tInfo.iCX >> 1));
			tRect.top = (LONG)(tStoreData.tPlayersPos[i].fY - (m_tInfo.iCY >> 1));

			GdiTransparentBlt(_DC
				, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 70
				, 150 - (150 - tStoreData.iHp[i]), 10
				, hMemDC
				, 0,0
				, 244, 32
				, RGB(255, 0, 255));
		}
	}


	//HFONT myFont = CreateFont(20, 0, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	//HFONT oldFont = (HFONT)SelectObject(_DC, myFont);
	//TCHAR	szBuff[32] = L"";
	//swprintf_s(szBuff, L"%d / 150", m_pTarget->Get_Hp());
	//SetBkMode(_DC, 1); //����
	//SetTextColor(_DC, RGB(0, 255, 0));
	//TextOut(_DC, m_tRect.left + iScrollX + 75, m_tRect.top + iScrollY + 10, szBuff, lstrlen(szBuff));
	//DeleteObject(myFont);

	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_MANABAR");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 45, m_tRect.top + iScrollY + 38,
		m_pTarget->Get_Mana(), 12,  //x ���� 120
		hMemDC,
		0, 0,
		192, 16,
		RGB(255, 0, 255));
}

void CUI::Release()
{
}


