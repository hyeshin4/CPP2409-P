#include "game2048.h"

const int numCell = 4;

void clearScreen()
{
    cout << "\033[2J\033[1;1H";
}
void run2048()
{
    // 게임을 진행하는 데 필요한 변수 선언
    int key;     // 사용자 입력 변수
    int act;     // 이동 변수
    int i, j, r; // 루프 변수

    srand(time(NULL)); // 난수생성 시드값
    new_num();         // 초기값 2개 생성
    new_num();

    draw(); // 임의의 수 2개 생성 후 게임판 그리기

    // 게임 시작
    while (1)
    {
        key = getInput(); // 사용자 입력을 key에 저장
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
                            handleItem(board[i][k], board[i][k - 1], i, k, 0, -1, act);
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
                            handleItem(board[i][k], board[i][k + 1], i, k, 0, 1, act);
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
                            handleItem(board[k][j], board[k - 1][j], k, j, -1, 0, act);
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
                            handleItem(board[k][j], board[k + 1][j], k, j, 1, 0, act);
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
            new_num_or_item();
            draw();            // 보드 출력
            check_game_over(); // 게임 오버 상태 확인
        }
    }
}

void displayMenu(const string modes[], int modeCount, int selected)
{
    clearScreen();
    cout << "==========================" << endl;
    cout << "        게임 모드         " << endl;
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

char getInput()
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

void draw()
{
    system("clear");
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
                cout << " J  |";
            }
            else if (board[i][j] == -2)
            {
                cout << " B  |";
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

void new_num()
{
    int cnt = 0;
    int *p0[numCell * numCell] = {0};

    for (int i = 0; i < numCell; i++)
    {
        for (int j = 0; j < numCell; j++)
        {
            if (board[i][j] == 0)
            {
                p0[cnt] = &board[i][j];
                cnt++;
            }
        }
    }

    *p0[rand() % cnt] = (rand() % 100 < 80) ? 2 : 4;
}

void new_num_or_item()
{
    int i, j, cnt = 0;
    int *p0[numCell * numCell] = {0};

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

void check_game_over()
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

void handleItem(int &current, int &next, int i, int j, int dx, int dy, int act)
{
    if (next == -1)
    { // 점프 아이템
        int nx = i + 2 * dx;
        int ny = j + 2 * dy;
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
