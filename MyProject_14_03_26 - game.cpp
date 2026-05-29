#include "framework.h"
#include "MyProject_14_03_26 - game.h"
#include <vector>

#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100
using namespace std;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#define TIMER_ID 1
#define TIME_INTERVAL 100

int a = 0;
int Time = 0;
int neededTime = 500;

int planePos = 50;
int cell_size = 30;
const int length = 15;
int plane[length][length];
int apple[2] = {7, 7};
bool eatingAplle = false;
bool gameOver = false;

vector<pair<int, int>> snake = { {10, 10}, {10, 9} };

struct Direction
{
    int x = 0;
    int y = 0;

    Direction(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}

    void Rigth()
    {
        if (x != -1 && gameOver == false)
        {
            y = 0;
            x = 1;
        }
    }

    void Left()
    {
        if (x != 1 && gameOver == false)
        {
            y = 0;
            x = -1;
        }
    }

    void Up()
    {
        if (y != 1 && gameOver == false)
        {
            y = -1;
            x = 0;
        }
    }

    void Down()
    {
        if (y != -1 && gameOver == false)
        {
            y = 1;
            x = 0;
        }
    }
};
Direction direction(0, 1);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT140326GAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    //AllocConsole();
    //FILE* fDummy;
    //freopen_s(&fDummy, "CONOUT$", "w", stdout);
    //freopen_s(&fDummy, "CONOUT$", "w", stderr);
    //freopen_s(&fDummy, "CONOUT$", "r", stdin);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT140326GAME));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT140326GAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT140326GAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

void GameOver(HWND hWnd, int type)
{
    gameOver = true;
    
    wchar_t buffer[100];

    if (type == 0)
    {
        swprintf(buffer, 100, L"Игра окончена! Вы вышли за пределы карты! \nВаш счёт: %d", a); 
        MessageBox(hWnd, buffer, L"Змейка", 0);
    }

    else if (type == 1)
    {
        swprintf(buffer, 100, L"Игра окончена! Вы сЪели себя! \nВаш счёт: %d", a);
        MessageBox(hWnd, buffer, L"Змейка", 0);
    }
    exit(0);
}

void GenerateApple(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(30, 230, 30));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, planePos + cell_size * apple[0], planePos + cell_size * apple[1], planePos + cell_size * (apple[0] + 1), planePos + cell_size * (apple[1] + 1));

    DeleteObject(hBrush);
    SelectObject(hdc, hOldBrush);
}

void CALLBACK TimerProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    if (gameOver) return;
    
    Time += TIME_INTERVAL;

    if (Time < neededTime) return;
    else Time = 0;

    if (!eatingAplle)
    {
        snake.pop_back();
    }
    else eatingAplle = false;

    snake.insert(snake.begin(), {snake[0].first, snake[0].second});
    snake[0].first += direction.x;
    snake[0].second += direction.y;

    if (snake[0].first < 0 || length <= snake[0].first) GameOver(hWnd, 0);
    if (snake[0].second < 0 || length <= snake[0].second) GameOver(hWnd, 0);

    for (int i = 3; i < snake.size(); i++)
    {
        if (snake[0].first == snake[i].first && snake[0].second == snake[i].second) GameOver(hWnd, 1);
    }
    
    if (snake[0].first == apple[0] && snake[0].second == apple[1])
    {
        apple[0] = rand() % length;
        apple[1] = rand() % length;

        eatingAplle = true;
        a++;

        if (a >= 3) neededTime = 400;
        if (a >= 5) neededTime = 300;
        if (a >= 7) neededTime = 200;
        if (a >= 10) neededTime = 100;
    }
    
    InvalidateRect(hWnd, NULL, TRUE);
}

void DrawCells(HDC hdc)
{
    for (int y = 0; y < length; y++)
    {
        for (int x = 0; x < length; x++)
        {
            Rectangle(hdc, planePos + cell_size * x, planePos + cell_size * y, planePos + cell_size * (x + 1), planePos + cell_size * (y + 1));
        }
    }
}

void DrawSnakeCell(HDC hdc, int x, int y)
{
    Rectangle(hdc, planePos + cell_size * x, planePos + cell_size * y, planePos + cell_size * (x + 1), planePos + cell_size * (y + 1));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        case 'A':
            direction.Left();
            break;
        case VK_RIGHT:
        case 'D':
            direction.Rigth();
            break;
        case VK_UP:
        case 'W':
            direction.Up();
            break;
        case VK_DOWN:
        case 'S':
            direction.Down();
            break;
        }
        break;
    }
    break;
    case WM_CREATE:
        {
            if (gameOver == false) SetTimer(hWnd, TIMER_ID, TIME_INTERVAL, TimerProc);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            

            WCHAR buffer[100];
            swprintf(buffer, 100, L"Счёт: %d", a);
            TextOut(hdc, 200, 20, buffer, wcslen(buffer));

            int endPos = planePos + cell_size * length;
            Rectangle(hdc, planePos, planePos, endPos, endPos);
            DrawCells(hdc);
            //----------------

            HBRUSH hBrush = CreateSolidBrush(RGB(254, 0, 0));
            HBRUSH hBrushHead = CreateSolidBrush(RGB(181, 0, 0));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushHead);

            DrawSnakeCell(hdc, snake[0].first, snake[0].second);

            SelectObject(hdc, hBrush);
            
            for (int i = 1; i < snake.size(); i++)
            {
                DrawSnakeCell(hdc, snake[i].first, snake[i].second);
            }
            DeleteObject(hBrush);
            SelectObject(hdc, hOldBrush);

            GenerateApple(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (gameOver == false) KillTimer(hWnd, TIMER_ID);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
