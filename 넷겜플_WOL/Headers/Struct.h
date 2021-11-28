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
	int		iFrameStart; // 열
	int		iFrameScene; // 행
	int		iFrameKey;// 프레임키 (PLAYER_DOWN)			

}FRAME_INFO;

typedef struct tagPlayerInfo
{
	INFO		tPos;
	FRAME_INFO	tFrameInfo;

}PLAYER_INFO;

typedef struct tagStoreData
{
	PLAYER_INFO tPlayersInfo[4] = {0};
	int iClientIndex = 0;

}STORE_DATA;

//////////////////////////////////// 서버
struct HpPotionCreate
{
	int		cnt; // 몇개의 클라에 보냈는지
	int		index; // 체력약 리스트 중 몇번째?
	bool	bCreateOn;
	POINT	pos;
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

typedef struct tagAttackInfo
{
	size_t			iType = 0;			// 4개의 공격 중에서 뭔지
	INFO			tInfo;			// 좌표와 가로세로 크기
	int				iFrameScene = 0;	// 행
	int				iFrameStart = 0;	// 열
	bool			bCollision = 0;

}ATTACKINFO;

struct AttackData
{
	int iSize = 0;
	ATTACKINFO* pAttackInfo = nullptr;
};

#endif // !__STRUCT_H__
