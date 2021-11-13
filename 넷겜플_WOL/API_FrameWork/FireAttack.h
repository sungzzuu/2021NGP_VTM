#pragma once
#include"Obj.h"
class CFireAttack : public CObj
{
public:
	CFireAttack();
	~CFireAttack();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Scene_Change();

private:
	float	m_fRotAngle;
	float	m_fRotDis;
	float	m_fRotSpeed;
	POINT	m_tRotPos;
	POINT	m_tStartPos;

	bool	m_bStart;

	bool	m_bScene;

	TCHAR*	m_pParticleKey;

	POINT	m_pParti_Pos;
	RECT	m_tParti_Rect;
	int		m_iParti_ImageCX; 
	int		m_iParti_ImageCY;
	LONG	Parti_Image_Dif_X;
	LONG	Parti_Image_Dif_Y;
};

