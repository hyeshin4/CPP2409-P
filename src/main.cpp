#include "game2048.h"

// 전역 변수 정의
int score = 0;
//const int numCell = 4;
int board[4][4] = {};

// 메인 함수
int main()
{
    const int modeCount = 4;

    string modes[modeCount] = {"모드 1: 2048 게임", "모드 2: 아이템 모드", "모드 3: 목표달성 모드", "모드 4: 설명서"};
    int selected = 0;

    while (true)
    {
        displayMenu(modes, modeCount, selected);
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
                clearScreen();
                cout << "모드 1: 2048 게임을 시작합니다!" << endl;
                Run2048();
            }
            else if (selected == 3)
            {
                clearScreen();
                cout << "1. 목표" << endl;
                cout << "-같은 숫자를 합쳐 더 높은 숫자를 생성하며, 최종적으로 2048을 만들면 승리합니다." << endl;
                cout << "2. 기본 규칙" << endl;
                cout << "-한 번의 움직임마다 보드에 새로운 숫자 2 또는 4가 랜덤 위치에 나타납니다." << endl;
                cout << "-같은 숫자가 충돌하면 합쳐져 두 배가 됩니다 (예: 2 + 2 = 4, 4 + 4 = 8)." << endl;
                cout << "3. 게임 방법" << endl;
                cout << "-WSAD로 위/아래/왼쪽/오른쪽으로 모든 숫자를 이동시킬 수 있습니다." << endl;
                cout << "-숫자는 선택한 방향으로 이동하며, 이동 가능한 가장 먼 칸까지 이동합니다." << endl;
                continue;
            }
            else
            {
                clearScreen();
                cout << modes[selected] << "은(는) 아직 구현되지 않았습니다!" << endl;
                break;
            }
        }
    }

    return 0;
}
