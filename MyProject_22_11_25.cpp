// MyProject_22_11_25.cpp : Определяет точку входа для приложения.
//
#include <cstdlib>
#include "framework.h"
#include "MyProject_22_11_25.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#define ID_EDIT 1001
#define ID_BUTTON 1002
HWND Edit, Edit2, Edit3, Edit4, eButton;
int N, delay;
int randomX, randomY;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_MYPROJECT221125, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT221125));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT221125));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT221125);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        
        Edit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 100, 10, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
        Edit2 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 415, 10, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

        Edit3 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 280, 50, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
        Edit4 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 415, 50, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

        eButton = CreateWindowW(L"BUTTON", L"Нарисовать", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 550, 10, 100, 25, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
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

            case ID_BUTTON:
                WCHAR buffer[100];
                WCHAR buffer2[100];
                WCHAR buffer3[100];
                WCHAR buffer4[100];
                GetWindowText(Edit, buffer, 100);
                GetWindowText(Edit2, buffer2, 100);
                GetWindowText(Edit3, buffer3, 100);
                GetWindowText(Edit4, buffer4, 100);
                N = _wtoi(buffer);
                delay = _wtoi(buffer2);
                randomX = _wtoi(buffer3);
                randomY = _wtoi(buffer4);


                if (N < 0)
                {
                    MessageBox(hWnd, L"Введите число", L"Ошибка!", MB_OK | MB_ICONERROR);
                }
                else
                {
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                break;

            case ID_EDIT:
            
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
            
            WCHAR text2[15];
            swprintf(text2, 100, L"Количество: ");
            TextOut(hdc, 10, 15, text2, wcslen(text2));

            WCHAR text3[30];
            swprintf(text3, 100, L"Коэффицент погрешности: ");
            TextOut(hdc, 230, 15, text3, wcslen(text3));

            WCHAR text4[40];
            swprintf(text4, 100, L"Максимальное рандомное число(X и Y):");
            TextOut(hdc, 10, 50, text4, wcslen(text4));

            int random, random2;
            for (int i = 0; i < N; i++) 
            {
                if (N == 0 ) { break; }
                
                if (N > 60)
                {
                    MessageBox(hWnd, L"Слишком большое число! Максимум: 60", L"Подозрительное окно -_-", MB_OK | MB_ICONERROR);
                    break;
                }
                if (randomX == 0 || randomY == 0 || delay == 0)
                {
                    MessageBox(hWnd, L"Укажите все значения!", L"Подозрительное окно -_-", MB_OK | MB_ICONERROR);
                    break;
                }
                random = 1 + (rand() % randomX);
                random2 = 1 + (rand() % randomY);

                HPEN hPen;
                hPen = CreatePen(PS_SOLID, 2, RGB(rand() % 255, rand() % 255, rand() % 255));
                SelectObject(hdc, hPen);
                if (N % 2 == 0)
                {
                    Rectangle(hdc, random + 100 + (rand() % delay), random2 + 100 + (rand() % delay), random + 300 + (rand() % delay), random2 + 200 + (rand() % delay));
                }
                else
                {
                    Ellipse(hdc, random + 100 + (rand() % delay), random2 + 100 + (rand() % delay), random + 300 + (rand() % delay), random2 + 200 + (rand() % delay));
                }
                DeleteObject(hPen);
            }
            

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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
