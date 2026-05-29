#include "framework.h"
#include "MyProject_04_04_26 - balls.h"
#include <vector>
#include <time.h>

using namespace std;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const int textDelayY = 20;
const int planeStartX = 50;
const int planeStartY = 50;
const int planeMaxX = 1200;
const int planeMaxY = 900;
const int HpSize = 20;
const int playerSizeX = 45;
const int playerSizeY = 5;
const int playerStep = 30;
const int BallSize = 25;
const int BallsSpawnTick = 20;

int gameTime = 0;
int score = 0;
int healths = 3;
float boost = 3.0f;
int playerX = 200;
int playerY = planeMaxY - 50;

bool gameOver = false;
int globalStep = 0;
vector<pair<int, int>> balls;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    srand(time(0));

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT040426BALLS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT040426BALLS));

    MSG msg;

    // Цикл основного сообщения:
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

void GameOver(HWND hWnd)
{
    gameOver = true;

    MessageBox(hWnd, L"Игра окончена! У вас не осталось жизней!", L"Игра", MB_ICONERROR | MB_OK);
}

void GenerateBalls()
{
    int x = rand() % (planeMaxX - planeStartX - playerSizeX) + planeStartX + playerSizeX / 2;
    int y = planeStartY + 0 - globalStep + BallSize;

    balls.push_back({ x, y });
}

void DeleteHealth(HWND hWnd)
{
    healths--;

    if (healths <= 0) GameOver(hWnd);
}

void ColisionHandler(HWND hWnd)
{
    int maxY = planeMaxY - BallSize;
    int index = 0;
    for (auto ball : balls)
    {
        int ballY = globalStep + ball.second;
        int ballX = ball.first;
        if (maxY < ballY)
        {
            DeleteHealth(hWnd);

            balls.erase(balls.begin() + index);
        }

        if ((playerY - playerSizeY <= ballY + BallSize && playerY + playerSizeY >= ballY - BallSize) && (playerX - playerSizeX <= ballX + BallSize && playerX + playerSizeX >= ballX - BallSize))
        {
            balls.erase(balls.begin() + index);

            score++;
        }

        else index++;
    }
}

void CheckFinish(HWND hWnd)
{
    if (score >= 5)
    {
        gameOver = true;

        MessageBox(hWnd, L"Вы выйграли!", L"Игра", MB_ICONINFORMATION | MB_OK);
    }
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT040426BALLS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT040426BALLS);
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

void DrawHp(HDC hdc, int startX)
{
    for (int i = 1; i <= healths; i++)
    {
        int space = HpSize * 2.2;

        int x = startX + i * space;

        Ellipse(hdc, x - HpSize, textDelayY - HpSize, x + HpSize, textDelayY + HpSize);
    }
}

void BallsDraw(HDC hdc)
{
    for (auto ball : balls)
    {
        int x = ball.first;
        int y = globalStep + ball.second;
        Ellipse(hdc, x - BallSize, y - BallSize, x + BallSize, y + BallSize);
    }
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 1000, NULL);
        SetTimer(hWnd, 2, 100, NULL);
        break;
    }
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        case 'A':
            playerX -= playerStep * boost;
            if (planeStartX + playerSizeX > playerX) playerX = planeStartX + playerSizeX;

            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case VK_RIGHT:
        case 'D':
            playerX += playerStep * boost;
            if (playerX > planeMaxX - playerSizeX) playerX = planeMaxX - playerSizeX;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        break;
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
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
    case WM_TIMER:
    {
        if (wParam == 1 && gameOver == false)
        {
            gameTime++;
        }
        else if (wParam == 2 && gameOver == false)
        {
            globalStep += 7 * boost;

            if (rand() % BallsSpawnTick == 0) GenerateBalls();

            ColisionHandler(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
            HBRUSH backgroundBrush = CreateSolidBrush(RGB(5, 0, 180));
            HPEN planePen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
            HBRUSH textBrush = CreateSolidBrush(RGB(0, 0, 0));
            HBRUSH ballsBrush = CreateSolidBrush(RGB(235, 3, 3));
            HBRUSH healthBrush = CreateSolidBrush(RGB(226, 9, 30));
            HBRUSH playerBrush = CreateSolidBrush(RGB(3, 205, 173));

            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, backgroundBrush);
            //Rectangle(hdc, 0, 0, 1500, 1000);
            Rectangle(hdc, planeStartX, planeStartY, planeMaxX, planeMaxY);

            HPEN hOldPen = (HPEN)SelectObject(hdc, planePen);
            Rectangle(hdc, planeStartX, planeStartY, planeMaxX, planeMaxY);

            WCHAR buffer[50];
            swprintf(buffer, 50, L"Score: %d", score);
            TextOut(hdc, 100, 5, buffer, wcslen(buffer));

            swprintf(buffer, 50, L"HP: ");
            TextOut(hdc, 250, 5, buffer, wcslen(buffer));

            swprintf(buffer, 50, L"Time: %d с", gameTime);
            TextOut(hdc, 500, 5, buffer, wcslen(buffer));

            SelectObject(hdc, hOldPen);
            SelectObject(hdc, healthBrush);
            DrawHp(hdc, 280);

            SelectObject(hdc, playerBrush);
            Rectangle(hdc, playerX - playerSizeX, playerY - playerSizeY, playerX + playerSizeX, playerY + playerSizeY);

            SelectObject(hdc, ballsBrush);
            BallsDraw(hdc);

            if (gameOver == false) CheckFinish(hWnd);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
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
