#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
using namespace std;

const int width = 30;
const int height = 20;

bool gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100], nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
int seconds = 0;

void HideCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console, &info);
}

void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % (width - 2) + 1;
    fruitY = rand() % (height - 2) + 1;
    score = 0;
    nTail = 0;
    seconds = 0;
    HideCursor();
}

void Draw() {
    SetCursorPosition(0, 0);
    cout << "Time: " << seconds << "s    Score: " << score << endl;

    for (int i = 0; i <= height; i++) {
        for (int j = 0; j <= width; j++) {
            if (i == 0 || i == height || j == 0 || j == width)
                cout << "#";
            else if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool printed = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printed = true;
                        break;
                    }
                }
                if (!printed) cout << " ";
            }
        }
        cout << endl;
    }
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': dir = LEFT; break;
            case 'd': dir = RIGHT; break;
            case 'w': dir = UP; break;
            case 's': dir = DOWN; break;
            case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    if (x <= 0) x = width - 1;
    else if (x >= width) x = 1;
    if (y <= 0) y = height - 1;
    else if (y >= height) y = 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        nTail++;
    }
}

int main() {
    Setup();
    DWORD startTime = GetTickCount();

    while (!gameOver && seconds < 60) {
        DWORD currentTime = GetTickCount();
        seconds = (currentTime - startTime) / 1000;

        Draw();
        Input();
        Logic();
        Sleep(100);
    }

    SetCursorPosition(0, height + 2);
    if (seconds >= 60)
        cout << "Time's up!\n";
    else
        cout << "Game Over!\n";

    cout << "Final Score: " << score << endl;
    system("pause");
    return 0;
}
