#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CTile::CTile()
	: m_iDrawIDX(7), m_iDrawIDY(0)//, m_iOption(0)
{
}


CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.iCX = TILE_RECTCX;
	m_tInfo.iCY = TILE_RECTCY;

	m_iImageCX = TILECX;
	m_iImageCY = TILECY;

	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);

	m_eGroup = GROUPID::BACKGROUND;

}

int CTile::Update()
{

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Tile");
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"WOL_TILE_DUNGEON");

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
	//	, TILECX, TILECY
	//	, hMemDC
	//	, m_iDrawID * TILECX, 0
	//	, SRCCOPY);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, TILE_RECTCX, TILE_RECTCY
		, hMemDC
		, m_iDrawIDX * 146, m_iDrawIDY * 145	//시작좌표
		, TILECX, TILECY													//길이
		, RGB(255, 0, 255));

	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

}

void CTile::Release()
{
}
