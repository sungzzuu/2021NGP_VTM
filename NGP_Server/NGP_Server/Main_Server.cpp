#include "pch.h"
#include <iostream>

struct MyThread
{
    int iIndex = 0;
    SOCKET sock = 0;
};

HANDLE g_hClientEvent[4];
int g_iWaitClientIndex[4];
int g_iClientCount = 0; //접속한 클라 갯수

CGameTimer m_GameTimer;

// 체력약 관련
HpPotionInfo g_tHpPotionInfo;
float fPotionCreateTime = 0.f;
LONG iHpPotionIndex;

// 게임시작 관련
PLAYER_INIT g_PlayerInit;
float fStartTime = 0.f;


STORE_DATA g_tStoreData;

bool isGameStart = false;

// 공격 관련
AttackData g_pAttackData[4];

DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

//플레이어 관련
bool SendPlayerInit(SOCKET sock, int PlayerIndex);  // 게임시작여부 보내기
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);


// 체력약 관련
void CreateHpPotion();
bool SendRecv_HpPotionInfo(SOCKET sock);
bool SendRecv_AttackInfo(SOCKET sock, int clientIndex);

void CountStart();
void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend);

CRITICAL_SECTION g_csHpPotion;

//충돌
void CheckCollision(int iIndex);
bool Check_Sphere_PlayerSkill(POS& tMePos, INFO& tYouPos);
bool Check_Sphere_SkillPlayer(INFO& tMePos, POS& tYouPos);

bool Check_Sphere(POS& tMePos, POS& tYouPos);
bool Check_Rect(POS& tMePos, POS& tYouPos, float* _x, float* _y);
bool g_isHit[4] = { false };

// 엔딩
bool g_bEnding = false;
void CheckEnding(int iCurIndex);


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

    // ServerMain 스레드
    CreateThread(NULL, 0, ServerMain, 0, 0, NULL);

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


    //이벤트 생성
    //클라4개 접속중일때 0이 3번, 1이 0번, 2가 1번, 3이 2번, 이벤트 기다림
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

        int nagleopt = TRUE;
        setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nagleopt, sizeof(nagleopt));

        tThread.sock = client_sock;
        ++g_iClientCount;
        ++tThread.iIndex;

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

    DeleteCriticalSection(&g_csHpPotion);

    // 윈속 종료
    WSACleanup();
    return 0;
}




// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
    MyThread* pThread = (MyThread*)arg;
    SOCKET client_sock = (SOCKET)pThread->sock;
    int iCurIndex = pThread->iIndex - 1; //현재 쓰레드 인덱스, 배열 인덱스로 사용해서 -1

    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // 클라이언트 정보 얻기
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

        // 타이머, 플레이어 배치, 팀 나누기
        if (!SendPlayerInit(client_sock, iCurIndex))
        {
            SetEvent(g_hClientEvent[iCurIndex]);
            break;
        }

        //플레이어 데이터 받기
        //////////////////////////////////////////////////////
        if (!SendRecv_PlayerInfo(client_sock, iCurIndex))
        {
            SetEvent(g_hClientEvent[iCurIndex]);
            break;
        }
        //////////////////////////////////////////////////////



        // 체력약
        if (!SendRecv_HpPotionInfo(client_sock))
        {
            SetEvent(g_hClientEvent[iCurIndex]);
            break;
        }

        // 공격
        if (!SendRecv_AttackInfo(client_sock, iCurIndex))
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
            //자신을 참조하던 클라를 찾음
            if (g_iWaitClientIndex[i] == iCurIndex)
            {
                g_iWaitClientIndex[i] = g_iWaitClientIndex[iCurIndex]; //자신이 참조하고있던 인덱스로 바꿔줌
                g_iWaitClientIndex[iCurIndex] = -1;
                break;
            }
        }
    }


    CloseHandle(g_hClientEvent[iCurIndex]);

    // closesocket()
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;

}

