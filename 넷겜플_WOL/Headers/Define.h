#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1280
#define WINCY 720

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr; }

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f

#define TILECX 145 //bmp크기
#define TILECY 145

#define TILE_RECTCX 50 //Rect크기
#define TILE_RECTCY 50

#define TILEX 30
#define TILEY 30

#define CHAR_CX 100
#define CHAR_CY 100

#define PLAYER_ATTACK 10

#endif // !__DEFINE_H__
