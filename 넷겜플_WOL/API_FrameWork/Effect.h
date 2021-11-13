#pragma once
#include "Obj.h"
class CEffect : public CObj
{
public:
	CEffect();
	virtual ~CEffect();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	bool m_bScene;
	DWORD		dwTime;

};

