#include "framework.h"
#include "MyProject_11_04_26 - tetris.h"
#include <vector>
#include <utility>
#include <time.h>
#include <ObjIdl.h>
#include <gdiplus.h>


#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

using FigurePoint = pair<int, int>;
using Figure = vector<FigurePoint>;

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

const int PlaneDelayX = 350;
const int PlaneDelayY = 50;
const int PlaneCellsX = 15;
const int PlaneCellsY = 25;
const int PlaneCellSize = 35;
const int FigureBoard = 1;

const vector<Figure> figures = 
{ 
    {{0, 0}, {0, 1}, {1, 1}, {1, 0}  }, // куб  0
    {{0, -1}, {0, 0}, {0, 1}, {0, 2} },  //палка  1
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1} },  //треугольник  2
    {{0, -1}, {0, 0}, {0, 1}, {1, 1} },  //буква г  3
    {{0, -1}, {0, 0}, {0, 1}, {1, -1} },  //буква г2  4
    {{-1, -1}, {0, -1}, {0, 0}, {1, 0} },  //червяк  5
    {{1, 1}, {0, 1}, {0, 0}, {-1, 0} },  //червяк2  5
};

//const vector<COLORREF> = {};

vector<vector<int>> plane(PlaneCellsY, vector<int>(PlaneCellsX, 0));

Figure currentFigure;
pair<int, int> figurePosition = { PlaneCellsX / 2, 3 };;

int score = 0;

float gameBoost = 1;
int elapsedTime = 0;
int targetTime = 400;
bool isPause = false;

ULONG_PTR gdiplusToken;
void InitGDI()
{
    GdiplusStartupInput gdiplus;
    GdiplusStartup(&gdiplusToken, &gdiplus, NULL);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    InitGDI();
    srand(time(0));
    for (int i = 0; i < PlaneCellsX - 1; i++) plane[24][i] = 1;

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT110426TETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT110426TETRIS));

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

Figure rotateFigure(const Figure& fig, int quarter)
{
    Figure result;
    for (auto point : fig)
    {
        if (quarter == 1) result.emplace_back(point.second, -point.first);
        if (quarter == -1) result.emplace_back(-point.second, point.first);
        if (quarter == 2) result.emplace_back(-point.second, -point.first);
    }
    return result;
}

int CheckColision()
{
    for (auto point : currentFigure)
    {
        int x = figurePosition.first + point.first;
        int y = figurePosition.second + point.second;
        
        if (x < 0 || PlaneCellsX <= x) return 1;
        if (y < 0 || PlaneCellsY <= y) return 1;

        bool hasBlock = plane[y][x] != 0;
        if (hasBlock) return 2;
    }
    return 0;
}

void DeleteRow(int row)
{
    for (int x = 0; x < PlaneCellsX; x++)
    {
        plane[row][x] = 0;
    }
}

void StepFigures(int nullRow)
{
    for (int y = 0; y < nullRow; y++)
    {
        for (int x = 0; x < PlaneCellsX; x++)
        {
            bool hasBlock = plane[y][x] != 0;
            if (hasBlock)
            {
                plane[y + 1][x] = plane[y][x];
                plane[y][x] = 0;
            }
        }
    }
}

void CheckFullRows()
{
    for (int y = 0; y < PlaneCellsY; y++)
    {
        int blocks = 0;
        for (int x = 0; x < PlaneCellsX; x++)
        {
            bool hasBlock = plane[y][x] != 0;
            if (hasBlock) blocks++;
            //else continue;
        }
        if (blocks == PlaneCellsX)
        {
            DeleteRow(y);
            StepFigures(y);
        }
    }
}

void GameProcess(HWND hWnd)
{
    if (currentFigure.size() == 0)
    {
        figurePosition = {PlaneCellsX / 2, 3};

        currentFigure = figures[rand() % figures.size()];

        if (CheckColision())
        {
            isPause = true;
            MessageBox(hWnd, L"Фигуры заполнены, вы проиграли!", L"Тетрис", MB_OK | MB_ICONINFORMATION);
            exit(0);
        }
    }
    figurePosition.second += 1;

    if (CheckColision())
    {
        figurePosition.second -= 1;

        for (auto point : currentFigure)
        {
            int x = figurePosition.first + point.first;
            int y = figurePosition.second + point.second;
            plane[y][x] = 1;
        }
        currentFigure.clear();
        score++;
    }

    CheckFullRows();

    RECT rect = { PlaneDelayX, PlaneDelayY, PlaneDelayX + PlaneCellsX * PlaneCellSize, PlaneDelayY + PlaneCellsY * PlaneCellSize };
    InvalidateRect(hWnd, &rect, FALSE);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT110426TETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT110426TETRIS);
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

