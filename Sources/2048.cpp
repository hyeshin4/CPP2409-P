#include <iostream>
using namespace std;

int score = 0;         // Score
const int numCell = 4; // 보드판의 가로 세로 길이
int board[4][4] = {    // Game board
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};

// 무작위 숫자 생성
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

// 게임판 그리기
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

int main()
{
    // 게임을 진행하는 데 필요한 변수 선언
    char board[numCell][numCell]{}; // 보드판을 나타내는 2차원 배열

    srand(time(NULL)); // Seed random number generator
    new_num();         // Generate two initial numbers
    new_num();
    draw(); // Draw initial game board
}