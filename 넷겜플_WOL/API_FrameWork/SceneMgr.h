#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

class CScene;
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { SCENE_LOGO, SCENE_MENU, SCENE_STAGE, SCENE_EDIT, SCENE_END };

public:
	void Scene_Change(SCENEID _eID);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

	SCENEID				m_eCurScene;
	SCENEID				m_ePreScene;
};


#endif // !__SCENEMGR_H__
