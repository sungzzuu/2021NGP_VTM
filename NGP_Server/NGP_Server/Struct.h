#pragma once

struct HpPotionInfo
{
	int		cnt; // ��� Ŭ�� ���´���
	int		index; // ü�¾� ����Ʈ �� ���°?
	bool	bCreateOn;
	POINT	pos;
};


typedef struct tagHpPotionRes
{
	bool	bCollision;
	int		iIndex;
}POTIONRES;