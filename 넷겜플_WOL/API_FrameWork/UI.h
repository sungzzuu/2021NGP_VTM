#pragma once
#include "Obj.h"
class CUI : public CObj
{
public:
	CUI();
	virtual ~CUI();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


};

