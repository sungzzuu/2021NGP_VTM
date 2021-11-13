#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr*	CScrollMgr::m_pInstance = nullptr;
CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	if (0 <= m_fScrollX)
		m_fScrollX = 0.f;

	if (0 <= m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCX - (TILE_RECTCX * TILEX) >= m_fScrollX)
		m_fScrollX = WINCX - (TILE_RECTCX * TILEX);

	if (WINCY - (TILE_RECTCY * TILEY) >= m_fScrollY)
		m_fScrollY = WINCY - (TILE_RECTCY * TILEY);
}