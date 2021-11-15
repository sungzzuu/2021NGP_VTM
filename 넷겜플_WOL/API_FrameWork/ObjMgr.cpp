#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		for (; iter != m_listObj[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}


	//CCollisionMgr::Collision_Wall(m_listObj[OBJID::PLAYER]);//플레이어 벽충돌

	//CCollisionMgr::Collision_Wall_Att(m_listObj[OBJID::ATTACK]);//플레이어공격  벽충돌(불)

	//if(!m_listObj[OBJID::PRISON].empty())
	//	CCollisionMgr::Collision_Prison(m_listObj[OBJID::PLAYER], m_listObj[OBJID::PRISON]); //플레이어 감옥충돌

	//if (!m_listObj[OBJID::GOLD].empty())
	CCollisionMgr::Collision_Gold(m_listObj[OBJID::PLAYER], m_listObj[OBJID::GOLD]); //플레이어 돈충돌

	//if (!m_listObj[OBJID::SHOP].empty())
	//	CCollisionMgr::Collision_Prison(m_listObj[OBJID::PLAYER], m_listObj[OBJID::SHOP]); //플레이어 상점충돌

}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update();
			if (m_listObj[i].empty())
				break;
			GROUPID::ID eID = pObj->Get_GroupID();
			m_RenderGroup[eID].emplace_back(pObj);
		}
	}
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < GROUPID::END; ++i)
	{
		if (GROUPID::OBJECT == i)
			m_RenderGroup[i].sort(CompareY<CObj*>);

		for (auto& pObj : m_RenderGroup[i])
			pObj->Render(_DC);

		m_RenderGroup[i].clear();
	}
	/*for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
			pObj->Render(_DC);
	}*/
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj* _pObj)
{
	m_listObj[_eID].emplace_back(_pObj);
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for (auto& pObj : m_listObj[_eID])
		Safe_Delete(pObj);
	m_listObj[_eID].clear();
}

CObj* CObjMgr::Get_Target(CObj* _pDst, OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;
	float fDis = 0.f;

	for (auto& _Src : m_listObj[_eID])
	{
		float fX = abs(_pDst->Get_Info().fX - _Src->Get_Info().fX);
		float fY = abs(_pDst->Get_Info().fY - _Src->Get_Info().fY);
		float fDis2 = sqrtf(fX * fX + fY * fY);

		if (fDis > fDis2 || !pTarget)
		{
			pTarget = _Src;
			fDis = fDis2;
		}
	}

	return pTarget;
}

bool CObjMgr::Get_AllDead(OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return true;
	else
		return false;
}
