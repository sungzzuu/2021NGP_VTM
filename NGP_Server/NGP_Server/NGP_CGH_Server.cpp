#include "pch.h"


struct MyThread
{
    int iIndex = 0;
    SOCKET sock = 0;
};

HANDLE g_hClientEvent[4];
int g_iWaitClientIndex[4];
int g_iClientCount = 0; //������ Ŭ�� ����

CGameTimer m_GameTimer;

// ü�¾� ����
HpPotionInfo g_tHpPotionInfo;
float fPotionCreateTime = 0.f;
LONG iHpPotionIndex;

STORE_DATA g_tStoreData;
bool isGameStart = false;


DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

//�÷��̾� ����
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);

//�浹
void CheckCollision(int iIndex);
bool Check_Sphere(POS& tMePos, POS& tYouPos);
bool Check_Rect(POS& tMePos, POS& tYouPos, float* _x, float* _y);

// ü�¾� ����
void CreateHpPotion();
bool SendRecv_HpPotionInfo(SOCKET sock);

CRITICAL_SECTION g_csHpPotion;

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


int main(int argc, char* argv[])
{
    InitializeCriticalSection(&g_csHpPotion);

    srand(unsigned int(time(NULL)));

    // ServerMain ������
    CreateThread(NULL, 0, ServerMain, 0, 0, NULL);

    int retval;

    // ���� �ʱ�ȭ
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

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;


    //�̺�Ʈ ����
    //Ŭ��4�� �������϶� 0�� 3��, 1�� 0��, 2�� 1��, 3�� 2��, �̺�Ʈ ��ٸ�
    for (int i = 0; i < 4; ++i)
    {
        g_hClientEvent[i] = CreateEvent(NULL, FALSE, (i < 3 ? FALSE : TRUE), NULL);
        g_iWaitClientIndex[i] = (i == 0) ? 3 : i - 1; // 3 0 1 2
    }


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
        ++tThread.iIndex;

        // ������ Ŭ���̾�Ʈ ���� ���
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        // ������ ����
        hThread = CreateThread(NULL, 0, ProcessClient, &tThread, 0, NULL);

        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }
    }

    // closesocket()
    closesocket(listen_sock);

    DeleteCriticalSection(&g_csHpPotion);

    // ���� ����
    WSACleanup();
    return 0;
}




// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
    MyThread* pThread = (MyThread*)arg;
    SOCKET client_sock = (SOCKET)pThread->sock;
    int iCurIndex = pThread->iIndex - 1; //���� ������ �ε���, �迭 �ε����� ����ؼ� -1

    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1)
    {
        if (!isGameStart)
        {
            if (g_iClientCount < 4)
                continue;
            else
                isGameStart = true;
        }

        if (g_iClientCount >= 2)
            WaitForSingleObject(g_hClientEvent[g_iWaitClientIndex[iCurIndex]], INFINITE);


        //�÷��̾� ������ �ޱ�
        //////////////////////////////////////////////////////
        if (!SendRecv_PlayerInfo(client_sock, iCurIndex))
        {
            SetEvent(g_hClientEvent[iCurIndex]);
            break;
        }
        //////////////////////////////////////////////////////



        // �̽����尡 �����ٸ� FALSE �����ϹǷ�
        if (!SendRecv_HpPotionInfo(client_sock))
        {
            SetEvent(g_hClientEvent[iCurIndex]);
            break;
        }

        SetEvent(g_hClientEvent[iCurIndex]);
    }


    if (--g_iClientCount >= 2)
    {
        for (int i = 0; i < 4; ++i)
        {
            //�ڽ��� �����ϴ� Ŭ�� ã��
            if (g_iWaitClientIndex[i] == iCurIndex)
            {
                g_iWaitClientIndex[i] = g_iWaitClientIndex[iCurIndex]; //�ڽ��� �����ϰ��ִ� �ε����� �ٲ���
                g_iWaitClientIndex[iCurIndex] = -1;
                break;
            }
        }
    }


    CloseHandle(g_hClientEvent[iCurIndex]);

    // closesocket()
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

// ���� ���μ��� ����
DWORD WINAPI ServerMain(LPVOID arg)
{
    m_GameTimer.Reset();

    while (true)
    {
        // 1. ü�¾� �ð��缭 ������
        m_GameTimer.Tick(60.0f);
        CreateHpPotion();

    }
}

bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex)
{
    int retval, iCurIndex = iIndex;

    PLAYER_INFO tPlayerInfo;
    retval = recvn(client_sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;


    // ���� ������ ���
    //buf[retval] = '\0';
    //printf("[%d] (%f, %f)\n", iCurIndex, tPlayerInfo.tPos.fX, tPlayerInfo.tPos.fY);

    g_tStoreData.tPlayersPos[iCurIndex] = tPlayerInfo.tPos;
    g_tStoreData.iClientIndex = iCurIndex;

    //�浹
    CheckCollision(iCurIndex);


    // ������ ������
    retval = send(client_sock, (char*)&g_tStoreData, sizeof(STORE_DATA), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return FALSE;
    }

    return TRUE;
}

void CreateHpPotion()
{
    fPotionCreateTime += m_GameTimer.GetTimeElapsed();

    if (fPotionCreateTime >= POTION_TIME)
    {
        EnterCriticalSection(&g_csHpPotion);

        fPotionCreateTime = 0.f;
        g_tHpPotionInfo.thpPotionCreate.cnt = 0;
        g_tHpPotionInfo.thpPotionCreate.bCreateOn = true;
        g_tHpPotionInfo.thpPotionCreate.index = iHpPotionIndex++;
        g_tHpPotionInfo.thpPotionCreate.pos.fX = (rand() % 1000) + 50; // ���� �缳�� �ʿ�
        g_tHpPotionInfo.thpPotionCreate.pos.fY = (rand() % 500) + 50;  // ���� �缳�� �ʿ�
        //printf("���ǻ���\n");
        LeaveCriticalSection(&g_csHpPotion);

    }
}

bool SendRecv_HpPotionInfo(SOCKET sock)
{
    int retval;

    // ����ȭ ����
    // ���⼭ g_tHpPotionInfo�� �����ڿ�
    // ���� �ٸ� �����忡�� ���ÿ� �����ϹǷ� ��ü�� ����
    // Main�����嵵 ����ȭ�� �ؾ���

    EnterCriticalSection(&g_csHpPotion);

    // ü�¾� ���� ���� ������
    retval = send(sock, (char*)&g_tHpPotionInfo, sizeof(HpPotionInfo), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        LeaveCriticalSection(&g_csHpPotion);

        return FALSE;
    }

    // [ü�¾����] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
    if (g_tHpPotionInfo.thpPotionCreate.bCreateOn)
    {
        g_tHpPotionInfo.thpPotionCreate.cnt++;

        // ������ Ŭ�� ������ŭ ü�¾� ���� �������� �ٽ� 0���� ����
        if (g_tHpPotionInfo.thpPotionCreate.cnt == g_iClientCount)
        {
            ZeroMemory(&g_tHpPotionInfo.thpPotionCreate, sizeof(HpPotionCreate));
        }
    }

    // [ü�¾����] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
    if (g_tHpPotionInfo.thpPotionDelete.bDeleteOn)
    {
        g_tHpPotionInfo.thpPotionDelete.cnt++;

        // ������ Ŭ�� ������ŭ ü�¾� ���� �������� �ٽ� 0���� ����
        if (g_tHpPotionInfo.thpPotionDelete.cnt == g_iClientCount)
        {
            ZeroMemory(&g_tHpPotionInfo.thpPotionDelete, sizeof(HpPotionDelete));
        }
    }
    LeaveCriticalSection(&g_csHpPotion);


    // ü�¾� �浹 ���� �ޱ�
    POTIONRES tHpPotionRes;

    retval = recvn(sock, (char*)&tHpPotionRes, sizeof(POTIONRES), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    // �浹�� ��� ó�� - �ʿ��� ���� �� �ٸ� Ŭ�� �˸���
    if (tHpPotionRes.bCollision)
    {
        //printf("���ǻ���\n");

        // ���� Ŭ�� 1���� ���
        if (g_iClientCount == 1)
            return TRUE;

        g_tHpPotionInfo.thpPotionDelete.bDeleteOn = true;
        g_tHpPotionInfo.thpPotionDelete.cnt = 1;
        g_tHpPotionInfo.thpPotionDelete.index = tHpPotionRes.iIndex;


    }

    return TRUE;
}





void CheckCollision(int iIndex)
{
    int iCurIndex = iIndex;

    float x = 0.f, y = 0.f;

    for (int i = 0; i < 4/*g_iClientCount*/; ++i)
    {
        if (iCurIndex != i && g_tStoreData.tPlayersPos[iCurIndex].fX != 0.f)
        {
            //if (Check_Sphere(g_tStoreData.tPlayersPos[iCurIndex], g_tStoreData.tPlayersPos[i]))
            //{
            //    //�浹ü ����?
            //    printf("[%d] %d�� �浹\n", iCurIndex, i);
            //}

            //if (Check_Rect(g_tStoreData.tPlayersPos[iCurIndex], g_tStoreData.tPlayersPos[i], &x, &y))
            //{
            //    if (x > y)
            //    {
            //        if (g_tStoreData.tPlayersPos[iCurIndex].fY < g_tStoreData.tPlayersPos[i].fY)
            //            g_tStoreData.tPlayersPos[iCurIndex].fY -= y;
            //        else
            //            g_tStoreData.tPlayersPos[iCurIndex].fY += y;
            //    }
            //    else
            //    {
            //        if (g_tStoreData.tPlayersPos[iCurIndex].fX < g_tStoreData.tPlayersPos[i].fX)
            //            g_tStoreData.tPlayersPos[iCurIndex].fX -= x;
            //        else
            //            g_tStoreData.tPlayersPos[iCurIndex].fX += x;
            //    }
            //}
        }
    }
}

bool Check_Sphere(POS& tMePos, POS& tYouPos)
{
    //float fRadius = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCY) >> 1);
    float fRadius = 30;

    float fX = tMePos.fX - tYouPos.fX;
    float fY = tMePos.fY - tYouPos.fY;
    float fDis = sqrtf(fX * fX + fY * fY);

    return fRadius > fDis;
}

bool Check_Rect(POS& tMePos, POS& tYouPos, float* _x, float* _y)
{
    float fX = abs(tMePos.fX - tYouPos.fX);
    float fY = abs(tMePos.fY - tYouPos.fY);

    //float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
    //float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

    //�÷��̾� 30,80
    float fCX = 30.f;
    float fCY = 80.f;

    if (fCX > fX && fCY > fY)
    {
        *_x = fCX - fX;
        *_y = fCY - fY;
        return true;
    }

    return false;
}
