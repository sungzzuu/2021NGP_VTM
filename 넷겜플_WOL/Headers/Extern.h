#pragma once

#ifndef __EXTERN_H__
#define __EXTERN_H__

extern HWND g_hWnd;
extern POTIONRES g_tHpPotionRes;
extern HANDLE hServerProcess;
extern DWORD WINAPI ServerProcess(LPVOID arg);
#endif // !__EXTERN_H__
