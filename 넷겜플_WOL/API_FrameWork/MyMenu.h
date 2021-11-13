#pragma once

#ifndef __MYMENU_H__
#define __MYMENU_H__


#include "Scene.h"
class CMyMenu : public CScene
{
public:
	CMyMenu();
	virtual ~CMyMenu();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__MYMENU_H__
