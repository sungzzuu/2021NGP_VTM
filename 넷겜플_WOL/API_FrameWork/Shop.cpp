#include "stdafx.h"
#include "Shop.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CShop::CShop()
{
}


CShop::~CShop()
{
}

void CShop::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ITEMSHOP_NPC.bmp", L"ITEMSHOP_NPC");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SHOP_TABLE.bmp", L"SHOP_TABLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GAIA_ARMOR_CARD.bmp", L"GAIA_ARMOR_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/POTION.bmp", L"POTION");

	m_tInfo.iCX = 200.f;
	m_tInfo.iCY = 80.f;

	m_bPotion = true;
	m_bShield = true;

	m_eGroup = GROUPID::OBJECT;

}

int CShop::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CShop::Late_Update()
{
}

void CShop::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"SHOP_TABLE");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		m_tInfo.iCX, m_tInfo.iCY + 40,
		hMemDC,
		0, 0,
		606, 234,
		RGB(255, 0, 255));

	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);


	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ITEMSHOP_NPC");
	GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 70, m_tRect.top + iScrollY - 80,
		50, 80,
		hMemDC,
		0, 0,
		128, 197,
		RGB(255, 0, 255));

	if (true == m_bPotion)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"POTION");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 30, m_tRect.top + iScrollY ,
			50, 80,
			hMemDC,
			0, 0,
			106, 155,
			RGB(255, 0, 255));
	}
	if (true == m_bShield)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"GAIA_ARMOR_CARD");
		GdiTransparentBlt(_DC, m_tRect.left + iScrollX + 120, m_tRect.top + iScrollY,
			50, 80,
			hMemDC,
			0, 0,
			106, 172,
			RGB(255, 0, 255));
	}
}

void CShop::Release()
{
}
