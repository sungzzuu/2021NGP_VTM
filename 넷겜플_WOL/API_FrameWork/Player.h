#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void	UpdateBeforeRender();


public:
	void Set_Bullet(list<CObj*>* _pBullet) { m_pBullet = _pBullet; }
	void Set_Shield(list<CObj*>* _pShield) { m_pShield = _pShield; }
	void Set_Right(bool right) { m_bRight_On = right; }
	void Set_Q(bool q) { m_bQ_On = q; }
	void Set_E(bool e) { m_bE_On = e; }
	void Set_R(bool r) { m_bR_On = r; }
public:
	const bool Get_Right() { return m_bRight_On; }
	const bool Get_Q() { return m_bQ_On; }
	const bool Get_E() { return m_bE_On; }
	const bool Get_R() { return m_bR_On; }

	const bool Get_QCool() { return m_bQ_Cool; }
	const bool Get_ECool() { return m_bE_Cool; }
	const bool Get_RCool() { return m_bR_Cool; }

	const int Get_Right_Remain() { return m_iRight_Remain; }

private:
	void Key_Check();
	void OffSet();
	void Scene_Change();

public:
	void Move();
	void Dash();
	void Hit();
	void Normal_Att();
	void Fire_Att();
	void Ice_Att();
	void Shield_Att();
	void Blast_Att();

private:
	list<CObj*>*	m_pBullet;
	list<CObj*>*	m_pShield;

	POINT			m_tPosin;
	POINT			m_tPoLeft;
	POINT			m_tPoRight;


	float			m_fPoSize;

	float			m_fJumpPower;
	float			m_fJumpAccel;
	bool			m_bJump;

	DWORD			Dash_Time;
	bool			m_bDash;
	
	float			m_fDashSpeed;

	int				Attack_Scene;

	bool			m_bBlast_GetMouse;
	POINT			m_tMouse;
	POINT			m_tBlast;
	DWORD			Blast_Time;
	float			m_fBlast_Dis;
	float			m_fBlast_Ang;
	float			m_fBlast_ScrX;
	float			m_fBlast_ScrY;
	bool			m_Blast_TargetPalyer;

	bool			m_bFireLeft;

	bool			m_bRight_On;
	bool			m_bQ_On;
	bool			m_bE_On;
	bool			m_bR_On;

	bool			m_bQ_Cool;
	bool			m_bE_Cool;
	bool			m_bR_Cool;

	DWORD			m_dQCool_Time;
	DWORD			m_dECool_Time;
	DWORD			m_dRCool_Time;

	int				m_iRight_Remain;
	DWORD			m_dRight_Time;

	int				m_iNormal_Idx;
	int				m_iFire_Idx;
	int				m_iBlast_Idx;


};


#endif // !__PLAYER_H__
