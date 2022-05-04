#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define mapWidth 115
#define mapHeight 29

typedef struct Object {  // Структура объекта
    float x, y;
    float radius;
    float vertSpeed;
    float horizSpeed;
} MapObject;


char map[mapHeight][mapWidth + 1];
MapObject circle;


BOOL IsPosInMap(int x, int y)   // Проверка выхода за массив
{
    return ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
}


void CreateMap()   // Создание массива с границами
{
    for (int i = 0; i < mapHeight; i++)
        for (int j = 0; j < mapWidth; j++)
            map[i][j] = ' ';
    for (int i = 0; i < mapWidth; i++) {
        map[0][i] = 'g';
        map[mapHeight - 1][i] = 'g';
    }
    for (int i = 0; i < mapHeight; i++) {
        map[i][0] = 'g';
        map[i][mapWidth - 1] = 'g';
    }
}


void PrintMap()    // Вывод массива в консоль
{
    map[mapHeight - 1][mapWidth - 1] = '\0';
    printf("\n");
    for (int j = 0; j < mapHeight; j++)
        printf("%s \n", map[j]);
}


void SetObjectPos(MapObject* obj, float xPos, float yPos)   // Меняет позицию у объекта
{
    (*obj).x = xPos;
    (*obj).y = yPos;
}


void InitObject(MapObject* obj, float xPos, float yPos, float cRadius)    // Создание объекта
{
    SetObjectPos(obj, xPos, yPos);
    (*obj).radius = cRadius;
    (*obj).vertSpeed = 0;
    (*obj).horizSpeed = 0;
}


void MoveObject(MapObject* obj)     // Движение объекта в массиве
{
    SetObjectPos(obj, (*obj).x + +(*obj).horizSpeed, (*obj).y + (*obj).vertSpeed);
    if ((*obj).y + (*obj).radius + 2 > mapHeight) (*obj).y -= 1;
    if ((*obj).y - (*obj).radius - 1 < 0) (*obj).y += 1;
    if ((*obj).x - (*obj).radius - 1 < 0) (*obj).x += 1;
    if ((*obj).x + (*obj).radius + 2 > mapWidth) (*obj).x -= 1;
    (*obj).vertSpeed = 0;
    (*obj).horizSpeed = 0;
}


void PutObjectOnMap(MapObject obj)    // Создание окружности в массиве
{
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iRadius = (int)round(obj.radius);
    
    for (int i = ix - iRadius; i < (ix + iRadius + 1); i++)
        for (int j = iy - iRadius; j < (iy + iRadius + 1); j++) 
            if ((pow(i - ix, 2) + pow(j - iy, 2) <= pow(iRadius, 2)) && IsPosInMap(i, j)) map[j][i] = '@';  
}


void setCur(int x, int y)    // Изменение положения курсора в консоли
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


int main()
{
    printf("entry radius(1-15)\n");
    int radius;
    scanf("%o", &radius);

    printf("entry color(1-15):\n1 - blue, 2 - green, 3 - cyan, 4 - red, 5 - purple, 6 - yellow, 7 - white, 8 - grey \n");
    int temp;
    scanf("%o", &temp);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), temp);   // Меняем цвет вывода в консоли

    /* Создание окружности */
    InitObject(&circle, 55, 15, radius);

    do
    {
        CreateMap();
        /* Обработка нажатия клавиш */
        if (GetKeyState(VK_SPACE) < 0 && GetKeyState(VK_RIGHT) < 0) circle.x = mapWidth - circle.radius - 2;
        if (GetKeyState(VK_SPACE) < 0 && GetKeyState(VK_DOWN) < 0) circle.y = mapHeight - circle.radius - 2;
        if (GetKeyState(VK_SPACE) < 0 && GetKeyState(VK_UP) < 0) circle.y = circle.radius + 1;
        if (GetKeyState(VK_SPACE) < 0 && GetKeyState(VK_LEFT) < 0) circle.x = circle.radius + 1;
        if (GetKeyState(VK_UP) < 0) circle.vertSpeed = -1;
        if (GetKeyState(VK_DOWN) < 0) circle.vertSpeed = +1;
        if (GetKeyState(VK_LEFT) < 0) circle.horizSpeed = -1;
        if (GetKeyState(VK_RIGHT) < 0) circle.horizSpeed = +1;
        if (GetKeyState(VK_HOME) < 0) {
            circle.x = 55;
            circle.y = 15;
        }
        MoveObject(&circle);
        PutObjectOnMap(circle);
        setCur(0, 0);
        PrintMap();
        /* Задержка для цикла */
        Sleep(1);
    } while (GetKeyState(VK_ESCAPE) >= 0);  // Консоль закроется при нажатии на Esc
    return 0;
}
