#include <iostream>
#include <termios.h> //터미널 I/O(입출력) 설정을 제어하는 헤더 파일
#include <unistd.h>  //기본적인 시스템 레벨의 입출력 작업, 프로세스 제어, 파일 관리 등을 위한 여러 함수들을 제공
using namespace std;

// adws로 입력받기
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

// 전역변수들 정의
int score = 0;              // Score
int targetNum = 10000;      // 상한선 초기화
int numCell = 10;           // 최대 보드판의 가로 세로 길이 
int board[10][10] = {};     // 게임판 초기화
int *p0[10 * 10] = {0};     // 포인터 배열 초기화

// 동적 배열 생성 및 해제 함수 
int** CreateBoard(int size);             // 동적 2D 배열 메모리 생성 함수 (게임판 생성)
void DeleteBoard(int** board, int size); // 동적 2D 배열 메모리 해제 함수
int** CreatePointerArray(int size); // 동적 1D 포인터 배열 생성 함수 (포인터 배열 생성)
void DeletePointerArray(int** p0); // 동적 1D 포인터 배열 메모리 해제 함수

// 공용 함수 선언
void ClearScreen(); // ANSI 기반 화면 관리
void DisplayMenu(const string modes[], int modeCount, int selected); // 메뉴 표시 함수
char GetInput(); // player 입력 함수
void NewNum(int numCell); // 세부 기능 2 (랜덤 2의배수 숫자 생성)
void Draw(int targetNum,int numCell); // 세부 기능 1 (3x3 게임판 만들기)
void CheckGameOver(int numCell); // 세부 기능 6 (게임 승리)

// mode 전용 함수
void NewNumOrItem(int numCell); // mode 2 전용 랜덤 생성 함수
void HandleItem(int &current, int &next, int i, int j, int dx, int dy, int act); // mode 2 Item effect
void LevelCheckGameOver(int targetNum, int numCell); // mode 3 전용 게임 종료 여부

// 게임 모드 3 가지 
void Run2048(int numCell); // mode 1 (NxN 2048 game)
void RunItemMode(); // mode 2 (Item mode)
void LevelSelect2048(int targetNum); //mode 3 (Level challenge)

// 동적 2D 배열 메모리 생성 함수
int** CreateBoard(int size)
{
    int** board = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        board[i] = new int[size]{0};
    }
    return board;
}

// 동적 2D 배열 메모리 해제 함수
void DeleteBoard(int** board, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete[] board[i];
    }
    delete[] board;
}

// 동적 1D 포인터 배열 생성 함수
int** CreatePointerArray(int size)
{
    int** p0 = new int*[size * size]{nullptr};
    return p0;
}

// 동적 1D 포인터 배열 메모리 해제 함수
void DeletePointerArray(int** p0)
{
    delete[] p0;
}

// ANSI 기반 화면 관리
void ClearScreen()
{
    cout << "\033[2J\033[1;1H";
}

// 메뉴 표시 함수
void DisplayMenu(const string modes[], int modeCount, int selected)
{
    ClearScreen();
    cout << "==========================" << endl;
    cout << "        게임 모드           " << endl;
    cout << "==========================" << endl;
    for (int i = 0; i < modeCount; ++i)
    {
        if (i == selected)
        {
            cout << "> \033[1;32m" << modes[i] << "\033[0m" << endl;
        }
        else
        {
            cout << "  " << modes[i] << endl;
        }
    }
    cout << "==========================" << endl;
    cout << "W: 위로, S: 아래로, 스페이스: 선택" << endl;
}

// 단일 키 입력 받기 (getch 대체)
char GetInput()
{
    char buf = 0;
    struct termios oldAttr, newAttr;
    tcgetattr(STDIN_FILENO, &oldAttr);
    newAttr = oldAttr;
    newAttr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
    buf = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
    return buf;
}

