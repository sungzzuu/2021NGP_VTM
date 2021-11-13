#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj);

public:
	void Delete_ID(OBJID::ID _eID);

public:
	CObj* Get_Player() const { return m_listObj[OBJID::PLAYER].front(); }
	CObj* Get_Target(CObj* _pDst, OBJID::ID _eID);
	bool Get_AllDead(OBJID::ID _eID);

	// ΩÃ±€≈Ê ∆–≈œ
public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	list<CObj*>		m_listObj[OBJID::END];

	// ΩÃ±€≈Ê ∆–≈œ
	static CObjMgr*		m_pInstance;

	list<CObj*>		m_RenderGroup[GROUPID::END];

};


#endif // !__OBJMGR_H__
