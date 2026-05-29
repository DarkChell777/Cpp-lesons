#include <string>
#include "framework.h"
#include "MyProject_31_01_26.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

#define ID_EDIT 998
#define ID_APPLY_BTN 999
#define ID_BUTTON 1000
#define ID_CAPCHA(n) (ID_BUTTON + (n))

HWND CapchaButtons[30];
bool CreatingButtons = false;

HWND Edit;
WCHAR buffer[100];

int x_min = 10, x_max = 1200;
int y_min = 130, y_max = 700;
int currentBtn = 0, lastBtn = 0;



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
    LoadStringW(hInstance, IDC_MYPROJECT310126, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT310126));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT310126));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT310126);
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

void CreateCapchaButtons(HWND hParent)
{
    for (int i = 0; i < 30; i++)
    {
        WCHAR val[4];
        wsprintf(val, L"%d", i + 1);

        int x = x_min + (rand() % x_max);
        int y = y_min + (rand() % y_max);

        CapchaButtons[i] = CreateWindowW(L"BUTTON", val, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, 50, 50, hParent, (HMENU)ID_CAPCHA(i + 1), hInst, NULL);

    }
}

void ReloadButtons(HWND hParent)
{
    for (int i = 0; i < 30; i++)
    {
        if (CapchaButtons[i] != NULL)
        {
            HWND hButton = GetDlgItem(hParent, ID_BUTTON + i + 1);

            int x = x_min + (rand() % x_max);
            int y = y_min + (rand() % y_max);

            MoveWindow(hButton, x, y, 50, 50, TRUE);
        }
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
        CreateWindowW(L"BUTTON", L"Перемешать", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 550, 10, 100, 25, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
        
        CreateWindowW(L"BUTTON", L"Подтвердить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 10, 100, 25, hWnd, (HMENU)ID_APPLY_BTN, hInst, NULL);

        Edit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 810, 10, 220, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

        if (!CreatingButtons)
        {
            CreateCapchaButtons(hWnd);

            CreatingButtons = true;
        }

    } break;

    case WM_COMMAND:
        {
            int ctrlID = LOWORD(wParam);
            if (ID_BUTTON < ctrlID && ctrlID < ID_BUTTON + 31)
            {
                currentBtn = ctrlID - ID_BUTTON;

                InvalidateRect(hWnd, NULL, TRUE);
            }

            if (ctrlID == ID_BUTTON)
            {
                ReloadButtons(hWnd);
            }

            if (ctrlID == ID_APPLY_BTN)
            {
                if (lastBtn + 1 == currentBtn && currentBtn != 0)
                {
                    HWND hButton = GetDlgItem(hWnd, 1000 + currentBtn);

                    ShowWindow(hButton, SW_HIDE);

                    lastBtn = currentBtn;

                    CapchaButtons[lastBtn] = NULL;
                }

                else if (lastBtn + 1 != currentBtn)
                {
                    MessageBox(hWnd, L"Вы робот!", L"Ошибка!", MB_OK | MB_ICONERROR);

                    ReloadButtons(hWnd);
                }

                if (lastBtn == 30)
                {
                    MessageBox(hWnd, L"Поздравляю! Вы доказали что вы человек. Наша компания очень заботится о наших пользхователях! Оцените качество приложения от 10 до 10.", L"Госуслуги", MB_OK | MB_ICONINFORMATION);
                }
            }
            
            switch (ctrlID)
            {
            case ID_BUTTON:
            {
                GetWindowText(Edit, buffer, 100);

            } break;

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
            
            WCHAR text[40];
            swprintf(text, 100, L"Подтвердите что вы не робот: ");
            TextOut(hdc, 300, 100, text, wcslen(text));

            WCHAR text2[40];
            swprintf(text2, 100, L"Ваше число: %d", currentBtn);
            TextOut(hdc, 20, 100, text2, wcslen(text2));

            WCHAR text3[60];
            swprintf(text3, 100, L"Прежде чем войти в Госуслуги, пройдите капчу!");
            TextOut(hdc, 20, 10, text3, wcslen(text3));

            WCHAR text4[20];
            swprintf(text4, 100, L"Ваше ФИО: ");
            TextOut(hdc, 700, 15, text4, wcslen(text4));

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
