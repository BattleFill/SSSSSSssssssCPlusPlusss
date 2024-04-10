#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int width = 40;
const int height = 20;

class Snake {
private:
    vector<pair<int, int>> body;
    char direction;
public:
    Snake() {
        direction = 'd';
        body.push_back({ height / 2, width / 2 });
    }

    void changeDirection(char newDir) {
        if ((newDir == 'w' && direction != 's') ||
            (newDir == 's' && direction != 'w') ||
            (newDir == 'a' && direction != 'd') ||
            (newDir == 'd' && direction != 'a')) {
            direction = newDir;
        }
    }

    void move() {
        pair<int, int> newHead;
        switch (direction) {
        case 'w':
            newHead = { (body[0].first - 1 + height) % height, body[0].second };
            break;
        case 's':
            newHead = { (body[0].first + 1) % height, body[0].second };
            break;
        case 'a':
            newHead = { body[0].first, (body[0].second - 1 + width) % width };
            break;
        case 'd':
            newHead = { body[0].first, (body[0].second + 1) % width };
            break;
        }

        if (find(body.begin() + 1, body.end(), newHead) != body.end()) {
            cout << "Game Over! Collision with yourself." << endl;
            exit(0);
        }

        body.insert(body.begin(), newHead);

        if (newHead != body[1]) {
            body.pop_back();
        }
    }

    void grow() {
        body.push_back(body[body.size() - 1]);
    }

    vector<pair<int, int>> getBody() {
        return body;
    }

    pair<int, int> getHead() {
        return body[0];
    }
};

class Food {
private:
    vector<pair<int, int>> positions;
public:
    Food() {
        positions = { {0, 0}, {0, 0} };
        generate();
    }

    vector<pair<int, int>> getPositions() {
        return positions;
    }

    void generate() {
        for (int i = 0; i < 2; ++i) {
            int row = rand() % height;
            int col = rand() % width;
            positions[i] = { row, col };
        }
    }

    void clear() {
        positions = { {-1, -1}, {-1, -1} };
    }
};

void draw(vector<pair<int, int>> snake, vector<pair<int, int>> food) {
    system("cls");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                cout << "#";
            }
            else {
                bool isBodyPart = false;
                for (auto segment : snake) {
                    if (segment.first == i && segment.second == j) {
                        cout << "O";
                        isBodyPart = true;
                        break;
                    }
                }
                if (!isBodyPart) {
                    bool isFood = false;
                    for (auto position : food) {
                        if (position.first == i && position.second == j) {
                            cout << "X";
                            isFood = true;
                            break;
                        }
                    }
                    if (!isFood) cout << " ";
                }
            }
        }
        cout << endl;
    }
}

int main() {
    srand(time(NULL));
    Snake snake;
    Food food;
    int score = 0;

    while (true) {
        draw(snake.getBody(), food.getPositions());
        char key;
        if (_kbhit()) {
            key = _getch();
            snake.changeDirection(key);
        }
        snake.move();
        for (auto position : food.getPositions()) {
            if (snake.getHead() == position) {
                food.generate();
                snake.grow();
                score++;
            }
        }
        Sleep(40);
    }

    return 0;
}
