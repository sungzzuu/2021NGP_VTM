#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "TileMgr.h"
#include "Tile.h"
#include "ScrollMgr.h"

int CCollisionMgr::m_iGold_Idx = 0;

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

/*void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				//cout << "충돌" << endl;
				if (fX > fY)
				{
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
				else
				{
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
			}
		}
	}
}*/

void CCollisionMgr::Collision_Shield(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		if(true == Dst->Get_Arrow())  //활 불 만
		{ 
			for (auto& Src : _Src)
			{
				if (Check_Sphere(Dst, Src))
				{
					Dst->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Gold(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				//++m_iGold_Idx;
				Dst->Set_Hp(15);
				Src->Set_Dead();
			}
		}
	}
}


void CCollisionMgr::Collision_Wall(list<CObj*>& _Dst)
{
	float fX = 0.f, fY = 0.f;
	vector<CObj*> Tile = CTileMgr::Get_Instance()->Get_Tile();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iCullX = abs(iScrollX / TILE_RECTCX);
	int iCullY = abs(iScrollY / TILE_RECTCY);

	int iCullEndX = iCullX + WINCX / TILE_RECTCX + 4;
	int iCullEndY = iCullY + WINCY / TILE_RECTCY + 4;


	for (auto& Dst : _Dst)
	{
		for (int i = iCullY; i < iCullEndY; ++i)
		{
			for (int j = iCullX; j < iCullEndX; ++j)
			{
				int iIdx = i * TILEX + j;
				if (0 > iIdx || Tile.size() <= (size_t)iIdx)
					continue;

				if (1 == Tile[iIdx]->Get_Option())
				{
					if (Check_Rect(Dst, Tile[iIdx], &fX, &fY))
					{
						if (fX > fY)
						{
							if (Dst->Get_Info().fY < Tile[iIdx]->Get_Info().fY)
								Dst->Set_PosY(-fY);
							else
								Dst->Set_PosY(fY);
						}
						else
						{
							if (Dst->Get_Info().fX < Tile[iIdx]->Get_Info().fX)
								Dst->Set_PosX(-fX);
							else
								Dst->Set_PosX(fX);
						}
					}
				}
			}
		}
		
	}


}

void CCollisionMgr::Collision_Wall_Att(list<CObj*>& _Dst)
{
	float fX = 0.f, fY = 0.f;
	vector<CObj*> Tile = CTileMgr::Get_Instance()->Get_Tile();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iCullX = abs(iScrollX / TILE_RECTCX);
	int iCullY = abs(iScrollY / TILE_RECTCY);

	int iCullEndX = iCullX + WINCX / TILE_RECTCX + 2;
	int iCullEndY = iCullY + WINCY / TILE_RECTCY + 2;


	for (auto& Dst : _Dst)
	{
		if(true == Dst->Get_Arrow() || true == Dst->Get_Fire())  //몬스터 활 불 or 플레이어 불
		{
			for (int i = iCullY; i < iCullEndY; ++i)
			{
				for (int j = iCullX; j < iCullEndX; ++j)
				{
					int iIdx = i * TILEX + j;
					if (0 > iIdx || Tile.size() <= (size_t)iIdx)
						continue;

					if (1 == Tile[iIdx]->Get_Option())
					{
						if (Check_Rect(Dst, Tile[iIdx], &fX, &fY))
						{
							Dst->Set_Dead();			//투사체 공격 없어짐
						}
					}
				}
			}
		}

	}

}

void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Src->Set_Collision(true);

				if (true == Dst->Get_Arrow()) //활 위자드불 공격 없어짐
					Dst->Set_Dead();
				//Dst->Set_Dead();
				//Src->Set_Dead();
			}
		}
	}
}


bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	float fRad = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCY) >> 1);

	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDis = sqrtf(fX * fX + fY * fY);

	return fRad > fDis;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fCX > fX && fCY > fY)
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		return true;
	}

	return false;
}
