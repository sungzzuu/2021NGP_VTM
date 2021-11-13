#include "stdafx.h"
#include "MyEdit.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"


CMyEdit::CMyEdit()
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/WOL_TILE_DUNGEON.bmp", L"WOL_TILE_DUNGEON");

	CTileMgr::Get_Instance()->Initialize();
}

void CMyEdit::Update()
{
	Key_Check();
}

void CMyEdit::Late_Update()
{
}

void CMyEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(0, 0, 0); //x y option
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(7, 0, 0);

	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
		CTileMgr::Get_Instance()->Picking_Tile(3, 0, 0);
	if (CKeyMgr::Get_Instance()->Key_Down('X'))
		CTileMgr::Get_Instance()->Picking_Tile(3, 2, 1);

	//876  435
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
		CTileMgr::Get_Instance()->Picking_Tile(5, 3, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('V'))
		CTileMgr::Get_Instance()->Picking_Tile(6, 3, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('B'))
		CTileMgr::Get_Instance()->Picking_Tile(7, 3, 1);

	if (CKeyMgr::Get_Instance()->Key_Down('F'))
		CTileMgr::Get_Instance()->Picking_Tile(5, 4, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('G'))
		CTileMgr::Get_Instance()->Picking_Tile(6, 4, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('H'))
		CTileMgr::Get_Instance()->Picking_Tile(7, 4, 1);

	if (CKeyMgr::Get_Instance()->Key_Down('R'))
		CTileMgr::Get_Instance()->Picking_Tile(5, 5, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('T'))
		CTileMgr::Get_Instance()->Picking_Tile(6, 5, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('Y'))
		CTileMgr::Get_Instance()->Picking_Tile(7, 5, 1);

	if (CKeyMgr::Get_Instance()->Key_Down('4'))
		CTileMgr::Get_Instance()->Picking_Tile(5, 6, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
		CTileMgr::Get_Instance()->Picking_Tile(6, 6, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('6'))
		CTileMgr::Get_Instance()->Picking_Tile(7, 6, 1);

	if (CKeyMgr::Get_Instance()->Key_Down('N'))
		CTileMgr::Get_Instance()->Picking_Tile(5, 7, 0);
	if (CKeyMgr::Get_Instance()->Key_Down('M'))
		CTileMgr::Get_Instance()->Picking_Tile(6, 7, 0);
	if (CKeyMgr::Get_Instance()->Key_Down('K'))
		CTileMgr::Get_Instance()->Picking_Tile(7, 7, 0);
	if (CKeyMgr::Get_Instance()->Key_Down('J'))
		CTileMgr::Get_Instance()->Picking_Tile(8, 7, 0);

	if (CKeyMgr::Get_Instance()->Key_Down('U'))
		CTileMgr::Get_Instance()->Picking_Tile(3, 4, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('I'))
		CTileMgr::Get_Instance()->Picking_Tile(4, 4, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('7'))
		CTileMgr::Get_Instance()->Picking_Tile(3, 5, 1);
	if (CKeyMgr::Get_Instance()->Key_Down('8'))
		CTileMgr::Get_Instance()->Picking_Tile(4, 5, 1);

	if (CKeyMgr::Get_Instance()->Key_Down('P'))
		CTileMgr::Get_Instance()->Save_Tile();
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
		CTileMgr::Get_Instance()->Load_Tile();

}