void DrawBlock(HDC hdc, int x, int y, HBRUSH hBoardBrush, HBRUSH hFigureBrush)
{
    SelectObject(hdc, hBoardBrush);
    Rectangle(hdc, PlaneDelayX + x * PlaneCellSize, PlaneDelayY + y * PlaneCellSize, PlaneDelayX + (x + 1) * PlaneCellSize, PlaneDelayY + (y + 1) * PlaneCellSize);
    SelectObject(hdc, hFigureBrush);
    Rectangle(hdc, PlaneDelayX + x * PlaneCellSize + FigureBoard, PlaneDelayY + y * PlaneCellSize + FigureBoard, PlaneDelayX + (x + 1) * PlaneCellSize - FigureBoard, PlaneDelayY + (y + 1) * PlaneCellSize - FigureBoard);
}

void DrawPlane(HDC hdc)
{
    HBRUSH hBoardBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hFigureBrush = CreateSolidBrush(RGB(254, 126, 0));

    for (int y = 0; y < PlaneCellsY; y++)
    {
        for (int x = 0; x < PlaneCellsX; x++)
        {
            bool hasBlock = plane[y][x] != 0;

            if (hasBlock)
            {
                DrawBlock(hdc, x, y, hBoardBrush, hFigureBrush);
            }
        }
    }

    for (auto point : currentFigure)
    {
        DrawBlock(hdc, figurePosition.first + point.first, figurePosition.second + point.second, hBoardBrush, hFigureBrush);
    }

    DeleteObject(hBoardBrush);
    DeleteObject(hFigureBrush);
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
        SetTimer(hWnd, 1, 100, NULL);
    }
    case WM_TIMER:
    {
        if (wParam == 1 && !isPause)
        {
            elapsedTime += 100;
            if (elapsedTime >= targetTime)
            {
                GameProcess(hWnd);
                elapsedTime = 0;
            }
        }
        break;
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        {
            currentFigure = rotateFigure(currentFigure, -1);

            if (CheckColision() == 1)
            {
                if (figurePosition.first < PlaneCellsX / 2) figurePosition.first += 1;
                if (figurePosition.first < PlaneCellsX / 2) figurePosition.first += 1;

                if (figurePosition.first > PlaneCellsX / 2) figurePosition.first -= 1;
                if (figurePosition.first > PlaneCellsX / 2) figurePosition.first -= 1;
            }
            if (CheckColision() == 2) figurePosition.second += 1;
            if (CheckColision() == 2) figurePosition.second += 1;
            
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_RIGHT:
        {
            currentFigure = rotateFigure(currentFigure, 1);

            if (CheckColision() == 1)
            {
                if (figurePosition.first < PlaneCellsX / 2) figurePosition.first += 1;
                if (figurePosition.first < PlaneCellsX / 2) figurePosition.first += 1;

                if (figurePosition.first > PlaneCellsX / 2) figurePosition.first -= 1;
                if (figurePosition.first > PlaneCellsX / 2) figurePosition.first -= 1;
            }
            if (CheckColision() == 2) figurePosition.second += 1;
            if (CheckColision() == 2) figurePosition.second += 1;

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_DOWN:
        {
            figurePosition.second += 1;
            if (CheckColision() != 0) figurePosition.second -= 1;

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case VK_UP:
        {
            isPause = !isPause;

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 'A':
        {
            figurePosition.first -= 1;
            if (CheckColision() != 0) figurePosition.first += 1;
            
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case 'D':
        {
            figurePosition.first += 1;
            if (CheckColision() != 0) figurePosition.first -= 1;

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        default:
            break;
        }
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HPEN hBoardPen = CreatePen(BS_SOLID, 5, RGB(0, 0, 0));

            RECT rect;
            GetClientRect(hWnd, &rect);
            Graphics graphics(hdc);
            Rect r(0, 0, rect.right, rect.bottom);

            LinearGradientBrush brush(
                Point(0, 0),
                Point(rect.right, rect.bottom),
                Color(42, 200, 90),
                Color(0, 0, 128)
                );

            //graphics.FillRectangle(&brush, r);

            HPEN hOldPen = (HPEN)SelectObject(hdc, hBoardPen);
            Rectangle(hdc, PlaneDelayX, PlaneDelayY, PlaneDelayX + PlaneCellsX * PlaneCellSize, PlaneDelayY + PlaneCellsY * PlaneCellSize);
            SelectObject(hdc, hOldPen);
            
            HFONT hFont = CreateFont(48, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Courier New");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

            SetBkMode(hdc, TRANSPARENT);

            WCHAR buffer[100];
            swprintf_s(buffer, 100, L"SCORE:%d", score);
            TextOutW(hdc, 900, 50, buffer, wcslen(buffer));

            swprintf_s(buffer, 100, L"PAUSE");
            if (isPause) TextOutW(hdc, 900, 100, buffer, wcslen(buffer));

            SelectObject(hdc, hOldFont);
            
            DrawPlane(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
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
