#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "ScrollMgr.h"


CTileMgr* CTileMgr::m_pInstance = nullptr;
CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)((TILE_RECTCX >> 1) + (TILE_RECTCX * j));
			float fY = (float)((TILE_RECTCY >> 1) + (TILE_RECTCY * i));

			CObj* pObj = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.emplace_back(pObj);
		}
	}
}

void CTileMgr::Update()
{
}

void CTileMgr::Late_Update()
{
}

void CTileMgr::Render(HDC _DC)
{
	//int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//int iCullX = abs(iScrollX / TILE_RECTCX);
	//int iCullY = abs(iScrollY / TILE_RECTCY);

	//int iCullEndX = iCullX + WINCX / TILE_RECTCX + 2;
	//int iCullEndY = iCullY + WINCY / TILE_RECTCY + 2;

	//for (int i = iCullY; i < iCullEndY; ++i)
	//{
	//	for (int j = iCullX; j < iCullEndX; ++j)
	//	{
	//		int iIdx = i * TILEX + j;			/////////////////////////////////

	//		if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
	//			continue;	

	//		m_vecTile[iIdx]->Render(_DC);
	//	}
	//}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();
}

void CTileMgr::Picking_Tile(int _iDrawIDX, int _iDrawIDY, int _iOption)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= iScrollX;
	pt.y -= iScrollY;   

	int x = pt.x / TILE_RECTCX;
	int y = pt.y / TILE_RECTCY;

	int iIdx = y * TILEX + x;

	cout << iIdx << endl;

	if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIdx])->Set_DrawIDX(_iDrawIDX);
	dynamic_cast<CTile*>(m_vecTile[iIdx])->Set_DrawIDY(_iDrawIDY);
	dynamic_cast<CTile*>(m_vecTile[iIdx])->Set_Option(_iOption);
}
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/TestTile.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (auto& pTile : m_vecTile)
	{
		WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_DrawIDX(), sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_DrawIDY(), sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_Option(), sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD dwByte = 0;
	INFO tTemp = {};
	int iDrawIDX = 0, iDrawIDY = 0, iOption = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iDrawIDX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iDrawIDY, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CTile* pTile = new CTile;
		pTile->Load_Tile(tTemp);
		pTile->Set_DrawIDX(iDrawIDX);
		pTile->Set_DrawIDY(iDrawIDY);
		pTile->Set_Option(iOption);

		m_vecTile.emplace_back(pTile);
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);
}