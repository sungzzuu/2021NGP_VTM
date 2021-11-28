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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BAR.bmp", L"UI_BAR");


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
	PAINTSTRUCT ps;
	TCHAR lpOut[10];
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		200, 60,
		hMemDC,
		0, 0,
		328, 80,
		RGB(255, 0, 255));

	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_BAR");		//체력바 틀
	for (int i = 0; i < 4; ++i)
	{
		STORE_DATA tStoreData = CDataMgr::Get_Instance()->m_tStoreData;
		if (i != tStoreData.iClientIndex && tStoreData.start == true)
		{
			RECT	tRect;
			tRect.left = (LONG)(tStoreData.tPlayersPos[i].fX - (m_tInfo.iCX >> 1));
			tRect.top = (LONG)(tStoreData.tPlayersPos[i].fY - (m_tInfo.iCY >> 1));

			GdiTransparentBlt(_DC
				, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 70
				, 106, 15
				, hMemDC
				, 0, 0
				, 260, 41
				, RGB(255, 0, 255));


		}
	}	// 체력바 틀 사이즈 맞춤

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
			if (tStoreData.start == true)	//체력바 그리기
			{
				GdiTransparentBlt(_DC
					, tRect.left + Image_Dif_X + 3, tRect.top + Image_Dif_Y - 66
					, tStoreData.iHp[i] - 50, 10
					, hMemDC
					, 0, 0
					, 244, 32
					, RGB(255, 0, 255));
			}
			if (tStoreData.team[i] == TEAMNUM::TEAM1)
			{
				BeginPaint(g_hWnd, &ps);
				SetTextColor(_DC, RGB(0, 0, 255));
				wsprintf(lpOut, TEXT("%d"), i+ 1);	//캐릭터 번호부여
				TextOut(_DC, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 60, lpOut, lstrlen(lpOut));
				EndPaint(g_hWnd, &ps);
			}
			else
			{
				BeginPaint(g_hWnd, &ps);
				SetTextColor(_DC, RGB(255, 0, 0));
				wsprintf(lpOut, TEXT("%d"), i + 1);	//캐릭터 번호부여
				TextOut(_DC, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 60, lpOut, lstrlen(lpOut));
				EndPaint(g_hWnd, &ps);
			}

		}
		else
		{
			RECT	tRect;
			tRect.left = (LONG)(tStoreData.tPlayersPos[i].fX - (m_tInfo.iCX >> 1));
			tRect.top = (LONG)(tStoreData.tPlayersPos[i].fY - (m_tInfo.iCY >> 1));

			if (tStoreData.team[i] == TEAMNUM::TEAM1)
			{
				BeginPaint(g_hWnd, &ps);
				SetTextColor(_DC, RGB(0, 0, 255));
				wsprintf(lpOut, TEXT("ME(%d)"), tStoreData.iClientIndex + 1);
				TextOut(_DC, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 60, lpOut, lstrlen(lpOut));
				EndPaint(g_hWnd, &ps);
			}
			else
			{
				BeginPaint(g_hWnd, &ps);
				SetTextColor(_DC, RGB(255, 0, 0));
				wsprintf(lpOut, TEXT("ME(%d)"), tStoreData.iClientIndex + 1);
				TextOut(_DC, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y - 60, lpOut, lstrlen(lpOut));
				EndPaint(g_hWnd, &ps);
			}
		}
	}	// 최대 체력 150 (현재체력 - 50)


	//HFONT myFont = CreateFont(20, 0, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	//HFONT oldFont = (HFONT)SelectObject(_DC, myFont);
	//TCHAR	szBuff[32] = L"";
	//swprintf_s(szBuff, L"%d / 150", m_pTarget->Get_Hp());
	//SetBkMode(_DC, 1); //투명
	//SetTextColor(_DC, RGB(0, 255, 0));
	//TextOut(_DC, m_tRect.left + iScrollX + 75, m_tRect.top + iScrollY + 10, szBuff, lstrlen(szBuff));
	//DeleteObject(myFont);

	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"UI_MANABAR");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 45, m_tRect.top + iScrollY + 38,
		m_pTarget->Get_Mana(), 12,  //x 마나 120
		hMemDC,
		0, 0,
		192, 16,
		RGB(255, 0, 255));
}

void CUI::Release()
{
}