void NewNum(int numCell)
{
    int i, j, cnt = 0;
    //int *p0[numCell * numCell] = {0}; // 포인터 배열 p0를 선언
    for (i = 0; i < numCell; i++)
    {
        for (j = 0; j < numCell; j++)
        {
            if (board[i][j] == 0)
            {
                p0[cnt] = &board[i][j]; // board[i][j]가 0인 경우, 해당 칸의 주소를 p0 배열에 저장
                cnt++;                  // cnt를 1 증가하여 다음 빈 칸의 위치를 가리킴
            }
        }
    }

    *p0[rand() % cnt] = (rand() % 100 < 80) ? 2 : 4;
    // p0 배열에 저장된 빈 칸 중에서 무작위로 하나를 선택하여 새로운 숫자를 생성
    // rand() % 100 < 80 조건을 사용해 80% 확률로 2를, 20% 확률로 4를 생성하여 선택된 빈 칸에 대입
}

void Draw(int targetNum,int numCell)
{
    int i, j;
    system("clear"); // 터미널 화면을 지우는 명령어
    for (int i = 0; i < numCell; i++)
    {
        for (int i = 0; i < numCell - 1; i++)
        {
            cout << "----|";
        }
        cout << "----" << endl;
        for (int j = 0; j < numCell; j++)
        {
            if (board[i][j] == -1)
            {
                if (j == numCell -1){
                    cout << " J   ";
                }
                else {cout << " J  |";}
            }
            else if (board[i][j] == -2)
            {
                if (j == numCell -1){
                    cout << " B   ";
                }
                else {cout << " B  |";}
            }
            else if (board[i][j] == targetNum)
            {
                if (j == numCell -1){
                    cout << "Goal ";
                }
                else {cout << "Goal|";}
            }
            else
            {
                cout << board[i][j];
                if (j == numCell - 1)
                    break;
                if (board[i][j] < 10)
                    cout << "   |";
                else if (board[i][j] < 100)
                    cout << "  |";
                else if (board[i][j] < 1000)
                    cout << " |";
                else if (board[i][j] < 10000)
                    cout << "|";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < numCell - 1; i++)
    {
        cout << "----|";
    }
    cout << "----" << endl;
    printf("\nScore : %d \n", score);
}

void CheckGameOver(int numCell)
{
    int i, j;

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] > 10000)
            { // 10000을 초과하는 경우
                cout << "10000점을 돌파했습니다. 게임을 종료합니다.";
                exit(0);
            }

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] == 0) // 빈 칸이 있는 경우 게임진행
                return;

    for (i = 0; i < numCell - 1; i++)
        for (j = 0; j < numCell - 1; j++)
            if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1])
                // 합칠 수 있는 수가 있는 경우 재개
                return;

    // 만약 위의 for문에 범위를 4미만으로 할 경우 board[4][j]라는 존재하지 않는 정보를 비교하게 됨
    // 때문에 3행 비교와 3열 비교는 각 각 분리해야 함
    for (i = 0; i < numCell - 1; i++)
        if (board[i][numCell - 1] == board[i + 1][numCell - 1]) // 가장 오른쪽 열을 검사
            return;

    for (j = 0; j < numCell - 1; j++)
        if (board[numCell - 1][j] == board[numCell - 1][j + 1]) // 가장 아래쪽 행을 검사
            return;

    // 위 4가지 경우를 모두 충족시키지 못한 경우
    cout << "Game Over..";
    exit(0);
}

void NewNumOrItem(int numCell)
{
    int i, j, cnt = 0;
    for (i = 0; i < numCell; i++)
    {
        for (j = 0; j < numCell; j++)
        {
            if (board[i][j] == 0)
            {
                p0[cnt] = &board[i][j];
                cnt++;
            }
        }
    }

    if (cnt == 0)
        return; // 빈 칸이 없으면 추가하지 않음

    int randIndex = rand() % cnt;
    int randChance = rand() % 100;

    if (randChance < 10)
    {
        *p0[randIndex] = -1; // 10% 확률로 점프 아이템 (J)
    }
    else if (randChance < 15)
    {
        *p0[randIndex] = -2; // 5% 확률로 폭발 블럭 (B)
    }
    else
    {
        *p0[randIndex] = (rand() % 100 < 80) ? 2 : 4; // 숫자 추가
    }
}

