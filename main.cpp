#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> // For Sleep()

using namespace std;

const int width = 20;
const int height = 10;
const char playerChar = '->';
const char obstacleChar = '=';
const char emptyChar = ' ';
int playerPos = height / 2;
vector<int> obstaclePos(width, -1); // Initialize obstacle positions

void draw() {
    system("cls"); // Clear the console screen
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
    // Move obstacles left
    for (int x = 0; x < width - 1; x++) {
        obstaclePos[x] = obstaclePos[x + 1];
    }
    // Generate new obstacle
    if (rand() % 3 == 0) { // Random chance of generating an obstacle
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
        //Sleep(200); // Slow down the loop
    }
    return 0;
}
