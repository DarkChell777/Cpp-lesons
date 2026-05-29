#include "framework.h"
#include "MyProject_21_03_26.h"
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <unordered_map>
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


#define ID_CELL(n) (1000 + (n))


const int PlanePos = 100;
const int CellSize = 40;
const int CellSpace = 1;
const int length = 5;

int box[length][length];
int original_box[length][length];
int bombs_box[length][length];

struct ButtonData
{
    HBRUSH hBrush;
    COLORREF textColor;
};

vector<pair<int, HWND>> Cells;
unordered_map<int, ButtonData> CellsColor;

int selectedCellId = 0;
int selectedX = 0;
int selectedY = 0;
int wmId = 0;

COLORREF standartCell = RGB(171, 171, 171);
COLORREF numberCell = RGB(87, 103, 233);
COLORREF flagCell = RGB(239, 201, 77);
COLORREF bombCell = RGB(243, 16, 16);

HBRUSH test = CreateSolidBrush(bombCell);

int CheckCell(int y, int x)
{
    if (original_box[y][x] == 1) return 1;
    return 0;
}

int CalculateNearBombs(int y, int x)
{
    int sumBomb = 0;

    if (original_box[y][x] == 1) return -1;

    bool up = y != 0;
    bool down = y != length - 1;
    bool right = x != length - 1;
    bool left = x != 0;

    if (up) sumBomb += CheckCell(y - 1, x); // up
    if (down) sumBomb += CheckCell(y + 1, x); // down
    if (right) sumBomb += CheckCell(y, x + 1); // right
    if (left) sumBomb += CheckCell(y, x - 1); // left

    if (up && right) sumBomb += CheckCell(y - 1, x + 1);  // up right
    if (up && left) sumBomb += CheckCell(y - 1, x - 1);  // up left
    if (down && right) sumBomb += CheckCell(y + 1, x + 1);  // down right
    if (down && left) sumBomb += CheckCell(y + 1, x - 1);  // down left

    return sumBomb;
}

void CreateBox()
{
    srand(time(0));

    for (int i = 0; i < length; i++)
    {
        for (int k = 0; k < length; k++)
        {
            int a = rand() % 5;
            if (a == 4) a = 1;
            else a = 0;

            box[i][k] = a;
            original_box[i][k] = a;
        }
    }
    for (int i = 0; i < length; i++)
    {
        for (int k = 0; k < length; k++)
        {
            bombs_box[i][k] = CalculateNearBombs(i, k);
        }
    }
}

void DebugPlane()
{
    int startX = 750;
    int startY = 300;

    string plane;
    ofstream file("out.txt");

    for (int y = 0; y < length; y++)
    {
        plane = "";
        for (int x = 0; x < length; x++)
        {
            plane += to_string(original_box[y][x]);
            plane += "   ";
        }
        file << plane << endl;
    }
    file << endl;
    for (int y = 0; y < length; y++)
    {
        plane = "";
        for (int x = 0; x < length; x++)
        {
            plane += to_string(bombs_box[y][x]);
            plane += "   ";
        }
        file << plane << endl;
    }

    file.close();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CreateBox();
    DebugPlane();

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT210326, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT210326));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT210326));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT210326);
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

//========

void WriteText(HDC hdc, const wchar_t *const text, int text2, int x, int y)
{
    WCHAR buffer[100];
    swprintf(buffer, 100, text, text2);
    TextOut(hdc, x, y, buffer, wcslen(buffer));
}

int GetCellIndex(int btnId)
{
    for (int i = 0; i < Cells.size(); i++)
    {
        if (Cells[i].first == btnId) return i;
    }
    return -1;
}

int GetIdFromPos(int y, int x)
{
    return y * length + x + 1000;
}

void UnlockNullCells(HWND hWnd, int y, int x)
{
    bool up = y != 0;
    bool down = y != length - 1;
    bool right = x != length - 1;
    bool left = x != 0;

    WCHAR buffer[100];
    
    HWND hButton = GetDlgItem(hWnd, GetIdFromPos(y, x));

    swprintf(buffer, 100, L"%d", bombs_box[y][x]);
    SetWindowText(hButton, buffer);

    box[y][x] = -1;

    if (up && bombs_box[y - 1][x] != -1)
    {
        hButton = GetDlgItem(hWnd, GetIdFromPos(y - 1, x));

        swprintf(buffer, 100, L"%d", bombs_box[y - 1][x]);
        SetWindowText(hButton, buffer);

        box[y - 1][x] = -1;
    }
    if (down && bombs_box[y + 1][x] != -1)
    {
        hButton = GetDlgItem(hWnd, GetIdFromPos(y + 1, x));

        swprintf(buffer, 100, L"%d", bombs_box[y + 1][x]);
        SetWindowText(hButton, buffer);

        box[y + 1][x] = -1;
    }
    if (right && bombs_box[y][x + 1] != -1)
    {
        hButton = GetDlgItem(hWnd, GetIdFromPos(y, x + 1));

        swprintf(buffer, 100, L"%d", bombs_box[y][x + 1]);
        SetWindowText(hButton, buffer);

        box[y][x + 1] = -1;
    }
    if (left && bombs_box[y][x - 1] != -1)
    {
        hButton = GetDlgItem(hWnd, GetIdFromPos(y, x - 1));

        swprintf(buffer, 100, L"%d", bombs_box[y][x - 1]);
        SetWindowText(hButton, buffer);

        box[y][x - 1] = -1;
    }
}

