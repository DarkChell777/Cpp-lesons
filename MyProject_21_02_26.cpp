#include "framework.h"
#include "MyProject_21_02_26.h"
#include <string>
#include <windows.h>
#include <vector>
#include <sstream>

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


vector<int> libruaryBookId;
vector<string> libruaryBookName;
vector<string> libruaryBookContent;
int cellWidth = 200;
int cellHeigth = 150;
int horizontalScrollPos = 0;
int maxScrollPos = 0;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    setlocale(LC_ALL, "Rissan");

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT210226, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT210226));

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

wstring utf8_to_wstring(const string& utf8str)
{
    if (utf8str.empty()) return wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), (int)utf8str.length(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), (int)utf8str.length(), &wstrTo[0], size_needed);
    return wstrTo;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPROJECT210226));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYPROJECT210226);
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

int FindBookIndex(int id)
{
    for (int i = 0; i < libruaryBookId.size(); i++)
    {
        if (id == libruaryBookId[i]) return i;
    }
    return -1;
}

void AddBook(int id, string name, string content)
{
    if (FindBookIndex(id) != -1)
    {
        stringstream desc;
        desc << "Такой ID уже существует!";
        return;
    }
    libruaryBookId.push_back(id);
    libruaryBookName.push_back(name);
    libruaryBookContent.push_back(content);
}

void InitializeData()
{
    AddBook(3167, "Kolobok", "I like to read books.");
    AddBook(435, "February", "The plane landed safely.");
    AddBook(6543, "Colonel ", "My mom cooks well.");
    AddBook(5422, "Queue", "She is a doctor.");
    AddBook(235, "Literally", "She is not feeling well today.");
    AddBook(54356, "Complement", "Do you speak English?");
    AddBook(5356, "Compliment", "The sky is clear today.");
    
}

void DrawCells(HDC hdc, int windowWidth, int WindowHeight)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
    HBRUSH hTextBrush = CreateSolidBrush(RGB(0, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, 
                            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    //HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    int startCell = horizontalScrollPos / cellWidth;
    int endCell = min((int)libruaryBookId.size(), startCell + (windowWidth / cellWidth) + 2);

    for (int i = startCell; i < endCell; i++)
    {
        int x = i * cellWidth - horizontalScrollPos;

        if (x + cellWidth > 0 && x < windowWidth)
        {
            Rectangle(hdc, x, 50, x + cellWidth, 50 + cellHeigth);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0, 0, 0));

            wstring text = L"ID: " + to_wstring(libruaryBookId[i]);
            TextOutW(hdc, x + 10, 60, text.c_str(), text.length());

            text = L"Name: " + utf8_to_wstring(libruaryBookName[i]);
            TextOutW(hdc, x + 10, 85, text.c_str(), text.length());

            RECT textRect = { x + 10, 110, x + cellWidth - 10, 50 + cellHeigth - 20 };
            wstring descText = wstring(libruaryBookContent[i].begin(), libruaryBookContent[i].end());
            DrawTextW(hdc, descText.c_str(), descText.length(), &textRect, DT_WORDBREAK | DT_LEFT);
        }
    }

    //SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    DeleteObject(hTextBrush);
}

void SetupScrollBar(HWND hWnd, int windowWidth)
{
    int totalWidth = libruaryBookId.size() * cellWidth;
    int visableWeight = windowWidth;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;
    si.nMin = 0;
    si.nMax = totalWidth - 1;
    si.nPage = visableWeight;
    si.nPos = horizontalScrollPos;

    SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
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

    case WM_SIZE:
    {
        int windowWidth = LOWORD(lParam);
        SetupScrollBar(hWnd, windowWidth);
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
        return 0;
    }

    case WM_HSCROLL:
    {
        int windowWidth;
        RECT rect;

        GetClientRect(hWnd, &rect);
        windowWidth = rect.right - rect.left;

        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        GetScrollInfo(hWnd, SB_HORZ, &si);

        int xPos = si.nPos;

        switch (LOWORD(wParam))
        {
            case SB_LINELEFT:
                xPos -= 20;
                break;

            case SB_LINERIGHT:
               xPos += 20;
               break;

            case SB_PAGELEFT:
                xPos -= windowWidth;
                break;

            case SB_PAGERIGHT:
                xPos += windowWidth;
                break;

            case SB_THUMBTRACK:
                xPos = si.nTrackPos;
                break;
        }

        int maxPos = max(0, si.nMax - (int)si.nPage + 1);
        if (xPos < 0) xPos = 0;
        if (xPos > maxPos) xPos = maxPos;

        if (xPos != si.nPos)
        {
            horizontalScrollPos = xPos;

            si.fMask = SIF_POS;
            si.nPos = xPos;
            SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            SetConsoleCP(1251);
            SetConsoleOutputCP(1251);

            InitializeData();
            RECT rect;
            GetClientRect(hWnd, &rect);

            DrawCells(hdc, rect.right - rect.left, rect.bottom - rect.top);

            const int N = 20;
            int array[N][N];
            for (int i = 0; i < N; i++)
            {
                for (int k = 0; k < N; k++)
                {
                    array[i][k] = i * k;
                }
            }
            int x1 = 50, y1 = 220;
            int size = 20;

            HBRUSH hBrush;

            for (int i = 0; i < N; i++)
            {
                for (int k = 0; k < N; k++)
                {
                    hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
                    SelectObject(hdc, hBrush);
                    Rectangle(hdc, x1 + size * k, y1 + size * i, x1 + (k + 1) * size, y1 + (i + 1) * size);
                    DeleteObject(hBrush);
                }
            }

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        SendMessage(hWnd, WM_HSCROLL, delta > 0 ? SB_LINELEFT : SB_LINERIGHT, 0);
        return 0;
    }

    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        return 1;
    }

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
