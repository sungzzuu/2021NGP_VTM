#pragma once
#include "Obj.h"
class CPotion : public CObj
{
public:
	CPotion();
	virtual ~CPotion();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	int GetIndex() { return m_iIndex; }
	void SetIndex(int iIndex) { m_iIndex = iIndex; }
	void CallBackCollision();
private:
	int m_iIndex;

};

