#include "stdafx.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "UI.h"
#include "SkillBar.h"
#include "Prison.h"
#include "TileMgr.h"
#include "Shop.h"
#include "KeyMgr.h"
#include "Potion.h"

CStage::CStage()
	:m_pPlayer(nullptr), m_pMonster(nullptr), m_bStage1_1(false), m_bStage1_2(false), m_bStage1_3(false)
	, m_bCard1(false), m_bCard2(false), m_bCard3(false)
	, m_pPrison1(nullptr), m_pPrison_Shop(nullptr), m_pShop(nullptr), m_pPrison_End(nullptr)
	, m_bStage2_1(false), m_bStage3_1(false), m_bStage3_2(false)
	, m_bStage4_1(false)//, m_bStage4_2(false)
	, m_bStage5_1(false)
	, m_bStage_Boss(false), m_pBoss(nullptr)
{
	m_lStartTime = GetTickCount64();

}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIRE_DRAGON_CARD.bmp", L"FIRE_DRAGON_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_KRYSTAL_CARD.bmp", L"ICE_KRYSTAL_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_BLAST_CARD.bmp", L"ICE_BLAST_CARD");

	CObj* pObj1 = CAbstractFactory<CMouse>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MOUSE, pObj1);

	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	m_pPlayer->Set_Target(pObj1);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, m_pPlayer);

	CObj* pObj = CAbstractFactory<CUI>::Create();
	pObj->Set_Target(m_pPlayer);
	CObjMgr::Get_Instance()->Add_Object(OBJID::INGAME_UI, pObj);

	pObj = CAbstractFactory<CSkillBar>::Create();
	pObj->Set_Target(m_pPlayer);
	CObjMgr::Get_Instance()->Add_Object(OBJID::INGAME_UI, pObj);
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/WOL_TILE_DUNGEON.bmp", L"WOL_TILE_DUNGEON");
	CTileMgr::Get_Instance()->Load_Tile();

}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();

	
	Add_Potion();
	Stage_Number();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
	Draw_Card(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage::Release()
{
}

void CStage::Stage_Number()
{

}

void CStage::Draw_Card(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (true == m_bCard1)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"FIRE_DRAGON_CARD");
		GdiTransparentBlt(_DC, 1300.f + iScrollX, 500.f + iScrollY,
			50, 100,
			hMemDC,
			0, 0,
			128, 192,
			RGB(255, 0, 255));
	}

	if (true == m_bCard2)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ICE_KRYSTAL_CARD");
		GdiTransparentBlt(_DC, 1300.f + iScrollX, 500.f + iScrollY,
			50, 100,
			hMemDC,
			0, 0,
			128, 192,
			RGB(255, 0, 255));
	}

	if (true == m_bCard3)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ICE_BLAST_CARD");
		GdiTransparentBlt(_DC, 1300.f + iScrollX, 500.f + iScrollY,
			50, 100,
			hMemDC,
			0, 0,
			128, 192,
			RGB(255, 0, 255));
	}
}

void CStage::Add_Potion()
{
	// 랜덤하게 체력약 생성
	if (GetTickCount64() - m_lStartTime > 10000)
	{
		m_lStartTime = GetTickCount64();

		int iX = (rand() % 1000) + 50;
		int iY = (rand() % 500) + 50;

		CObj* pObj1 = CAbstractFactory<CPotion>::Create();
		pObj1->Set_Pos(iX, iY);
		CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj1);
	}
}
