#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

using namespace std;

const int width = 20;
const int height = 10;
const char playerChar = '>';
const char obstacleChar = '=';
const char emptyChar = ' ';
int playerPos = height / 2;
vector<int> obstaclePos(width, -1);
int level = 1;
int obstacleSpeed = 200;

void draw() {
    system("cls");
    cout << "Level: " << level << endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 && y == playerPos) {
                cout << playerChar;
            } else if (obstaclePos[x] == y) {
                cout << obstacleChar;
            } else {
                cout << emptyChar;
            }
        }
        cout << endl;
    }
}

void update() {
    for (int x = 0; x < width - 1; x++) {
        obstaclePos[x] = obstaclePos[x + 1];
    }
    // Generate new obstacle
    if (rand() % (3 - level / 3) == 0) {
        obstaclePos[width - 1] = rand() % height;
    } else {
        obstaclePos[width - 1] = -1;
    }
}

bool checkCollision() {
    if (obstaclePos[0] == playerPos) {
        return true;
    }
    return false;
}

void nextLevel() {
    level++;
    obstacleSpeed -= 20;
    if (obstacleSpeed < 50) {
        obstacleSpeed = 50;
    }
    playerPos = height / 2;
    obstaclePos = vector<int>(width, -1);
    cout << "Next Level: " << level << "!" << endl;
    Sleep(1000);
}

int main() {
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'w' && playerPos > 0) {
                playerPos--;
            } else if (ch == 's' && playerPos < height - 1) {
                playerPos++;
            }
        }
        update();
        draw();
        if (checkCollision()) {
            cout << "Game Over!" << endl;
            break;
        }
        Sleep(obstacleSpeed);
        if (rand() % 10 == 0) {
            nextLevel();
        }
    }
    return 0;
}
