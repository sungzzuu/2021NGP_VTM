#include "stdafx.h"
#include "DataMgr.h"


CDataMgr* CDataMgr::m_pInstance = nullptr;
CDataMgr::CDataMgr()
{
}

CDataMgr::~CDataMgr()
{
}

TCHAR* CDataMgr::GetFrameKey(int iFrameKey)
{
	switch (iFrameKey)
	{
	case 1:
		return L"Player_UP";
	case 2:
		return L"Player_DOWN";
	case 3:
		return L"Player_LEFT";
	case 4:
		return L"Player_RIGHT";
	default:
		break;
	}
	return nullptr;
}

int CDataMgr::SetFrameKey(TCHAR* pFrameKey)
{
	if (pFrameKey==L"Player_UP")
	{
		return 1;
	}
	else if (pFrameKey == L"Player_DOWN")
	{
		return 2;

	}
	else if (pFrameKey == L"Player_LEFT")
	{
		return 3;

	}
	else if (pFrameKey == L"Player_RIGHT")
	{
		return 4;

	}

	return 0;
}

