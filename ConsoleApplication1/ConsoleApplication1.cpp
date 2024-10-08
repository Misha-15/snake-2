#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
using namespace std;

// Функция для инициализации динамического массива
int** CreateArray(int size) {
    int** arr = new int* [size];
    for (int i = 0; i < size; i++) {
        arr[i] = new int[2]; // Две координаты: x и y
    }
    return arr;
}

// Функция для удаления динамического массива
void DeleteArray(int** arr, int size) {
    for (int i = 0; i < size; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

// Настройки начального состояния игры
void Setup(int& x, int& y, int& fruitX, int& fruitY, int& score, int& nTail, int**& tail, int& obstacleCount, int**& obstacles, int width, int height) {
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tail = CreateArray(width * height);  // Динамический массив для хвоста
    obstacleCount = 10;
    obstacles = CreateArray(obstacleCount); // Динамический массив для препятствий

    // Генерация препятствий
    for (int i = 0; i < obstacleCount; i++) {
        obstacles[i][0] = rand() % width;
        obstacles[i][1] = rand() % height;
    }
}

// Функция для рисования игрового поля
void Draw(int x, int y, int fruitX, int fruitY, int score, int nTail, int** tail, int obstacleCount, int** obstacles, int width, int height) {
    system("cls");

    // Верхняя граница
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Поле игры
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; // Левая граница

            if (i == y && j == x)
                cout << "O"; // Голова змейки
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Фрукт
            else {
                bool print = false;

                // Проверка на хвост змейки
                for (int k = 0; k < nTail; k++) {
                    if (tail[k][0] == j && tail[k][1] == i) {
                        cout << "o";
                        print = true;
                    }
                }

                // Проверка на препятствия
                for (int k = 0; k < obstacleCount; k++) {
                    if (obstacles[k][0] == j && obstacles[k][1] == i) {
                        cout << "X";
                        print = true;
                    }
                }

                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#"; // Правая граница
        }
        cout << "\n";
    }

    // Нижняя граница
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << "\n";

    cout << "score: " << score << "\n";
}

// Функция для управления движением змейки
void Input(eDirection& dir) {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'x':
            dir = STOP; // Прекращение игры
            break;
        }
    }
}

// Функция для логики игры
void Logic(int& x, int& y, int& fruitX, int& fruitY, int& score, int& nTail, int** tail, eDirection& dir, int obstacleCount, int** obstacles, int width, int height, int& speed) {
    int prevX = x, prevY = y;  // Запоминаем предыдущие координаты
    int prev2X, prev2Y;

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Столкновение с границами поля
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    // Столкновение с хвостом
    for (int i = 0; i < nTail; i++)
        if (tail[i][0] == x && tail[i][1] == y)
            dir = STOP;

    // Перемещение хвоста
    for (int i = 0; i < nTail; i++) {
        prev2X = tail[i][0];
        prev2Y = tail[i][1];
        tail[i][0] = prevX;
        tail[i][1] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Столкновение с препятствиями
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i][0] == x && obstacles[i][1] == y)
            dir = STOP;
    }

    // Поедание фрукта
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        if (speed > 30) speed -= 5;
    }
}

// Основной цикл игры
int main() {
    int x, y, fruitX, fruitY, score, nTail, obstacleCount, speed = 100;
    int width = 40, height = 20;
    int** tail = nullptr;
    int** obstacles = nullptr;
    eDirection dir = STOP;

    srand(time(0));

    Setup(x, y, fruitX, fruitY, score, nTail, tail, obstacleCount, obstacles, width, height);

    while (dir != STOP) {
        Draw(x, y, fruitX, fruitY, score, nTail, tail, obstacleCount, obstacles, width, height);
        Input(dir);
        Logic(x, y, fruitX, fruitY, score, nTail, tail, dir, obstacleCount, obstacles, width, height, speed);
        Sleep(speed);
    }

    cout << "Game over! Your score: " << score << "\n";

    DeleteArray(tail, width * height);
    DeleteArray(obstacles, obstacleCount);
    return 0;
}
