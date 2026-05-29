
#include "framework.h"
#include "MyProject_04_04_26 - maze.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "json.hpp"

#define MAX_LOADSTRING 100

using namespace std;
using json = nlohmann::json;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int MazeWidth = 31;
int MazeHeight = 31;

const int CellSize = 20;
const int PlanePos = 100;
const string Difficulty = "easy";

int px = 1, py = 1;
bool isFinish = false;
bool isStop = false;

int count = 0;

HBRUSH Brushes[5];
//int level[MazeHeight][MazeWidth];

vector<vector<int>> level(MazeHeight, vector<int>(MazeWidth));

string executeCurl(const string& url, const string& jsonData) {
    string tempFile = "response.json";
    string tempJsonFile = "data.json";

    ofstream jsonFile(tempJsonFile);
    jsonFile << jsonData;
    jsonFile.close();

    string curl = "C:/curl/bin/curl.exe";

    stringstream cmd;
    cmd << curl << " -s -X POST -H \"Content-Type: application/json\" ";
    cmd << "--max-time 15 --connect-timeout 10 ";  
    cmd << "-d @" << tempJsonFile << " \"" << url << "\" -o \"" << tempFile << "\"";

    int result = system(cmd.str().c_str());

    ifstream file(tempFile);
    string response;
    stringstream buffer;
    buffer << file.rdbuf();
    response = buffer.str();
    file.close();

    //remove(tempFile.c_str());
    //remove(tempJsonFile.c_str());

    return response;
}

int charToMazeValue(const string& cell, int y, int x) {
    if (cell == "#") return 1;
    if (cell == " ") return 0;
    if (cell == "E") return 3;
    if (cell == "S")
    {
        px = x;
        py = y;
        return 0;
    }
    return 1;
}

void GetResponce()
{
    json request;
    request["width"] = MazeWidth;
    request["height"] = MazeHeight;
    request["difficulty"] = Difficulty;

    string response = executeCurl("https://elysiatools.com/ru/api/tools/maze-generator", request.dump());

    try
    {
        if (!response.empty()) {
            json data = json::parse(response);

            if (data.contains("data") && data["data"].contains("result")) {
                auto maze = data["data"]["result"]["maze"];

                level.clear();
                for (int i = 0; i < MazeHeight; i++) {
                    vector<int> row;  
                    auto mazeRow = maze[i];

                    for (int j = 0; j < MazeWidth; j++) {
                        string cellValue = mazeRow[j];  
                        row.push_back(charToMazeValue(cellValue, i, j));  
                    }
                    level.push_back(row);  
                }
            }
        }
        else exit(2);
    }
    catch (const exception& e)
    {
        exit(2);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GetResponce();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPROJECT040426MAZE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT040426MAZE));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MYPROJECT040426MAZE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYPROJECT040426MAZE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

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

    case WM_KEYDOWN:
    {
        if (!isFinish && !isStop)
        {
            if (wParam == VK_UP && level[py - 1][px] != 1) py--;

            else if (wParam == VK_DOWN && level[py + 1][px] != 1) py++;
           
            if (wParam == VK_LEFT && level[py][px - 1] != 1) px--;
                
            else if (wParam == VK_RIGHT && level[py][px + 1] != 1) px++;
            
            InvalidateRect(hWnd, NULL, FALSE);

        }
        if (level[py][px] == 3)
        {
            isStop = true;

            MazeHeight /= 2;
            MazeWidth /= 2;

            if (MazeHeight % 2 == 0) MazeHeight++;
            if (MazeWidth % 2 == 0) MazeWidth++;

            if (MazeHeight < 5 || MazeWidth < 5) exit(0);

            GetResponce();

            isStop = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }

        if (level[py][px] == 4)
        {
            isFinish = true;

        }


    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Brushes[0] = CreateSolidBrush(RGB(222, 222, 222));
        Brushes[1] = CreateSolidBrush(RGB(50, 50, 50));
        Brushes[2] = CreateSolidBrush(RGB(0, 255, 0));
        Brushes[3] = CreateSolidBrush(RGB(255, 0, 255));
        Brushes[4] = CreateSolidBrush(RGB(255, 0, 0));

        ;
        for (int y = 0; y < MazeHeight; y++)
        {
            for (int x = 0; x < MazeWidth; x++)
            {
                if (level[y][x] == 0) SelectObject(hdc, Brushes[0]);

                else if (level[y][x] == 1 || level[y][x] == -1) SelectObject(hdc, Brushes[1]);

                else if (level[y][x] == 2) SelectObject(hdc, Brushes[2]);

                else if (level[y][x] == 3) SelectObject(hdc, Brushes[3]);

                else if (level[y][x] == 4) SelectObject(hdc, Brushes[4]);

                Rectangle(hdc, PlanePos + x * CellSize, PlanePos + y * CellSize, PlanePos + (x + 1) * CellSize, PlanePos + (y + 1) * CellSize);
            }
        }

        if (isFinish == true) {
            TextOutA(hdc, 130, 150, "YOU WIN", strlen("YOU WIN"));
        }


        HBRUSH playerBrush = CreateSolidBrush(RGB(255, 255, 0)); // Красный цвет квадрата игрока

        SelectObject(hdc, playerBrush);
        Ellipse(hdc, PlanePos + px * CellSize, PlanePos + py * CellSize, PlanePos + (px + 1) * CellSize, PlanePos + (py + 1) * CellSize);

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