#pragma once

#ifndef __TILE_H__
#define __TILE_H__

#include "Obj.h"
class CTile : public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Set_DrawIDX(int _iDrawIDX) { m_iDrawIDX = _iDrawIDX; }
	void Set_DrawIDY(int _iDrawIDY) { m_iDrawIDY = _iDrawIDY; }
	void Set_Option(int _iOption) { m_iOption = _iOption; }

public:
	const int& Get_DrawIDX() { return m_iDrawIDX; }
	const int& Get_DrawIDY() { return m_iDrawIDY; }

	//const int& Get_Option() { return m_iOption; }

public:
	void Load_Tile(INFO& _rInfo) { memcpy(&m_tInfo, &_rInfo, sizeof(INFO)); }

private:
	int		m_iDrawIDX;
	int		m_iDrawIDY;

	//int		m_iOption;
};


#endif // !__TILE_H__

