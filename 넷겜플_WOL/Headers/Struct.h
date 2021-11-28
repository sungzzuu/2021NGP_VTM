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
typedef struct tagPlayerInfo
{
	POS tPos = { 0 };
	//FRAME tFrame;
}PLAYER_INFO;

typedef struct tagSkillInfo
{
	std::vector<POS> vecFirePos;
	bool isHit = false;
	//FRAME tFrame;
}SKILL_INFO;

typedef struct tagStoreData
{
	POS tPlayersPos[4] = { 0 };
	SKILL_INFO tFiresPos[4];
	int iClientIndex = 0;
}STORE_DATA;

//////////////////////////////////// 서버
struct HpPotionCreate
{
	int		cnt; // 몇개의 클라에 보냈는지
	int		index; // 체력약 리스트 중 몇번째?
	bool	bCreateOn;
	POS	pos;
};

struct HpPotionDelete
{
	int		cnt; // 몇개의 클라에 보냈는지
	int		index; // 체력약 리스트 중 몇번째?
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
#endif // !__STRUCT_H__
