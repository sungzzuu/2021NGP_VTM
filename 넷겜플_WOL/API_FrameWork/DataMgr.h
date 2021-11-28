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
private:
	static CDataMgr* m_pInstance;

public:
	PLAYER_INFO			m_tPlayerInfo;
	STORE_DATA			m_tStoreData;
	ATTACKINFO*			m_pAttackInfo = nullptr;
	int					m_iSize;

};


#endif // !__DATAMGR_H__
