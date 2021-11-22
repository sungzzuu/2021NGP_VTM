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
typedef struct tagPlayerInfo
{
	INFO tPos;

}PLAYER_INFO;

typedef struct tagStoreData
{
	INFO tPlayersPos[4] = {0};
	int iClientIndex = 0;
}STORE_DATA;



///////////////////////////////////////////////// 서버에도 있는 구조체
struct HpPotionInfo
{
	int		cnt; // 몇개의 클라에 보냈는지
	int		index; // 체력약 리스트 중 몇번째?
	bool	bCreateOn;
	POINT	pos;
};
typedef struct tagHpPotionRes
{
	bool	bCollision;
	int		iIndex;
}POTIONRES;

#endif // !__STRUCT_H__
