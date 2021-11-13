#pragma once
#include "Obj.h"
class CSkillBar : public CObj
{
public:
	CSkillBar();
	virtual ~CSkillBar();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	float m_iQSpeed;
	float m_iESpeed;
	float m_iRSpeed;

	bool	m_bRight_Draw;
	bool	m_bQ_Draw;
	bool	m_bE_Draw;
	bool	m_bR_Draw;

};

