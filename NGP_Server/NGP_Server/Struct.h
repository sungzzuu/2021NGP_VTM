#pragma once

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