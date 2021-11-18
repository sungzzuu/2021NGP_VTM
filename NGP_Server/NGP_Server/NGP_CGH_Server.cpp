#include "pch.h"


struct MyThread
{
    int iIndex = 0;
    SOCKET sock = 0;
};
struct Point
{
    float x = 0.f, y = 0.f;
};
HANDLE g_hClientEvent[4];
int g_iClientCount = 0; //접속한 클라 갯수
Point g_tPosition[4];

DWORD WINAPI ProcessClient(LPVOID arg);
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
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
int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0)
    {
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

//////////////
void WaitForClientEvent(int iCurIndex);


int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;


    //이벤트 그냥 미리 생성, 
    g_hClientEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hClientEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hClientEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_hClientEvent[3] = CreateEvent(NULL, FALSE, TRUE, NULL);



    MyThread tThread;
    tThread.iIndex = 0;
    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_display("accept()");
            break;
        }

        tThread.sock = client_sock;
        ++g_iClientCount;

        // 접속한 클라이언트 정보 출력
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // 스레드 생성
        hThread = CreateThread(NULL, 0, ProcessClient, &tThread, 0, NULL);

        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}




// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
    MyThread* pThread = (MyThread*)arg;
    SOCKET client_sock = (SOCKET)pThread->sock;
    int iCurIndex = pThread->iIndex - 1; //배열 인덱스로 사용해서 -1

    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1)
    {
        if (g_iClientCount >= 2)
            WaitForClientEvent(iCurIndex);


        // 데이터 받기
        retval = recvn(client_sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        // 데이터 보내기
        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("send()");
            break;
        }
    }

    --g_iClientCount;

    CloseHandle(g_hClientEvent[iCurIndex]);

    // closesocket()
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

void WaitForClientEvent(int iCurIndex)
{
    if (iCurIndex == 0 && g_hClientEvent[3])
        WaitForSingleObject(g_hClientEvent[3], INFINITE);

    else if (iCurIndex == 1 && g_hClientEvent[0])
        WaitForSingleObject(g_hClientEvent[0], INFINITE);

    else if (iCurIndex == 2 && g_hClientEvent[1])
        WaitForSingleObject(g_hClientEvent[1], INFINITE);

    else if (iCurIndex == 3 && g_hClientEvent[2])
        WaitForSingleObject(g_hClientEvent[2], INFINITE);

    //for (int i = 0; i < g_iClientCount; ++i)
    //{
    //    if (iCurIndex == i && g_hClientEvent[1])
    //        WaitForSingleObject(g_hClientEvent[1], INFINITE);
    //}
}
