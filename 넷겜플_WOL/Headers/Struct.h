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
	int		iFrameStart; // 
	int		iFrameScene; // 
	int		iFrameKey;//  (PLAYER_DOWN)			

}FRAME_INFO;

typedef struct tagPlayerInit
{
	POS tPos[4] = { 0 };
	TEAMNUM::TEAM team[4];
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT;

typedef struct tagPlayerInitSend
{
	POS tPos = { 0 };
	TEAMNUM::TEAM team[4];
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT_SEND;


typedef struct tagPlayerInfo
{
	POS tPos = { 0 };
	FRAME_INFO	tFrameInfo;

	int iHp;

	TEAMNUM::TEAM team;
	bool start;
	bool button;

	bool				isHit = false; //충돌 판정
	bool				isDead = false; //죽었다 살아나면 false로 변환 필요 / True일때 충돌x
	ENDING::END_TYPE	eEnding = ENDING::ING;

	//FRAME tFrame;

}PLAYER_INFO;

//typedef struct tagSkillInfo
//{
//	std::vector<POS> vecFirePos;
//	bool isHit = false;
//}SKILL_INFO;

typedef struct tagStoreData
{
	PLAYER_INFO tPlayersInfo[4] = { 0 };
	//SKILL_INFO tFiresPos[4];
	int iClientIndex = 0;

	int iHp[4] = {};
	TEAMNUM::TEAM team[4];
	bool start;
}STORE_DATA;

//////////////////////////////////// 
struct HpPotionCreate
{
	int		cnt; // 
	int		index; // 
	bool	bCreateOn;
	POS		pos;
};

struct HpPotionDelete
{
	int		cnt; // 
	int		index; // 
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
	size_t			iType = 0;			 
	INFO			tInfo;			 
	int				iFrameScene = 0;	 
	int				iFrameStart = 0;	 
	bool			bCollision = 0;

}ATTACKINFO;

struct AttackData
{
	int iSize = 0;
	ATTACKINFO* pAttackInfo = nullptr;
};

#endif // !__STRUCT_H__
