#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1280
#define WINCY 720
//#define WINCX 800
//#define WINCY 600


#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr; }

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f

#define TILECX 145 //bmpũ??
#define TILECY 145

#define TILE_RECTCX 50 //Rectũ??
#define TILE_RECTCY 50

#define TILEX 30
#define TILEY 30

#define CHAR_CX 100
#define CHAR_CY 100

#define PLAYER_ATTACK 10

#define SERVERPORT 9000

#define MAP_EDGE 50
#define	REVIVE_TIME 10000
#endif // !__DEFINE_H__
