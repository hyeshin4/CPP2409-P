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
int score = 0;         // Score
const int numCell = 4; // 보드판의 가로 세로 길이 (기본 4x4)
int board[numCell][numCell] = {0}; // 게임판 초기화

// 사용될 함수들
void new_num(void); // 세부 기능 2 (랜덤 2의배수 숫자 생성)
void draw(void); // 세부 기능 1 (3x3 게임판 만들기)
void check_game_over(void); // 세부 기능 6 (게임 승리)

// 세부 기능 4 (사용자 이동기능-입력받기)
int getch(void)
{
    // termios는 비차단 입력을 설정하여 키 입력이 발생하는 즉시 프로그램이 이를 처리함
    struct termios oldattr, newattr; 
    // oldattr는 기존의 터미널 속성을 저장하고, newattr는 변경된 속성을 설정하기 위해 사용
    int ch;                          // 입력받은 한 글자를 저장할 변수 ch를 선언
    tcgetattr(STDIN_FILENO, &oldattr);
    // 현재 터미널 속성을 oldattr에 저장. STDIN_FILENO는 표준 입력 파일 디스크립터(일반적으로 키보드 입력)
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // newattr의 c_lflag 필드를 변경하여 입력 모드를 수정.
    // ICANON 비트를 꺼서 비정규 모드로 변경. Enter를 누르지 않아도 키 입력이 즉시 프로그램으로 전달됨
    // ECHO 비트를 꺼서 에코 기능을 비활성화. 입력한 문자가 화면에 표시되지 않음
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    // 새로운 설정(newattr)을 터미널에 적용합니다. TCSANOW는 즉시 속성을 변경
    ch = getchar(); // 한 문자를 입력받아 ch 변수에 저장
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    // 원래 터미널 속성(oldattr)을 복원, 프로그램이 끝나고 나서 터미널에 영향을 주지 않도록 함
    return ch;
}

