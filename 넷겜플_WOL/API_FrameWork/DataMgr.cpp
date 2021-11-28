#include "stdafx.h"
#include "DataMgr.h"
#include "ObjMgr.h"
#include "Obj.h"

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

	if (m_pAttackInfo != nullptr)
		delete[] m_pAttackInfo;

	m_iSize = list_Attack.size();
	m_pAttackInfo = new ATTACKINFO[m_iSize];

	int cnt = 0;
	for (auto& attack : list_Attack)
	{
		m_pAttackInfo[cnt].iType = attack->m_eAttackName;
		m_pAttackInfo[cnt].tInfo = attack->Get_Info();
		FRAME tFrame = attack->Get_Frame();
		m_pAttackInfo[cnt].iFrameStart = tFrame.iFrameStart;
		m_pAttackInfo[cnt].iFrameScene = tFrame.iFrameScene;
		m_pAttackInfo[cnt].bCollision = false;
		cnt++;

		//tAttackInfo.iType = attack->m_eAttackName;
		//tAttackInfo.tInfo = attack->Get_Info();
		//FRAME tFrame = attack->Get_Frame();
		//tAttackInfo.iFrameStart = tFrame.iFrameStart;
		//tAttackInfo.iFrameScene = tFrame.iFrameScene;
		//tAttackInfo.bCollision = false;
		//m_vecAttackInfo.push_back(tAttackInfo);
	}

}