// 서버 프로세스 구현
DWORD WINAPI ServerMain(LPVOID arg)
{
    m_GameTimer.Reset();

    while (true)
    {
        // 1. 체력약 시간재서 보내기
        if (g_iClientCount == 4)   // 클라 4명이면 스타트
        {
            m_GameTimer.Tick(60.0f);
            if (!g_PlayerInit.start)
                CountStart();
            CreateHpPotion();
        }

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


    // 받은 데이터 출력
    //buf[retval] = '\0';
    //printf("[%d] (%f, %f)\n", iCurIndex, tPlayerInfo.tPos.fX, tPlayerInfo.tPos.fY);
    if (g_iClientCount == 4)    // 클라 4명이면 스타트
    {
        tPlayerInfo.start = true;
    }
    // 엔딩 변수 저장
    ENDING::END_TYPE eEnding = ENDING::ING;
    if (g_bEnding)
    {
        eEnding = g_tStoreData.tPlayersInfo[iCurIndex].eEnding;
    }

    g_tStoreData.tPlayersInfo[iCurIndex] = tPlayerInfo;
    g_tStoreData.iClientIndex = iCurIndex;

    // 엔딩 변수 설정
    g_tStoreData.tPlayersInfo[iCurIndex].eEnding = eEnding;

    g_tStoreData.iHp[iCurIndex] = tPlayerInfo.iHp;
    g_tStoreData.start = tPlayerInfo.start;
    if (iCurIndex == 1 || iCurIndex == 3) { g_tStoreData.team[iCurIndex] = TEAMNUM::TEAM1; }
    else { g_tStoreData.team[iCurIndex] = TEAMNUM::TEAM2; }

    g_tStoreData.tPlayersInfo[iCurIndex].isHit = g_isHit[iCurIndex];
    if (g_isHit[iCurIndex])
        g_isHit[iCurIndex] = false;

    // 엔딩 판정
    CheckEnding(iCurIndex);

    // 데이터 보내기
    retval = send(client_sock, (char*)&g_tStoreData, sizeof(STORE_DATA), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return FALSE;
    }

    return TRUE;
}

void CheckEnding(int iCurIndex)
{
    if (g_bEnding)
        return;

    if (g_tStoreData.tPlayersInfo[iCurIndex].isDead)
    {
        TEAMNUM::TEAM eNowTeam = g_tStoreData.tPlayersInfo[iCurIndex].team;

        for (int i = 0; i < 4; i++)
        {
            if (i == iCurIndex)
                continue;

            if (g_tStoreData.tPlayersInfo[i].team == eNowTeam && g_tStoreData.tPlayersInfo[i].isDead)
            {
                g_bEnding = true;
                g_tStoreData.tPlayersInfo[iCurIndex].eEnding = ENDING::LOSE;
                g_tStoreData.tPlayersInfo[i].eEnding = ENDING::LOSE;
                for (int j = 0; j < 4; j++)
                {
                    if (j == i || j == iCurIndex)
                        continue;
                    g_tStoreData.tPlayersInfo[j].eEnding = ENDING::WIN;

                }
                break;
            }
        }

    }
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
        g_tHpPotionInfo.thpPotionCreate.pos.fX = (rand() % 1000) + 50; // 범위 재설정 필요
        g_tHpPotionInfo.thpPotionCreate.pos.fY = (rand() % 500) + 50;  // 범위 재설정 필요
        //printf("포션생성\n");
        LeaveCriticalSection(&g_csHpPotion);

    }

}

