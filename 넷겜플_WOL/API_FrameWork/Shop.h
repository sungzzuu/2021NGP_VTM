#pragma once
#include "Obj.h"
class CShop : public CObj
{
public:
	CShop();
	virtual ~CShop();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Set_Potion() { m_bPotion = false; }
	void Set_Shield() { m_bShield = false; }

private:
	bool	m_bPotion;
	bool	m_bShield;
};

