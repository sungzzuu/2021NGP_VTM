// API_FrameWork.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "API_FrameWork.h"

#include "MainGame.h"
#include "Potion.h"
#include "ObjMgr.h"
#include "MyButton.h"
#include "DataMgr.h"


#define MAX_LOADSTRING 100

// ���� ����:
HWND	g_hWnd;
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// ���� ���� �Լ�
void err_display(char* msg);
void err_quit(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI ServerProcess(LPVOID arg);

bool SendRecvPlayerInfo(SOCKET sock);

bool SendRecvHpPotionInfo(SOCKET sock);
bool SendRecvAttacks(SOCKET sock);

bool RecvPlayerInit(SOCKET sock);

// ���� ���� ����
HANDLE hServerProcess;

HANDLE hGameEvent;
HANDLE hSocketEvent;

char SERVERIP[512] = /*"172.30.1.46"*//*"192.168.0.134"*//*"192.168.120.31"*/ /*"172.20.10.9"*/ "127.0.0.1";


// ü�¾� ���� ����, �Լ�
POTIONRES g_tHpPotionRes;
void Add_Potion(HpPotionCreate);
void Delete_Potion(HpPotionDelete hpPotionDelete);

PLAYER_INIT_SEND g_tPlayerInit;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	srand(unsigned int(time(NULL)));

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_API_FRAMEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_API_FRAMEWORK));

	MSG msg;
	msg.message = WM_NULL;

	CMainGame mainGame;
	mainGame.Initialize();

	CMyButton button;
	// ���� ���� �κ� ����. �̺�Ʈ �����ؼ� Ŭ�� Late_Update()���� ������ send �ϵ���


	//hServerProcess = CreateThread(NULL, 0, ServerProcess, NULL, 0, NULL);


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
			WaitForSingleObject(hGameEvent, INFINITE);

			mainGame.Update();
			mainGame.Late_Update();

			// ���� ��� ����

			mainGame.Render();

			SetEvent(hSocketEvent);


			dwTime1 = GetTickCount();
		}

	}

	CDataMgr::Get_Instance()->Destroy_Instance();

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_API_FRAMEWORK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
	//���� ���� ����
	hGameEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	int retval;

	// ���� �ʱ�ȭ
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

	int nagleopt = TRUE;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nagleopt, sizeof(nagleopt));



	while (1)
	{
		WaitForSingleObject(hSocketEvent, INFINITE);

		// �̰��� ������ �Լ� �߰�! �ְ� �޴� ��
		//////////////////////////////////////////////////////////

		retval = RecvPlayerInit(sock);
		if (retval == FALSE)
			break;

		retval = SendRecvPlayerInfo(sock);
		if (retval == FALSE)
			break;

		// ü�¾�
		retval = SendRecvHpPotionInfo(sock);
		if (retval == FALSE)
			break;

		// ����
		retval = SendRecvAttacks(sock);
		if (retval == FALSE)
			break;
		
		//////////////////////////////////////////////////////////

		SetEvent(hGameEvent);
	}

	SetEvent(hGameEvent);

	closesocket(sock);

	// ���� ����
	WSACleanup();

}

