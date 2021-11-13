#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__

class CTile;
class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Picking_Tile(int _iDrawIDX, int _iDrawIDY, int _iOption);

public:
	void Save_Tile();
	void Load_Tile();

public:
	vector<CObj*> Get_Tile() { return m_vecTile; }

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTileMgr*	m_pInstance;
	vector<CObj*>		m_vecTile;

};


#endif // !__TILEMGR_H__
