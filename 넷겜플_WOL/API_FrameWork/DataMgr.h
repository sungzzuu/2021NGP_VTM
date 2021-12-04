#pragma once

#ifndef __DATAMGR_H__
#define __DATAMGR_H__

class CDataMgr
{
private:
	CDataMgr();
	~CDataMgr();


public:
	static CDataMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CDataMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

	TCHAR* GetFrameKey(int iFrameKey);
	int SetFrameKey(TCHAR* pFrameKey);

	void SetAttackArr();	// 리스트 갱신
	void RenderOthersAttack(HDC _DC);
	TCHAR* GetFrameKey_Attack(int iType, POINT& pt, POINT& imSize, UINT& tpColor);
private:
	static CDataMgr* m_pInstance;

public:
	PLAYER_INIT			m_tPlayerInit;
	PLAYER_INFO			m_tPlayerInfo;
	STORE_DATA			m_tStoreData;
	AttackData			m_pAttackData;
	AttackData			m_pOthersAttackData[4];


};


#endif // !__DATAMGR_H__