bool SendRecv_HpPotionInfo(SOCKET sock)
{
    int retval;

    // 동기화 오류
    // 여기서 g_tHpPotionInfo는 공유자원
    // 서로 다른 스레드에서 동시에 접근하므로 객체가 변함
    // Main스레드도 동기화를 해야함

    EnterCriticalSection(&g_csHpPotion);

    // 체력약 생성 정보 보내기
    retval = send(sock, (char*)&g_tHpPotionInfo, sizeof(HpPotionInfo), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        LeaveCriticalSection(&g_csHpPotion);

        return FALSE;
    }

    // [체력약생성] 현재접속된 모든 클라에 보냈으면 변수 초기화
    if (g_tHpPotionInfo.thpPotionCreate.bCreateOn)
    {
        g_tHpPotionInfo.thpPotionCreate.cnt++;

        // 접속한 클라에 개수만큼 체력약 정보 보냈으면 다시 0으로 리셋
        if (g_tHpPotionInfo.thpPotionCreate.cnt == g_iClientCount)
        {
            ZeroMemory(&g_tHpPotionInfo.thpPotionCreate, sizeof(HpPotionCreate));
        }
    }

    // [체력약삭제] 현재접속된 모든 클라에 보냈으면 변수 초기화
    if (g_tHpPotionInfo.thpPotionDelete.bDeleteOn)
    {
        g_tHpPotionInfo.thpPotionDelete.cnt++;

        // 접속한 클라에 개수만큼 체력약 정보 보냈으면 다시 0으로 리셋
        if (g_tHpPotionInfo.thpPotionDelete.cnt == g_iClientCount)
        {
            ZeroMemory(&g_tHpPotionInfo.thpPotionDelete, sizeof(HpPotionDelete));
        }
    }
    LeaveCriticalSection(&g_csHpPotion);


    // 체력약 충돌 정보 받기
    POTIONRES tHpPotionRes;

    retval = recvn(sock, (char*)&tHpPotionRes, sizeof(POTIONRES), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    // 충돌일 경우 처리 - 맵에서 삭제 및 다른 클라에 알리기
    if (tHpPotionRes.bCollision)
    {
        //printf("포션삭제\n");

        // 접속 클라 1개인 경우
        if (g_iClientCount == 1)
            return TRUE;

        g_tHpPotionInfo.thpPotionDelete.bDeleteOn = true;
        g_tHpPotionInfo.thpPotionDelete.cnt = 1;
        g_tHpPotionInfo.thpPotionDelete.index = tHpPotionRes.iIndex;


    }

    return TRUE;
}

bool SendRecv_AttackInfo(SOCKET sock, int clientIndex)
{
    int retval;

    // 공격 정보 받기 - 1. 벡터의 크기
    int iSize = 0;
    retval = recvn(sock, (char*)&iSize, sizeof(int), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    //if(iSize != 0)
    //    printf("vec: %d\n", iSize);

    g_pAttackData[clientIndex].iSize = iSize;

    if (iSize != 0)
    {
        // 동적배열 초기화
        delete[] g_pAttackData[clientIndex].pAttackInfo;
        g_pAttackData[clientIndex].pAttackInfo = new ATTACKINFO[iSize];

        // 공격 정보 받기 - 2. 벡터
        retval = recvn(sock, (char*)g_pAttackData[clientIndex].pAttackInfo, iSize * sizeof(ATTACKINFO), 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("recv()");
            return FALSE;
        }
        else if (retval == 0)
            return FALSE;

        //for (int i = 0; i < iSize; ++i)
        //{
        //    printf("vec.front(): %d\n", g_pAttackData[clientIndex].pAttackInfo[i].iType);
        //}
    }

    ////////////////////////////////////////////////////////////////////
    //충돌체크
    CheckCollision(clientIndex);
    ////////////////////////////////////////////////////////////////////


    for (int i = 0; i < 4; i++)
    {
        //if (i == clientIndex)
        //    continue;

        // 공격 정보 보내기 - 1. 배열의 크기
        retval = send(sock, (char*)&g_pAttackData[i].iSize, sizeof(int), 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("recv()");
            return FALSE;
        }
        iSize = g_pAttackData[i].iSize;

        if (iSize == 0)
            continue;

        // 공격 정보 보내기 - 2. 배열
        retval = send(sock, (char*)g_pAttackData[i].pAttackInfo, iSize * sizeof(ATTACKINFO), 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("recv()");
            return FALSE;
        }
    }

    return TRUE;

}

bool SendPlayerInit(SOCKET sock, int PlayerIndex)
{
    int retval;

    g_PlayerInit.iCount = (int)fStartTime;
    PLAYER_INIT_SEND tPlayerInitSend;
    tPlayerInitSend.start = g_PlayerInit.start;
    tPlayerInitSend.idx = PlayerIndex;
    tPlayerInitSend.iCount = g_PlayerInit.iCount;

    if (g_PlayerInit.start)
    {
        Get_InitPos(PlayerIndex, tPlayerInitSend);
    }
    retval = send(sock, (char*)&tPlayerInitSend, sizeof(PLAYER_INIT_SEND), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return FALSE;
    }
    return TRUE;
}

void CountStart()
{
    fStartTime += m_GameTimer.GetTimeElapsed();  // 이전 프레임에서 현재 프레임까지 시간 
    //std::cout << 6 - (int)fStartTime << std::endl;
    if (fStartTime >= START_TIME)
    {
        g_PlayerInit.start = true;
    }
}

void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend)
{
    switch (idx)
    {
    case 0:
        tPlayerInitSend.tPos = { 300.f, 300.f };
        break;
    case 1:
        tPlayerInitSend.tPos = { 900.f, 300.f };
        break;
    case 2:
        tPlayerInitSend.tPos = { 300.f, 600.f };
        break;
    case 3:
        tPlayerInitSend.tPos = { 900.f, 600.f };
        break;
    default:
        break;
    }

    for (int i = 0; i < 4; ++i)
    {
        if (i % 2)
            tPlayerInitSend.team[i] = TEAMNUM::TEAM1;
        else
            tPlayerInitSend.team[i] = TEAMNUM::TEAM2;
    }
}

void CheckCollision(int iIndex)
{
    int iCurIndex = iIndex;

    float x = 0.f, y = 0.f;

    for (int i = 0; i < 4/*g_iClientCount*/; ++i)
    {
        //자기 자신 검사x
        if (iCurIndex == i)
            continue;

        //죽었으면 충돌x
        if (g_tStoreData.tPlayersInfo[i].isDead)
            continue;

        //같은팀이면 충돌x
        if (g_tStoreData.team[iCurIndex] == g_tStoreData.team[i])
            continue;

        for (int j = 0; j < g_pAttackData[iCurIndex].iSize; ++j) //본인의 스킬 갯수
        {
            if (Check_Sphere_SkillPlayer(g_pAttackData[iCurIndex].pAttackInfo[j].tInfo, g_tStoreData.tPlayersInfo[i].tPos))
            {
                g_pAttackData[iCurIndex].pAttackInfo[j].bCollision = true;
                g_isHit[i] = true;
            }
        }
    }
}

bool Check_Sphere(INFO& tMePos, INFO& tYouPos)
{
    float fRadius = (float)((tMePos.iCX + tYouPos.iCY) >> 1);
    //float fRadius = 30;

    float fX = tMePos.fX - tYouPos.fX;
    float fY = tMePos.fY - tYouPos.fY;
    float fDis = sqrtf(fX * fX + fY * fY);

    return fRadius > fDis;
}

bool Check_Sphere_PlayerSkill(POS& tMePos, INFO& tYouPos)
{
    float fRadius = (float)((30 + tYouPos.iCY) >> 1);

    float fX = tMePos.fX - tYouPos.fX;
    float fY = tMePos.fY - tYouPos.fY;
    float fDis = sqrtf(fX * fX + fY * fY);

    return fRadius > fDis;
}

bool Check_Sphere_SkillPlayer(INFO& tMePos, POS& tYouPos)
{
    float fRadius = (float)((tMePos.iCY + 80) >> 1);

    float fX = tMePos.fX - tYouPos.fX;
    float fY = tMePos.fY - tYouPos.fY;
    float fDis = sqrtf(fX * fX + fY * fY);

    return fRadius > fDis;
}

bool Check_Rect(INFO& tMePos, INFO& tYouPos, float* _x, float* _y)
{
    float fX = abs(tMePos.fX - tYouPos.fX);
    float fY = abs(tMePos.fY - tYouPos.fY);

    //float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
    //float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

    //플레이어 30,80
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
