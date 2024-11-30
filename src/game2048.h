#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// 상수 정의
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

// 전역 변수 및 상수 선언
extern int score;         // 점수
extern const int numCell; // 보드판 크기
extern int board[4][4];   // 게임판

// 공용 함수 선언
void clearScreen();
void displayMenu(const string modes[], int modeCount, int selected);
char GetInput();
void Run2048();

// 2048 게임 관련 함수
void Draw();
void NewNum();
void NewNumOrItem();
void CheckGameOver();
void HandleItem(int &current, int &next, int i, int j, int dx, int dy, int act);