void HandleItem(int &current, int &next, int i, int j, int dx, int dy, int act)
{
    if (next == -1)
    { // 점프 아이템
        int nx = i + 2 * dx;
        int ny = j + 2 * dy;
        // 바로 옆 칸이 아닌 두 칸 떨어진 대상
        if (nx >= 0 && nx < numCell && ny >= 0 && ny < numCell && board[nx][ny] == current)
        {
            board[nx][ny] *= 2;
            score += 2 * current;
            current = 0;
            next = 0; // 점프 아이템 소멸
            act++;
        }
    }
    else if (next == -2)
    { // 폭발 아이템
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                int nx = i + x, ny = j + y;
                if (nx >= 0 && nx < numCell && ny >= 0 && ny < numCell)
                {
                    board[nx][ny] = 0; // 인접 블록 제거
                }
            }
        }
        current = 0;
        next = 0; // 폭발 아이템 소멸
        act++;
    }
}

// 원래 의도는 target의 위치를 고정시키고 target을 병합하는 것을 승리요건으로 하려 했으나,
// 위치를 고정하는 부분도 target 이외의 target 크기의 숫자 두 개가 합쳐져도 승리하게 되어버리는 승리조건도 구현하지 못하게 되어 많이 아쉽습니다...
// 하지만 게임 시작과 동시에 제공되는 target 블럭이 플레이에 따라 클리어에 핵심 요소 또는 걸림돌이 될지 모른다는 변수는 또다른 재미요소라 생각합니다.
void LevelCheckGameOver(int targetNum, int numCell)
{
    int i, j;

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] > targetNum) {
                cout << targetNum <<"점을 돌파했습니다. 게임을 종료합니다."; 
                exit(0);
            }

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] == 0) // 빈 칸이 있는 경우 게임진행
                return;

    for (i = 0; i < numCell - 1; i++)
        for (j = 0; j < numCell - 1; j++)
            if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1])
                // 합칠 수 있는 수가 있는 경우 재개
                return;

    // 만약 위의 for문에 범위를 4미만으로 할 경우 board[4][j]라는 존재하지 않는 정보를 비교하게 됨
    // 때문에 3행 비교와 3열 비교는 각 각 분리해야 함
    for (i = 0; i < numCell - 1; i++)
        if (board[i][numCell - 1] == board[i + 1][numCell - 1]) // 가장 오른쪽 열을 검사
            return;

    for (j = 0; j < numCell - 1; j++)
        if (board[numCell - 1][j] == board[numCell - 1][j + 1]) // 가장 아래쪽 행을 검사
            return;

    // 위 4가지 경우를 모두 충족시키지 못한 경우
    cout << "Game Over..";
    exit(0);
}

void Run2048(int numCell)
{
    // 게임을 진행하는 데 필요한 변수 선언
    int key;     // 사용자 입력 변수
    int act;     // 이동 변수
    int i, j, r; // 루프 변수

    srand(time(NULL)); // 난수생성 시드값
    NewNum(numCell);          // 초기값 2개 생성
    NewNum(numCell);
    Draw(targetNum, numCell); // 임의의 수 2개 생성 후 게임판 그리기

    // 게임 시작
    while (1)
    {
        key = GetInput(); // 사용자 입력을 key에 저장
        act = 0;

        // 세부 기능 3 (사용자 입력에 따른 블록 합체 및 이동 기능)
        switch (key)
        {
        case LEFT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = 1; j < numCell; j++)
                { // 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k > 0 && board[i][k - 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k - 1] = board[i][k];
                            board[i][k] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k - 1], i, k, 0, -1, act);
                            if (board[i][k - 1] == board[i][k])
                            { // 합체
                                board[i][k - 1] *= 2;
                                score += board[i][k - 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case RIGHT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = numCell - 2; j >= 0; j--)
                { // 오른쪽에서 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k < numCell - 1 && board[i][k + 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k + 1] = board[i][k];
                            board[i][k] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k + 1], i, k, 0, 1, act);
                            if (board[i][k + 1] == board[i][k])
                            { // 합체
                                board[i][k + 1] *= 2;
                                score += board[i][k + 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case UP:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = 1; i < numCell; i++)
                { // 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k > 0 && board[k - 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k - 1][j] = board[k][j];
                            board[k][j] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k - 1][j], k, j, -1, 0, act);
                            if (board[k - 1][j] == board[k][j])
                            { // 합체
                                board[k - 1][j] *= 2;
                                score += board[k - 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case DOWN:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = numCell - 2; i >= 0; i--)
                { // 아래에서 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k < numCell - 1 && board[k + 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k + 1][j] = board[k][j];
                            board[k][j] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k + 1][j], k, j, 1, 0, act);
                            if (board[k + 1][j] == board[k][j])
                            { // 합체
                                board[k + 1][j] *= 2;
                                score += board[k + 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;
        }
        // 병합된 숫자 복구
        for (i = 0; i < numCell; i++)
        {
            for (j = 0; j < numCell; j++)
            {
                if (board[i][j] > 10000)
                    board[i][j] -= 10000; // 병합된 숫자 복구
            }
        }
        // 동작 발생
        if (act > 0)
        {
            NewNum(numCell);
            Draw(targetNum, numCell);          // 보드 출력
            CheckGameOver(numCell); // 게임 오버 상태 확인
        }
    }
}

