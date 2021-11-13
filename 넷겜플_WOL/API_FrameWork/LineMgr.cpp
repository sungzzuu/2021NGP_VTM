#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;
CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	Load_Line();
}

void CLineMgr::Update()
{
}

void CLineMgr::Late_Update()
{
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& pLine : m_listLine)
		pLine->Render(_DC);
}

void CLineMgr::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
}

bool CLineMgr::Collision_Line(float _x, float* _y)
{
	if (m_listLine.empty())
		return false;

	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_listLine)
	{
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& pLine->Get_Info().tRightPos.fX >= _x)
			pTargetLine = pLine;
	}

	if (!pTargetLine)
		return false;

	float x1 = pTargetLine->Get_Info().tLeftPos.fX;
	float y1 = pTargetLine->Get_Info().tLeftPos.fY;
	float x2 = pTargetLine->Get_Info().tRightPos.fX;
	float y2 = pTargetLine->Get_Info().tRightPos.fY;

	*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;

	return true;
}

void CLineMgr::Load_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_READ, NULL
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	LINEINFO	tTemp = {};

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_listLine.emplace_back(new CLine(tTemp.tLeftPos, tTemp.tRightPos));
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);
}
