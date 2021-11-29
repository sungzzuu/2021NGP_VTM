#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

typedef struct taPos
{
	float	fX;
	float	fY;
}POS;


typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float	fX;
	float	fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _Left, LINEPOS& _Right) : tLeftPos(_Left), tRightPos(_Right) {}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;        
}FRAME;

/// ///////////////////////////////////////////
typedef struct tagFrameInfo
{
	int		iFrameStart; // ��
	int		iFrameScene; // ��
	int		iFrameKey;// ������Ű (PLAYER_DOWN)			

}FRAME_INFO;

typedef struct tagPlayerInfo
{
	POS tPos = { 0 };
	FRAME_INFO	tFrameInfo;

	//INFO tPos;
	int iHp;

	TEAMNUM::TEAM team;
	bool start;
	bool button;
	//FRAME tFrame;
}PLAYER_INFO;

typedef struct tagSkillInfo
{
	std::vector<POS> vecFirePos;
	bool isHit = false;
}SKILL_INFO;

typedef struct tagStoreData
{
	PLAYER_INFO tPlayersInfo[4] = { 0 };
	//SKILL_INFO tFiresPos[4];
	int iClientIndex = 0;

	int iHp[4] = {};
	TEAMNUM::TEAM team[4];
	bool start;
	bool button;
}STORE_DATA;



//////////////////////////////////// ����
struct HpPotionCreate
{
	int		cnt; // ��� Ŭ�� ���´���
	int		index; // ü�¾� ����Ʈ �� ���°?
	bool	bCreateOn;
	POS	pos;
};

struct HpPotionDelete
{
	int		cnt; // ��� Ŭ�� ���´���
	int		index; // ü�¾� ����Ʈ �� ���°?
	bool	bDeleteOn;
};

struct HpPotionInfo
{
	HpPotionCreate thpPotionCreate;
	HpPotionDelete thpPotionDelete;
};

//struct HpPotion
typedef struct tagHpPotionRes
{
	bool	bCollision;
	int		iIndex;
}POTIONRES;

typedef struct tagAttackInfo
{
	size_t			iType = 0;			// 4���� ���� �߿��� ����
	INFO			tInfo;			// ��ǥ�� ���μ��� ũ��
	int				iFrameScene = 0;	// ��
	int				iFrameStart = 0;	// ��
	bool			bCollision = 0;

}ATTACKINFO;

struct AttackData
{
	int iSize = 0;
	ATTACKINFO* pAttackInfo = nullptr;
};

#endif // !__STRUCT_H__