void RunItemMode()
{
    // 게임을 진행하는 데 필요한 변수 선언
    int key;     // 사용자 입력 변수
    int act;     // 이동 변수
    int i, j, r; // 루프 변수

    srand(time(NULL)); // 난수생성 시드값
    NewNum(numCell);          // 초기값 2개 생성
    NewNum(numCell);
    Draw(targetNum, numCell); // 임의의 수 2개 생성 후 게임판 그리기

    // 게임 시작
    while (1)
    {
        key = GetInput(); // 사용자 입력을 key에 저장
        act = 0;

        // 세부 기능 3 (사용자 입력에 따른 블록 합체 및 이동 기능)
        switch (key)
        {
        case LEFT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = 1; j < numCell; j++)
                { // 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k > 0 && board[i][k - 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k - 1] = board[i][k];
                            board[i][k] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k - 1], i, k, 0, -1, act);
                            if (board[i][k - 1] == board[i][k])
                            { // 합체
                                board[i][k - 1] *= 2;
                                score += board[i][k - 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case RIGHT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = numCell - 2; j >= 0; j--)
                { // 오른쪽에서 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k < numCell - 1 && board[i][k + 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k + 1] = board[i][k];
                            board[i][k] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k + 1], i, k, 0, 1, act);
                            if (board[i][k + 1] == board[i][k])
                            { // 합체
                                board[i][k + 1] *= 2;
                                score += board[i][k + 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case UP:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = 1; i < numCell; i++)
                { // 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k > 0 && board[k - 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k - 1][j] = board[k][j];
                            board[k][j] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k - 1][j], k, j, -1, 0, act);
                            if (board[k - 1][j] == board[k][j])
                            { // 합체
                                board[k - 1][j] *= 2;
                                score += board[k - 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case DOWN:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = numCell - 2; i >= 0; i--)
                { // 아래에서 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k < numCell - 1 && board[k + 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k + 1][j] = board[k][j];
                            board[k][j] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k + 1][j], k, j, 1, 0, act);
                            if (board[k + 1][j] == board[k][j])
                            { // 합체
                                board[k + 1][j] *= 2;
                                score += board[k + 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;
        }
        // 병합된 숫자 복구
        for (i = 0; i < numCell; i++)
        {
            for (j = 0; j < numCell; j++)
            {
                if (board[i][j] > 10000)
                    board[i][j] -= 10000; // 병합된 숫자 복구
            }
        }
        // 동작 발생
        if (act > 0)
        {
            NewNumOrItem(numCell);
            Draw(targetNum, numCell);          // 보드 출력
            CheckGameOver(numCell); // 게임 오버 상태 확인
        }
    }
}

