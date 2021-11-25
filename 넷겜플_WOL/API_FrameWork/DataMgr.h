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

private:
	static CDataMgr* m_pInstance;

public:
	PLAYER_INFO m_tPlayerInfo;
	STORE_DATA m_tStoreData;

};


#endif // !__DATAMGR_H__
