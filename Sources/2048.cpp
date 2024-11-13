#include <iostream>
#include <termios.h> //터미널 I/O(입출력) 설정을 제어하는 헤더 파일
#include <unistd.h>  //기본적인 시스템 레벨의 입출력 작업, 프로세스 제어, 파일 관리 등을 위한 여러 함수들을 제공

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
// adws로 입력받기
using namespace std;

int score = 0;         // Score
const int numCell = 4; // 보드판의 가로 세로 길이
int board[4][4] = {    // Game board
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

// 세부 기능 2 (랜덤 2의배수 숫자 생성)
void new_num(void)
{
    int i, j, cnt = 0;
    int *p0[16] = {0}; // 포인터 배열 p0를 선언하여 최대 16개의 빈 칸을 저장할 수 있도록 초기화 (게임판의 총 칸 수는 4x4=16)

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
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
            cout << "---|";
        }
        cout << "---" << endl;
        for (int j = 0; j < numCell; j++)
        {
            cout << board[i][j];
            if (j == numCell - 1)
            {
                break;
            }
            cout << "  |";
        }
        cout << endl;
    }
    for (int i = 0; i < numCell - 1; i++)
    {
        cout << "---|";
    }
    cout << "---" << endl;
    printf("\nScore : %d \n", score);
}

// 세부 기능 4 (사용자 이동기능-입력받기)
int getch(void)
{
    // termios는 비차단 입력을 설정하여 키 입력이 발생하는 즉시 프로그램이 이를 처리함
    struct termios oldattr, newattr; // oldattr는 기존의 터미널 속성을 저장하고, newattr는 변경된 속성을 설정하기 위해 사용
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
    int key;     // 사용자 입력 변수
    int act;     // 이동 변수
    int i, j, r; // 루프 변수

    // 게임을 진행하는 데 필요한 변수 선언
    // char board[numCell][numCell]{}; // 보드판을 나타내는 2차원 배열

    srand(time(NULL)); // Seed random number generator
    new_num();         // Generate two initial numbers
    new_num();
    draw(); // Draw initial game board

    // 게임 시작
    while (1)
    {
        key = getch(); // Get key input
        act = 0;       //

        // 세부 기능 3 (입력에 따라 블록 합체 및 이동 기능)
        switch (key) {
            case LEFT:
                cout << "왼쪽" <<endl;
                break;
            case RIGHT:
                cout << "오른쪽" <<endl;
                break;
            case UP:
                cout << "위" <<endl;
                break;
            case DOWN:
                cout << "아래" <<endl;
                break;

        }
    }
}
