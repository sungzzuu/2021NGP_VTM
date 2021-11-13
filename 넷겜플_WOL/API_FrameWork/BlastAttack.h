#pragma once
#include "Obj.h"
class CBlastAttack : public CObj
{
public:
	CBlastAttack();
	virtual ~CBlastAttack();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Scene_Change();

private:
	DWORD		dwTime;
	bool		m_bScene;
};

