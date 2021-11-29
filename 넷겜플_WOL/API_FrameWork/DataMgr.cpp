#include "stdafx.h"
#include "DataMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "BmpMgr.h"

CDataMgr* CDataMgr::m_pInstance = nullptr;
CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

TCHAR* CDataMgr::GetFrameKey(int iFrameKey)
{
	switch (iFrameKey)
	{
	case 1:
		return L"Player_UP";
	case 2:
		return L"Player_DOWN";
	case 3:
		return L"Player_LEFT";
	case 4:
		return L"Player_RIGHT";
	default:
		break;
	}
	return nullptr;
}

int CDataMgr::SetFrameKey(TCHAR* pFrameKey)
{
	if (pFrameKey==L"Player_UP")
	{
		return 1;
	}
	else if (pFrameKey == L"Player_DOWN")
	{
		return 2;

	}
	else if (pFrameKey == L"Player_LEFT")
	{
		return 3;

	}
	else if (pFrameKey == L"Player_RIGHT")
	{
		return 4;

	}

	return 0;
}

void CDataMgr::SetAttackArr()
{
	list<CObj*> list_Attack = CObjMgr::Get_Instance()->Get_list(OBJID::ATTACK);

	if (m_pAttackData.pAttackInfo != nullptr)
		delete[] m_pAttackData.pAttackInfo;

	m_pAttackData.iSize = list_Attack.size();
	m_pAttackData.pAttackInfo = new ATTACKINFO[m_pAttackData.iSize];

	int cnt = -1;
	for (auto& attack : list_Attack)
	{
		cnt++;
		m_pAttackData.pAttackInfo[cnt].iType = attack->m_iAttackName;
		m_pAttackData.pAttackInfo[cnt].tInfo = attack->Get_Info();
		FRAME tFrame = attack->Get_Frame();
		m_pAttackData.pAttackInfo[cnt].iFrameStart = tFrame.iFrameStart;
		m_pAttackData.pAttackInfo[cnt].iFrameScene = tFrame.iFrameScene;
		m_pAttackData.pAttackInfo[cnt].bCollision = false;
		

		//tAttackInfo.iType = attack->m_eAttackName;
		//tAttackInfo.tInfo = attack->Get_Info();
		//FRAME tFrame = attack->Get_Frame();
		//tAttackInfo.iFrameStart = tFrame.iFrameStart;
		//tAttackInfo.iFrameScene = tFrame.iFrameScene;
		//tAttackInfo.bCollision = false;
		//m_vecAttackInfo.push_back(tAttackInfo);
	}

}

void CDataMgr::RenderOthersAttack(HDC _DC)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_pOthersAttackData[i].iSize == 0)
			continue;

		for (int j = 0; j < m_pOthersAttackData[i].iSize; j++)
		{
			POINT size;
			TCHAR* pFrameKey = GetFrameKey_Attack(m_pOthersAttackData[i].pAttackInfo[j].iType, size);
			HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Normal_ATACK");

			//Ellipse(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
			RECT tRect;
			INFO tInfo = m_pOthersAttackData[i].pAttackInfo[j].tInfo;
			tRect.left = (LONG)(tInfo.fX - (tInfo.iCX >> 1));
			tRect.top = (LONG)(tInfo.fY - (tInfo.iCY >> 1));
			tRect.right = (LONG)(tInfo.fX + (tInfo.iCX >> 1));
			tRect.bottom = (LONG)(tInfo.fY + (tInfo.iCY >> 1));

			GdiTransparentBlt(_DC
				, tRect.left, tRect.top
				, CHAR_CX, CHAR_CY
				, hMemDC
				, m_pOthersAttackData[i].pAttackInfo[j].iFrameStart * size.x, m_pOthersAttackData[i].pAttackInfo[j].iFrameScene * size.y	//시작좌표
				, size.x, size.y													//길이
				, RGB(255, 0, 255));
		}
		
	}
	
}

TCHAR* CDataMgr::GetFrameKey_Attack(int iType, POINT& pt)
{
	switch (iType)
	{
	case 0:
		pt.x = 200;
		pt.y = 200;
		return L"Normal_ATTACK";
	case 1:
		pt.x = 180;
		pt.y = 180;
		return L"SKILL_FIREDRAGON";
	case 2:
		pt.x = 200;
		pt.y = 200;
		return L"Ice_ATTACK";
	case 3:
		pt.x = 200;
		pt.y = 250;
		return L"ICE_BLAST";
	default:
		break;
	}
}