bool SendRecvPlayerInfo(SOCKET sock)
{
	int retval;

	//�ڱ� ��ǥ ������
	PLAYER_INFO tPlayerInfo = CDataMgr::Get_Instance()->m_tPlayerInfo;
	retval = send(sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	//��� ��ǥ �ޱ�
	retval = recvn(sock, (char*)&(CDataMgr::Get_Instance()->m_tStoreData), sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;


	//�浹�� ��ǥ�� ����
	//Player Render ����



	return TRUE;
}

bool SendRecvHpPotionInfo(SOCKET sock)
{
	int retval;

	// ü�¾� �����ޱ� ����&����
	HpPotionInfo tHpPotionInfo;
	retval = recvn(sock, (char*)&tHpPotionInfo, sizeof(HpPotionInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// ü�¾� ����
	if (tHpPotionInfo.thpPotionCreate.bCreateOn)
	{
		Add_Potion(tHpPotionInfo.thpPotionCreate);
		printf("ü�¾� ����\n");
	}

	// ü�¾� ����
	if (tHpPotionInfo.thpPotionDelete.bDeleteOn)
	{
		Delete_Potion(tHpPotionInfo.thpPotionDelete);
		printf("ü�¾� ������(�ٸ� Ŭ�� ����)\n");
	}

	// ü�¾� �浹 ���� ������
	retval = send(sock, (char*)&g_tHpPotionRes, sizeof(POTIONRES), 0); // ���̰� ������ ���� �ƴ� ���������� len
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	ZeroMemory(&g_tHpPotionRes, sizeof(POTIONRES));

	return TRUE;
}

bool SendRecvAttacks(SOCKET sock)
{
	int retval;

	// ���� ���� ������ - 1. ������ ũ��
	CDataMgr::Get_Instance()->SetAttackArr();
	ATTACKINFO* pAttackInfo = CDataMgr::Get_Instance()->m_pAttackData.pAttackInfo;
	int iSize = CDataMgr::Get_Instance()->m_pAttackData.iSize;

	retval = send(sock, (char*)&iSize, sizeof(int), 0); // ���̰� ������ ���� �ƴ� ���������� len
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	if (iSize != 0)
	{
		retval = send(sock, (char*)pAttackInfo, iSize * sizeof(ATTACKINFO), 0); // ���̰� ������ ���� �ƴ� ���������� len
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			return 0;
		}
		for (int i = 0; i < iSize; ++i)
		{
			//printf("vec.front(): %d\n", pAttackInfo[i].iType);
		}
		printf("\n");
	}



	for (int i = 0; i < 3; i++)
	{
		iSize = 0;
		// ���� ���� �ޱ� - 1. �迭�� ũ��
		retval = recvn(sock, (char*)&iSize, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			return FALSE;
		}
		else if (retval == 0)
			return FALSE;

		CDataMgr::Get_Instance()->m_pOthersAttackData[i].iSize = iSize;

		if (iSize == 0)
			continue;

		// ���� ���� �ޱ� - 2. �迭
		if (CDataMgr::Get_Instance()->m_pOthersAttackData[i].pAttackInfo != nullptr)
			delete[] CDataMgr::Get_Instance()->m_pOthersAttackData[i].pAttackInfo;
		CDataMgr::Get_Instance()->m_pOthersAttackData[i].pAttackInfo = new ATTACKINFO[iSize];

		retval = recvn(sock, (char*)CDataMgr::Get_Instance()->m_pOthersAttackData[i].pAttackInfo, iSize * sizeof(ATTACKINFO), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			return FALSE;
		}
		else if (retval == 0)
			return FALSE;

		for (int j = 0; j < iSize; ++j)
		{
			//printf("other_vec.front(): %d\n",
				//CDataMgr::Get_Instance()->m_pOthersAttackData[i].pAttackInfo[j].iType);
		}

	}

	return TRUE;
}

void Add_Potion(HpPotionCreate hpPotionCreate)
{
	CObj* pObj1 = CAbstractFactory<CPotion>::Create();
	pObj1->Set_Pos(hpPotionCreate.pos.fX, hpPotionCreate.pos.fY);
	dynamic_cast<CPotion*>(pObj1)->SetIndex(hpPotionCreate.index);
	CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj1);
}

void Delete_Potion(HpPotionDelete hpPotionDelete)
{
	// index ��ġ�ϴ� ü�¾� ã�Ƽ� �����ϱ�
	CObjMgr::Get_Instance()->Delete_Potion(hpPotionDelete.index);
}


bool RecvPlayerInit(SOCKET sock)
{
	int retval;

	retval = recvn(sock, (char*)&g_tPlayerInit, sizeof(PLAYER_INIT_SEND), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	//buf[retval] = '\0';
	//printf("(%f, %f)\n", g_tPlayerInit.tPos.fX, g_tPlayerInit.tPos.fY);
	//std::cout<< CDataMgr::Get_Instance()->m_tStoreData.team[1] << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		std::cout << g_tPlayerInit.team[i] << std::endl;
	}

}