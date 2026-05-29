#include "framework.h"
#include "MyProject_29_11_25.h"

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

#define ID_EDIT 1001
#define ID_BUTTON 1002
HWND Edit, Edit2, Edit3, Button;
HWND hCombo;
int number1, number2, bilets;
bool isFirst = TRUE;

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
    LoadStringW(hInstance, IDC_MYPROJECT291125, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT291125));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT291125));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT291125);
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
            Edit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 140, 10, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);
            Edit2 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 280, 10, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

            Button = CreateWindowW(L"BUTTON", L"Сравнить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 10, 100, 25, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
            
            hCombo = CreateWindowW(L"COMBOBOX", L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 550, 10, 100, 250, hWnd, (HMENU)ID_EDIT, hInst, NULL);
            Edit3 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 160, 500, 120, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"1 задание");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"2 задание");
            SendMessageW(hCombo, CB_SETCURSEL, 0, 0);
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
            case ID_BUTTON:
                if (wmId == ID_EDIT && HIWORD(wParam) == CBN_SELCHANGE)
                {
                    int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
                    if (index == 1)
                    {
                        isFirst == FALSE;
                    }
                    else
                    {
                        isFirst == TRUE;
                    }
                }
                WCHAR buffer[100];
                WCHAR buffer2[100];
                WCHAR buffer3[100];
                GetWindowText(Edit, buffer, 100);
                GetWindowText(Edit2, buffer2, 100);
                GetWindowText(Edit3, buffer3, 100);
                number1 = _wtoi(buffer);
                number2 = _wtoi(buffer2);
                bilets = _wtoi(buffer3);

                InvalidateRect(hWnd, NULL, TRUE);
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

            HPEN hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
            SelectObject(hdc, hPen);

            HFONT hFont = CreateFont(36, 0, 0, 0, 
                FW_NORMAL, 
                FALSE, FALSE, FALSE, 
                DEFAULT_CHARSET, 
                OUT_DEFAULT_PRECIS, 
                CLIP_DEFAULT_PRECIS, 
                DEFAULT_QUALITY, 
                DEFAULT_PITCH | FF_SWISS, 
                L"Arial");

            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            if (isFirst)
            {
                if (number1 > 1000000 || number2 > 1000000)
                {
                    MessageBox(hWnd, L"Слишком большое число!", L"Подозрительное окно -_-", MB_OK | MB_ICONERROR);
                    break;
                }
                WCHAR text[20];
                swprintf(text, 100, L"Введите 2 числа:");
                TextOut(hdc, 10, 15, text, wcslen(text));

                HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

                Ellipse(hdc, 100, 100, 300, 300);
                WCHAR text2[20];
                swprintf(text2, 100, L"%d", number1);
                TextOut(hdc, 150, 180, text2, wcslen(text2));

                Ellipse(hdc, 500, 100, 700, 300);
                WCHAR text3[20];
                swprintf(text3, 100, L"%d", number2);
                TextOut(hdc, 550, 180, text3, wcslen(text3));

                if (number1 > number2)
                {
                    MoveToEx(hdc, 380, 130, NULL);
                    LineTo(hdc, 450, 180);
                    LineTo(hdc, 380, 230);
                }
                else if (number1 < number2)
                {
                    MoveToEx(hdc, 450, 130, NULL);
                    LineTo(hdc, 380, 180);
                    LineTo(hdc, 450, 230);
                }
                else
                {
                    MoveToEx(hdc, 340, 150, NULL);
                    LineTo(hdc, 440, 150);
                    MoveToEx(hdc, 340, 190, NULL);
                    LineTo(hdc, 440, 190);
                }
                //break;
                SelectObject(hdc, hOldFont);
            }
            

            WCHAR text4[30];
            swprintf(text4, 100, L"Количество билетов: ");
            TextOut(hdc, 10, 500, text4, wcslen(text4));

            int bilet_1, bilet_2, bilet_3;
            bilet_3 = bilets / 60;
            bilet_2 = bilets % 60 / 10;
            bilet_1 = bilets % 60 % 10;

            WCHAR text5[100];
            swprintf(text5, 100, L"Количество билетов: %d - на 60 поездок, %d - на 10 поездок, %d - на 1 поездку", bilet_3, bilet_2, bilet_1);
            TextOut(hdc, 10, 540, text5, wcslen(text5));

            


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
