#pragma once

#ifndef __MYEDIT_H__
#define __MYEDIT_H__


#include "Scene.h"
class CMyEdit : public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Key_Check();
};


#endif // !__MYEDIT_H__