void LevelSelect2048(int targetNum)
{
    int key;     // 사용자 입력 변수
    int act;     // 이동 변수
    int i, j, r; // 루프 변수

    board[2][2] = targetNum;
    srand(time(NULL)); // 난수생성 시드값
    NewNum(numCell);          // 초기값 2개 생성
    NewNum(numCell);

    Draw(targetNum, numCell); // 임의의 수 2개 생성 후 게임판 그리기

    // 게임 시작
    while (1)
    {
        key = GetInput(); // 사용자 입력을 key에 저장
        act = 0;

        // 세부 기능 3 (사용자 입력에 따른 블록 합체 및 이동 기능)
        switch (key)
        {
        case LEFT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = 1; j < numCell; j++)
                { // 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k > 0 && board[i][k - 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k - 1] = board[i][k];
                            board[i][k] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k - 1], i, k, 0, -1, act);
                            if (board[i][k - 1] == board[i][k])
                            { // 합체
                                board[i][k - 1] *= 2;
                                score += board[i][k - 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case RIGHT:
            for (int i = 0; i < numCell; i++)
            {
                for (int j = numCell - 2; j >= 0; j--)
                { // 오른쪽에서 두 번째 칸부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = j;
                        while (k < numCell - 1 && board[i][k + 1] == 0)
                        { // 빈 칸으로 이동
                            board[i][k + 1] = board[i][k];
                            board[i][k] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[i][k], board[i][k + 1], i, k, 0, 1, act);
                            if (board[i][k + 1] == board[i][k])
                            { // 합체
                                board[i][k + 1] *= 2;
                                score += board[i][k + 1];
                                board[i][k] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case UP:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = 1; i < numCell; i++)
                { // 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k > 0 && board[k - 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k - 1][j] = board[k][j];
                            board[k][j] = 0;
                            k--;
                            act++;
                        }
                        if (k > 0)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k - 1][j], k, j, -1, 0, act);
                            if (board[k - 1][j] == board[k][j])
                            { // 합체
                                board[k - 1][j] *= 2;
                                score += board[k - 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;

        case DOWN:
            for (int j = 0; j < numCell; j++)
            {
                for (int i = numCell - 2; i >= 0; i--)
                { // 아래에서 두 번째 행부터 시작
                    if (board[i][j] != 0)
                    {
                        int k = i;
                        while (k < numCell - 1 && board[k + 1][j] == 0)
                        { // 빈 칸으로 이동
                            board[k + 1][j] = board[k][j];
                            board[k][j] = 0;
                            k++;
                            act++;
                        }
                        if (k < numCell - 1)
                        { // 아이템 및 합체 처리
                            HandleItem(board[k][j], board[k + 1][j], k, j, 1, 0, act);
                            if (board[k + 1][j] == board[k][j])
                            { // 합체
                                board[k + 1][j] *= 2;
                                score += board[k + 1][j];
                                board[k][j] = 0;
                                act++;
                            }
                        }
                    }
                }
            }
            break;
        }
        // 병합된 숫자 복구
        for (i = 0; i < numCell; i++)
        {
            for (j = 0; j < numCell; j++)
            {
                if (board[i][j] > 10000)
                    board[i][j] -= 10000; // 병합된 숫자 복구
            }
        }
        // 동작 발생
        if (act > 0)
        {
            NewNum(numCell);
            Draw(targetNum, numCell); // 보드 출력
            LevelCheckGameOver(targetNum, numCell);
        }
    }
}

// 메인 함수
int main()
{
    const int modeCount = 4; // mode 선택 (설명서 포함)
    const int levelCount = 4; // level 선택 (back 포함)
    int targetNum = 0; // mode 3 변수 초기화
    int** board= CreateBoard(numCell); // 보드 10x10 초기화
    int** p0= CreatePointerArray(numCell); // 포인터 10x10 초기화

    string modes[modeCount] = {"모드 1: 2048 게임", "모드 2: 아이템 모드", "모드 3: 목표달성 모드", "모드 4: 설명서"};
    string levels[levelCount] = {"level: easy(1024)", "level: normal(2048)", "level: hard(4096)", "back"};

    int selected = 0; // 선택된 모드

    while (true)
    {
        DisplayMenu(modes, modeCount, selected);
        char input = GetInput();

        if (input == 'w' || input == 'W')
        {
            selected = (selected - 1 + modeCount) % modeCount;
        }
        else if (input == 's' || input == 'S')
        {
            selected = (selected + 1) % modeCount;
        }
        else if (input == ' ')
        {
            if (selected == 0)
            {
                DeleteBoard(board, numCell);    // 이전 크기로 메모리 해제
                DeletePointerArray(p0);        // 1D 포인터 배열 해제
                ClearScreen();

                cout << "원하는 모드를 입력하시오 (3~10 + enter)" << endl; // 원하는 N 입력
                cin >> numCell; // 입력받은 정수 numCell 초기화 (10 -> input)

                board = CreateBoard(numCell);
                p0 = CreatePointerArray(numCell);
                ClearScreen();
                Run2048(numCell); // 2048 게임 실행
                DeleteBoard(board, numCell);
                DeletePointerArray(p0);

            }
            else if (selected == 1)
            {
                DeleteBoard(board, numCell);    // 이전 크기로 메모리 해제
                DeletePointerArray(p0);        // 1D 포인터 배열 해제
                ClearScreen();
                numCell = 4; // Item 모드 디폴트값 4
                board = CreateBoard(numCell);
                p0 = CreatePointerArray(numCell);
                RunItemMode(); // 게임 실행
                DeleteBoard(board, numCell);
                DeletePointerArray(p0);
            }
            else if (selected == 2)
            {
                ClearScreen();
                cout << "모드 3: 목표달성 모드" << endl;
                while (true)
                {
                    DisplayMenu(levels, levelCount, selected);
                    input = GetInput();
                    if (input == 'w' || input == 'W')
                    {
                        selected = (selected - 1 + levelCount) % levelCount;
                    }
                    else if (input == 's' || input == 'S')
                    {
                        selected = (selected + 1) % levelCount;
                    }
                    else if (input == ' ')
                    {
                        if (selected == 0) // 1024 (easy mode)
                        {
                            DeleteBoard(board, numCell);    // 이전 크기로 메모리 해제
                            DeletePointerArray(p0);        // 1D 포인터 배열 해제
                            ClearScreen();

                            targetNum = 1024;
                            numCell = 5; // Target 모드 디폴트값 5
                            board = CreateBoard(numCell);
                            p0 = CreatePointerArray(numCell);
                            LevelSelect2048(targetNum);
                            DeleteBoard(board, numCell);
                            DeletePointerArray(p0);

                        }
                        else if (selected == 1) // 2048 (normal mode)
                        {
                            DeleteBoard(board, numCell);    // 이전 크기로 메모리 해제
                            DeletePointerArray(p0);        // 1D 포인터 배열 해제
                            ClearScreen();

                            targetNum = 2048;
                            numCell = 5; // target 모드 디폴트값 5
                            board = CreateBoard(numCell);
                            p0 = CreatePointerArray(numCell);
                            LevelSelect2048(targetNum);
                            DeleteBoard(board, numCell);
                            DeletePointerArray(p0);
                        }
                        else if (selected == 2) // 4096 (hard mode)
                        {
                            DeleteBoard(board, numCell);    // 이전 크기로 메모리 해제
                            DeletePointerArray(p0);        // 1D 포인터 배열 해제
                            ClearScreen();

                            targetNum = 4096;
                            numCell = 5; // target 모드 디폴트값 5
                            board = CreateBoard(numCell);
                            p0 = CreatePointerArray(numCell);
                            LevelSelect2048(targetNum);
                            DeleteBoard(board, numCell);
                            DeletePointerArray(p0);
                        }

                        else
                        {
                            break;
                        }
                    }
                }
            }
            else if (selected == 3)
            {
                while (1)
                {
                    ClearScreen();
                    cout << "======================================= 2048 게임 설명서 =======================================" << endl;
                    cout << "1. 목표" << endl;
                    cout << "  - 같은 숫자를 합쳐 더 높은 숫자를 생성하며, 최종적으로 2048을 만들면 승리합니다." << endl;
                    cout << "  - 단, 점수 기능이 있기 때문에 2048을 생성한 이후에도 플레이는 이어서 할 수 있습니다." << endl;
                    cout << "2. 기본 규칙" << endl;
                    cout << "  - 한 번의 움직임마다 보드에 새로운 숫자 2 또는 4가 랜덤 위치에 나타납니다." << endl;
                    cout << "  - 같은 숫자가 충돌하면 합쳐져 두 배가 됩니다 (예: 2 + 2 = 4, 4 + 4 = 8)." << endl;
                    cout << "3. 게임 방법" << endl;
                    cout << "  - WSAD로 위/아래/왼쪽/오른쪽으로 모든 숫자를 이동시킬 수 있습니다." << endl;
                    cout << "  - 숫자는 선택한 방향으로 이동하며, 이동 가능한 가장 먼 칸까지 이동합니다." << endl;
                    cout << "  - 빈칸이 없거나 숫자를 합칠 수 없으면 움직일 수 없습니다." << endl;
                    cout << "==============================================================================================" << endl;
                    cout << " (다음 페이지로 가려면 'SPACE'를 누르세요.) " << endl;
                    cout << " (뒤로 가려면 'b'를 누르세요.) " << endl;

                    char backInput = GetInput();
                    if (backInput == 'b')
                    {
                        break;
                    }
                    else if (backInput == ' ')
                    {
                        while (1){
                            ClearScreen();
                            cout << "=======================================  Mode   설명서  =======================================" << endl;
                            cout << "모드 1: 2048 게임" << endl;
                            cout << "      - 일반적인 2048 게임입니다." << endl;
                            cout << "      - 상한선 10000을 초과하게 되면 게임을 종료합니다." << endl;
                            cout << "      - 3부터 10까지의 정수 중 하나를 입력하여 원하는 크기의 보드로 실행가능합니다." << endl;
                            cout << " " << endl;
                            cout << "모드 2: 아이템 모드" << endl;
                            cout << "      - 일반적인 2048 게임에서 아이템(J, B)이 추가된 모드입니다." << endl;
                            cout << "      - J는 이동하는 숫자 블럭과 접촉했을 때 J블럭을 기준으로 바로 건너편에 일치하는 블럭이 존재하다면 결합됩니다." << endl;
                            cout << "      - B는 이동하는 숫자 블럭과 접촉했을 때 상.하.좌.우.대각선에 위치한 8개의 블럭을 파괴합니다." << endl;
                            cout << "      - 플레이어가 이용하기에 따라 장애물 블럭이 될 수도 있습니다." << endl;
                            cout << "      - 하지만, 이용하기에 따라 10000점을 초과하면 종료하는 기존의 게임보다 최고 기록을 갱신할 수도 있습니다." << endl;
                            cout << "        (참고로 J블럭이 두 개 합쳐지면 B블럭이 된다는 점! 잘 활용해보세요)" << endl;
                            cout << "        (mode 2는 4X4 보드가 디폴트 입니다.)" << endl;
                            cout << " " << endl;
                            cout << "모드 3: 목표달성 모드" << endl;
                            cout << "      - 3가지 level로 구분되며 easy, normal, hard 각각 1024, 2048, 4096의 target 블럭이 초기에 생성됩니다." << endl;
                            cout << "      - 이전 게임의 목표는 기록 갱신이지만, 이 게임은 승리 조건이 target블럭끼리의 결합입니다." << endl;
                            cout << "      - 초기에 생성되는 target 블럭이 게임을 빠르게 끝낼 중요한 key일수도, 걸림돌일수도 있습니다." << endl;
                            cout << "      - 초기에 생성된 target이 아니더라도 플레이어가 생성한 target끼리의 결합도 승리조건입니다." << endl;
                            cout << "        (mode 3는 5X5 보드가 디폴트 입니다.)" << endl;
                            cout << "==============================================================================================" << endl;
                            cout << " (뒤로 가려면 'space'를 누르세요.) " << endl;

                            char backInput = GetInput();
                            if (backInput == ' ')
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