int main()
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
        key = getch(); // 사용자 입력을 key에 저장
        act = 0;       

        // 세부 기능 3 (사용자 입력에 따른 블록 합체 및 이동 기능)
        switch (key)
        {
        case LEFT:
            for (i = 0; i < numCell; i++)
            { // 각 행에 대해 처리
                for (j = 1; j <= numCell-1; j++)
                { // 두 번째 칸부터 마지막 칸까지
                    for (r = j; r > 0; r--)
                    { // 현재 칸을 왼쪽으로 이동
                        if (board[i][r] == 0 || board[i][r] > 10000)
                            break; // case1.비어있거나 이미 병합된 경우
                        if (board[i][r - 1] != 0 && board[i][r - 1] != board[i][r])
                            break; // case2.병합 불가능한 경우 (연속 같은 수)
                        if (board[i][r - 1] == 0)
                            board[i][r - 1] = board[i][r]; // case3.왼쪽이 비어있으면 이동
                        else if (board[i][r] == board[i][r - 1])
                        { // case4.같은 숫자가 충돌하면 병합
                            board[i][r - 1] *= 2;       // 2배로 병합
                            board[i][r - 1] += 10000;   // 병합된 숫자 표시
                            score += 2 * (board[i][r]); // 점수 업데이트
                        }
                        // case3,4(움직임 발생)인 경우만 해당
                        board[i][r] = 0; // 현재 칸 비우기
                        act++;           // 이동/병합 발생 표시
                    }
                }
            }
            break;

        case RIGHT:
            for (i = 0; i < numCell; i++)
            { // 각 행에 대해 처리
                for (j = numCell-2; j >= 0; j--)
                { // 마지막 -1 칸부터 첫 번째 칸까지
                    for (r = j; r < numCell-1; r++)
                    { // 현재 칸을 오른쪽으로 이동
                        if (board[i][r] == 0 || board[i][r] > 10000)
                            break;
                        if (board[i][r + 1] != 0 && board[i][r + 1] != board[i][r])
                            break;
                        if (board[i][r + 1] == 0)
                            board[i][r + 1] = board[i][r];
                        else if (board[i][r] == board[i][r + 1])
                        {
                            board[i][r + 1] *= 2;
                            board[i][r + 1] += 10000;
                            score += 2 * (board[i][r]);
                        }
                        board[i][r] = 0;
                        act++;
                    }
                }
            }
            break;

        case UP:
            for (j = 0; j < numCell; j++)
            { // 각 열에 대해 처리
                for (i = 1; i <= numCell-1; i++)
                { // 두 번째 행부터 마지막 행까지
                    for (r = i; r > 0; r--)
                    { // 현재 칸을 위로 이동
                        if (board[r][j] == 0 || board[r][j] > 10000)
                            break;
                        if (board[r - 1][j] != 0 && board[r - 1][j] != board[r][j])
                            break;
                        if (board[r - 1][j] == 0)
                            board[r - 1][j] = board[r][j];
                        else if (board[r][j] == board[r - 1][j])
                        {
                            board[r - 1][j] *= 2;
                            board[r - 1][j] += 10000;
                            score += 2 * (board[r][j]);
                        }
                        board[r][j] = 0;
                        act++;
                    }
                }
            }
            break;

        case DOWN:
            for (j = 0; j < numCell; j++)
            { // 각 열에 대해 처리
                for (i = numCell-2; i >= 0; i--)
                { // 마지막-1 행부터 첫 번째 행까지
                    for (r = i; r < numCell-1; r++)
                    { // 현재 칸을 아래로 이동
                        if (board[r][j] == 0 || board[r][j] > 10000)
                            break;
                        if (board[r + 1][j] != 0 && board[r + 1][j] != board[r][j])
                            break;
                        if (board[r + 1][j] == 0)
                            board[r + 1][j] = board[r][j];
                        else if (board[r][j] == board[r + 1][j])
                        {
                            board[r + 1][j] *= 2;
                            board[r + 1][j] += 10000;
                            score += 2 * (board[r][j]);
                        }
                        board[r][j] = 0;
                        act++;
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
            new_num();         // 새로운 숫자 추가
            draw();            // 보드 출력
            check_game_over(); // 게임 오버 상태 확인
        }
    }
}

// 함수 구체화

// 세부 기능 2 (랜덤 2의배수 숫자 생성)
void new_num(void)
{
    int i, j, cnt = 0;
    int *p0[numCell*numCell] = {0}; // 포인터 배열 p0를 선언

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

// 세부 기능 1 (3x3 게임판 만들기)
void draw(void)
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
            cout << board[i][j];
            if (j == numCell - 1)
            {
                break;
            }
            if (board[i][j] < 10){
                cout << "   |";}
            else if (board[i][j] < 100){
                cout << "  |";}
            else if (board[i][j] < 1000){
                cout << " |";}
            else if (board[i][j] < 10000){
                cout << "|";}
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

// 세부 기능 6 (게임 승리)
void check_game_over(void)
{
    int i, j;

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] > 10000){ // 10000을 초과하는 경우
                cout << "10000점을 돌파했습니다. 게임을 종료합니다.";
                exit(0); }

    for (i = 0; i < numCell; i++)
        for (j = 0; j < numCell; j++)
            if (board[i][j] == 0) // 빈 칸이 있는 경우 게임진행
                return; 

    for (i = 0; i < numCell-1; i++)
        for (j = 0; j < numCell-1; j++)
            if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1]) 
            // 합칠 수 있는 수가 있는 경우 재개
                return;

// 만약 위의 for문에 범위를 4미만으로 할 경우 board[4][j]라는 존재하지 않는 정보를 비교하게 됨
// 때문에 3행 비교와 3열 비교는 각 각 분리해야 함
    for (i = 0; i < numCell-1; i++)
        if (board[i][numCell-1] == board[i + 1][numCell-1]) //가장 오른쪽 열을 검사
            return;

    for (j = 0; j < numCell-1; j++)
        if (board[numCell-1][j] == board[numCell-1][j + 1]) //가장 아래쪽 행을 검사
            return;



// 위 4가지 경우를 모두 충족시키지 못한 경우
    cout << "Game Over..";
    exit(0);
}