#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"


CLine::CLine()
{
}


CLine::CLine(LINEPOS& _Left, LINEPOS& _Right)
	: m_tInfo(_Left, _Right)
{

}

CLine::~CLine()
{
	Release();
}

void CLine::Initialize()
{
}

void CLine::Update()
{
}

void CLine::Late_Update()
{
}

void CLine::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(_DC, (int)m_tInfo.tLeftPos.fX + iScrollX, (int)m_tInfo.tLeftPos.fY, nullptr);
	LineTo(_DC, (int)m_tInfo.tRightPos.fX + iScrollX, (int)m_tInfo.tRightPos.fY);
}

void CLine::Release()
{
}
