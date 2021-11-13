#pragma once
#include "Obj.h"
class CPrison : public CObj
{
public:
	CPrison();
	virtual ~CPrison();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	
	void Set_Bar(bool bar) { m_bBar = bar; }

private:
	bool m_bBar;
};