bool CheckWin()
{
    for (int y = 0; y < length; y++)
    {
        for (int x = 0; x < length; x++)
        {
            if (original_box[y][x] == 1 && box[y][x] != -2) return false;

            if (original_box[y][x] == 0 && box[y][x] != -1) return false;
        }
    }
    return true;
}

void SetButtonColor(HWND hWnd, int id, COLORREF backColor, COLORREF textColor = RGB(255, 255, 255))
{
    HWND hButton = GetDlgItem(hWnd, id);

    auto it = CellsColor.find(id);
    if (it != CellsColor.end() && it->second.hBrush) DeleteObject(it->second.hBrush);

    HBRUSH newBrush = CreateSolidBrush(backColor);

    ButtonData data;
    data.hBrush = newBrush;
    data.textColor = textColor;
    CellsColor[id] = data;

    InvalidateRect(hButton, NULL, TRUE);
    UpdateWindow(hButton);
}

void DrawPlane(HWND hWnd)
{
    int iter = 0;
    for (int y = 1; y <= length; y++)
    {
        for (int x = 1; x <= length; x++)
        {
            int xPos = PlanePos + CellSize * x + CellSpace * x;
            int yPos = PlanePos + CellSize * y + CellSpace * y;

            HWND button = CreateWindowW(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, xPos, yPos, CellSize, CellSize, hWnd, (HMENU)ID_CELL(iter), hInst, NULL);
            Cells.emplace_back( ID_CELL(iter), button );

            //Cells.emplace_back(ID_CELL(iter), button);

            //SetButtonColor(hWnd, ID_CELL(iter), standartCell, numberCell);

            iter++;
        }
    }
}



void ReDrawPlane(HWND hWnd)
{
    for (int y = 0; y < length; y++)
    {
        for (int x = 0; x < length; x++)
        {
            int cell = box[y][x];

            if (cell == -2)
            {
                int id = GetIdFromPos(y, x);
                WCHAR buffer[10];

                HWND hButton = GetDlgItem(hWnd, id);

                swprintf(buffer, 10, L"$");
                SetWindowText(hButton, buffer);
            }

            if (cell == -1 && bombs_box[y][x] == 0)
            {
                UnlockNullCells(hWnd, y, x);
            }

            if (cell == 0 || cell == 1)
            {
                int id = GetIdFromPos(y, x);
                WCHAR buffer[10];

                HWND hButton = GetDlgItem(hWnd, id);

                swprintf(buffer, 10, L"");
                SetWindowText(hButton, buffer);

            }
        }
    }
}

//========

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    

    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindowW(L"BUTTON", L"Вскопать", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 750, 100, 120, 30, hWnd, (HMENU)900, hInst, NULL);
        CreateWindowW(L"BUTTON", L"Флажок", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 750, 140, 120, 30, hWnd, (HMENU)901, hInst, NULL);
        CreateWindowW(L"BUTTON", L"Убрать флажок", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 750, 180, 120, 30, hWnd, (HMENU)902, hInst, NULL);

        DrawPlane(hWnd);
    }
    break;
    case WM_CTLCOLORBTN:
    {
        HDC hdc = (HDC)wParam;
        HWND hButton = (HWND)lParam;
        int id = GetDlgCtrlID(hButton);

        auto it = CellsColor.find(id);
        if (it != CellsColor.end())
        {
            SetTextColor(hdc, it->second.textColor);
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)it->second.hBrush;
        }
        break;
    }
    case WM_COMMAND:
        {
            wmId = LOWORD(wParam);
            
            if (1000 < wmId && wmId < 1000 + length * length)
            {
                int index = GetCellIndex(wmId);

                if (index == -1) exit(1);

                selectedCellId = wmId;
                selectedX = (wmId - 1000) % length;
                selectedY = (wmId - 1000) / length;

                InvalidateRect(hWnd, NULL, TRUE);
            }

            HWND hButton = GetDlgItem(hWnd, selectedCellId);
            if (wmId == 900) 
            {
                if (original_box[selectedY][selectedX] == 1)
                {
                    MessageBox(hWnd, L"Вы подорвались! Игра окончена!", L"Сапёр", NULL);
                    exit(0);
                }
                else if (original_box[selectedY][selectedX] == 0)
                {
                    box[selectedY][selectedX] = -1;
                    UnlockNullCells(hWnd, selectedY, selectedX);
                    ReDrawPlane(hWnd);
                }
                
            }

            else if (wmId == 901 && box[selectedY][selectedX] != -1)
            {
                box[selectedY][selectedX] = -2;
                SetWindowText(hButton, L"$");
            }

            else if (wmId == 902 && box[selectedY][selectedX] != -1)
            {
                box[selectedY][selectedX] = original_box[selectedY][selectedX];
                SetWindowText(hButton, L"⁐");
            }

            if (CheckWin())
            {
                MessageBox(hWnd, L"Вы обезвредили все мины! Поздравляю с победой!", L"Сапёр", NULL);
                exit(0);
            }

            InvalidateRect(hWnd, NULL, TRUE);
            
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DeleteObject(hButton);
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

            WriteText(hdc, L"Добро пожаловать в Сапёр! Найдите бомбы и выйграйте игру.", NULL, 200, 20);
            WriteText(hdc, L"X: %d", selectedX, 750, 30);
            WriteText(hdc, L"Y: %d", selectedY, 750, 50);
            WriteText(hdc, L"ID: %d", wmId, 750, 70);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        for (auto& pair : CellsColor)
        {
            if (pair.second.hBrush) DeleteObject(pair.second.hBrush);
        }
        CellsColor.clear();
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
