#pragma once

#ifndef __STAGE_H__
#define __STAGE_H__


#include "Scene.h"
class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Stage_Number();
	void Draw_Card(HDC _DC);
private:
	CObj*	m_pPlayer;
	CObj*	m_pMonster;

	bool	m_bCard1;
	bool	m_bCard2;
	bool	m_bCard3;

	CObj*	m_pPrison1;
	CObj*	m_pPrison_Shop;
	CObj*	m_pPrison_End;

	bool	m_bStage1_1;
	bool	m_bStage1_2;
	bool	m_bStage1_3;

	bool	m_bStage2_1;

	bool	m_bStage3_1;
	bool	m_bStage3_2;

	bool	m_bStage4_1;
	//bool	m_bStage4_2;
	CObj*	m_pShop;

	bool	m_bStage5_1;
	bool	m_bStage5_2;

	bool    m_bStage_Boss;
	CObj*	m_pBoss;

};


#endif // !__STAGE_H__
