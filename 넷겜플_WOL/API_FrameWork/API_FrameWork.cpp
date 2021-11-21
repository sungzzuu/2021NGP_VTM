// API_FrameWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "API_FrameWork.h"

#include "MainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HWND	g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// 서버 관련 함수
void err_display(char* msg);
void err_quit(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI ServerProcess(LPVOID arg);
bool RecvHpPotionInfo(SOCKET sock);

// 서버 관련 변수
HANDLE hServerProcess;
char SERVERIP[512] = "127.0.0.1";

// 체력약 관련 변수
POTIONRES g_tHpPotionInfo;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(unsigned int(time(NULL)));

    // TODO: 여기에 코드를 입력합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_API_FRAMEWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_API_FRAMEWORK));

    MSG msg;
	msg.message = WM_NULL;

	CMainGame mainGame;
	mainGame.Initialize();

	// 서버 전송 부분 생성. 이벤트 생성해서 클라 Late_Update()까지 끝나면 send 하도록
	hServerProcess = CreateThread(NULL, 0, ServerProcess, NULL, 0, NULL);

	DWORD	dwTime1 = GetTickCount();

    while (WM_QUIT != msg.message)
    {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (dwTime1 + 10 < GetTickCount())
		{
			mainGame.Update();
			mainGame.Late_Update();

			// 서버 통신 진행

			mainGame.Render();

			dwTime1 = GetTickCount();
		}

    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_API_FRAMEWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc = { 0, 0, WINCX, WINCY };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0
	   , rc.right - rc.left, rc.bottom - rc.top
	   , nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

DWORD WINAPI ServerProcess(LPVOID arg)
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        err_quit("socket()");

    // connet()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
        err_quit("connect()");

    char str[] = "클라에서 왔다.";
    char buf[BUFSIZ + 1];

    int len = strlen(str);
    while (true)
    {
        // 이곳에 각각의 함수 추가! 주고 받는 것
        retval = RecvHpPotionInfo(sock);

        if (retval == FALSE)
            break;
    }


    //// 데이터 받기
    //retval = recvn(sock, (char*)&len, sizeof(int), 0);
    //if (retval == SOCKET_ERROR) {
    //    err_display("recv()");
    //    break;
    //}
    //else if (retval == 0)
    //    break;
    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
}

bool RecvHpPotionInfo(SOCKET sock)
{
    int retval;

    // 체력약 정보받기 계속 정보를 받긴하지만, 생성되었을때, 아닐때가 다름
    HpPotionInfo tHpPotionInfo;
    retval = recvn(sock, (char*)&tHpPotionInfo, sizeof(HpPotionInfo), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    if (tHpPotionInfo.bCreateOn)
    {
        printf("체력약 생성\n");
    }
    // 체력약 충돌 정보 보내기
    
    retval = send(sock, (char*)&g_tHpPotionInfo, sizeof(POTIONRES), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    return TRUE;

    
}
