#include "framework.h"
#include "GameForTwoPlayer.h"
#include <windows.h>
#include <time.h>
#include <utility>
#include <comdef.h>
#include <gdiplus.h>
#include <cmath>
#include <string>

#include "audiere.h"
#include<conio.h>

#pragma comment(lib, "gdiplus.lib")

#define MAX_LOADSTRING 100

using namespace std;
using namespace Gdiplus;
using namespace audiere;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ULONG_PTR gdiplusToken;

const int IconWidth = 1600 / 6;
const int IconHeight = 1600 / 4;
const int IconPixelsSize = 216;
const int ImageStepX = 61;
const int ImageStepY = 61;


const int IconBoard = 5;
const int IconSize = 100;
const int PlaneStartX = 100;
const int PlaneStartY = 200;
const int PlaneEndX = 1200;
const int PlaneEndY = 800;

const int HealthPosX = 100;
const int HealthPosX2 = 800;
const int HealthPosY = 20;
const int HealthPieceLengthX = 50;
const int HealthPieceLengthY = 30;
const int HealthBoard = 3;

const int rechargePosX = 100;
const int rechargePosX2 = 800;
const int rechargePosY = 70;
const int rechargePieceLengthX = 40;
const int rechargePieceLengthY = 20;
const int rechargeBoard = 2;


const int playerSpeed = 15;
const int shootLength = 100;
const float shootRotateInterval = 2.3f;
const float gradusStepInMiliseconds = 360 / shootRotateInterval / 1000 * 2;
const HPEN hShootPen = CreatePen(PS_SOLID, 20, RGB(254, 127, 0));

Image* playerIcons = NULL;
HBRUSH hBoardBrush;
int currentIconIndex = 2;
int currentIconIndex2 = 7;
pair<int, int> playerPosition = {200, 200};
pair<int, int> playerPosition2 = {400, 200};
int playerShootGradus = 0;
int playerShootGradus2 = 0;
int shootDirection = 1;
int shootDirection2 = -1;
int playerHealth = 5;
int playerHealth2 = 5;
float rechargeTime = 0;
float rechargeTime2 = 0;

const float rechargeTargetTime = 1500;
const int shootSpeed = 16;
const int shootSize = 7;
const int shootMaxBounces = 2;

bool GameStarted = false;
bool GamePause = false;

bool spacePressed = false;
bool enterPressed = false;
bool escapePressed = false;

HWND MenuButtons[5];
HWND GameButtons[2];

struct Ray {
    string type;
    double x, y;           
    double dx, dy;         
    double angle;          
    int bounces;           
    bool active;           
    double speed;          
};

const string firstRay = "first";
const string secondRay = "second";
Ray shootRay;
Ray shootRay2;

OutputStreamPtr soundtrack;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiStartupInput, NULL);
    srand(time(0));

    AudioDevicePtr device = OpenDevice();
    soundtrack = OpenSound(device, "Sound.wav", true);
    soundtrack->setRepeat(true);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEFORTWOPLAYER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEFORTWOPLAYER));

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

// ----------------------

void CalculateBounce(Ray& ray, int hitEdge)
{
    if (hitEdge == 0) ray.dx = -ray.dx;
    else ray.dy = -ray.dy;
    
    ray.angle = atan2(ray.dy, ray.dx) * 180.0 / 3.14159;
    ray.bounces++;
}

bool IsPointInCircle(const Ray& ray, string type) 
{
    pair<int, int> circle;
    int radius = IconSize / 2 + IconBoard;
    bool startedShot = ray.type == type && ray.bounces == 0;

    if (type == firstRay) circle = playerPosition;
    else circle = playerPosition2;

    double dx = ray.x - circle.first;
    double dy = ray.y - circle.second;
    double distance = dx * dx + dy * dy;

    return distance <= radius * radius && !startedShot;
}

void LaunchRay(Ray& ray, int startX, int startY, int angle, string type)
{
    ray.type = type;

    ray.x = startX;
    ray.y = startY;

    ray.angle = angle;
    double rad = ray.angle * 3.14159 / 180.0;

    ray.dx = cos(rad);
    ray.dy = sin(rad);

    ray.bounces = 0;
    ray.active = true;

    ray.speed = shootSpeed;
}

void CheckGameOver(HWND hWnd)
{
    if (playerHealth > 0 && playerHealth2 > 0) return;

    GamePause = true;

    HWND pauseBtn = GetDlgItem(hWnd, 901);
    EnableWindow(pauseBtn, false);
}

void UpdateRay(Ray& ray, HWND hWnd)
{
    if (!ray.active) return;

    double oldX = ray.x;
    double oldY = ray.y;

    ray.x += ray.dx * ray.speed;
    ray.y += ray.dy * ray.speed;

    bool bounced = false;
    int hitEdge = -1;

    if (ray.x <= PlaneStartX && ray.dx < 0) {
        ray.x = PlaneStartX;
        bounced = true;
        hitEdge = 0;
    }
    
    else if (ray.x >= PlaneEndX && ray.dx > 0) {
        ray.x = PlaneEndX;
        bounced = true;
        hitEdge = 0;
    }

    if (ray.y <= PlaneStartY && ray.dy < 0) {
        ray.y = PlaneStartY;
        bounced = true;
        hitEdge = 1;
    }
    
    else if (ray.y >= PlaneEndY && ray.dy > 0) {
        ray.y = PlaneEndY;
        bounced = true;
        hitEdge = 1;
    }

    if (bounced) 
    {
        if (ray.bounces < shootMaxBounces) CalculateBounce(ray, hitEdge);
        
        else ray.active = false;
    }

    if (IsPointInCircle(ray, firstRay))
    {
        ray.active = false;

        playerHealth--;
    }
    if (IsPointInCircle(ray, secondRay))
    {
        ray.active = false;

        playerHealth2--;
    }

    if (ray.x < PlaneStartX - 10 || ray.x > PlaneEndX + 10 || ray.y < PlaneStartY - 10 || ray.y > PlaneEndY + 10) ray.active = false;

    CheckGameOver(hWnd);
}

void DrawRay(Ray& ray, HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);

    if (ray.active) {
        HPEN hRayPen = CreatePen(PS_SOLID, 2, RGB(230, 35, 10));
        SelectObject(hdc, hRayPen);

        HBRUSH hRayBrush = CreateSolidBrush(RGB(230, 35, 10));
        SelectObject(hdc, hRayBrush);
        Ellipse(hdc, (int)ray.x - shootSize, (int)ray.y - shootSize, (int)ray.x + shootSize, (int)ray.y + shootSize);

        HPEN hTailPen = CreatePen(PS_SOLID, 1, RGB(219, 110, 10, 100));
        SelectObject(hdc, hTailPen);

        double backX = ray.x - ray.dx * 30;
        double backY = ray.y - ray.dy * 30;
        MoveToEx(hdc, (int)backX, (int)backY, NULL);
        LineTo(hdc, (int)ray.x, (int)ray.y);

        DeleteObject(hTailPen);
        DeleteObject(hRayBrush);
        DeleteObject(hRayPen);
    }
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

// ----------------------

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEFORTWOPLAYER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEFORTWOPLAYER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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

void KeyboardListen(HWND hWnd)
{
    if (playerHealth <= 0 || playerHealth2 <= 0)
    {
        GamePause = true;
        return;
    }

    if (GetAsyncKeyState('W') && 0x8000)
    {
        if (playerPosition.second - IconSize / 2 - IconBoard - playerSpeed > PlaneStartY) playerPosition.second -= playerSpeed;
        else playerPosition.second = PlaneStartY + IconSize / 2 + IconBoard;
    }
    else if (GetAsyncKeyState('S') && 0x8000)
    {
        if (playerPosition.second + IconSize / 2 + IconBoard + playerSpeed < PlaneEndY) playerPosition.second += playerSpeed;
        else playerPosition.second = PlaneEndY - IconSize / 2 - IconBoard;
    }
    if (GetAsyncKeyState(VK_UP) && 0x8000)
    {
        if (playerPosition2.second - IconSize / 2 - IconBoard - playerSpeed > PlaneStartY) playerPosition2.second -= playerSpeed;
        else playerPosition2.second = PlaneStartY + IconSize / 2 + IconBoard;
    }
    else if (GetAsyncKeyState(VK_DOWN) && 0x8000)
    {
        if (playerPosition2.second + IconSize / 2 + IconBoard + playerSpeed < PlaneEndY) playerPosition2.second += playerSpeed;
        else playerPosition2.second = PlaneEndY - IconSize / 2 - IconBoard;
    }
    
    if (GetAsyncKeyState(VK_SPACE))
    {
        if (spacePressed == false && rechargeTime > rechargeTargetTime)
        {
            spacePressed = true;
            shootDirection *= -1;
            rechargeTime = 0;
            LaunchRay(shootRay, playerPosition.first, playerPosition.second, playerShootGradus, firstRay);
        }
    }
    else spacePressed = false;

    if (GetAsyncKeyState(VK_RETURN) && 0x8000)
    {
        if (enterPressed == false && rechargeTime2 > rechargeTargetTime)
        {
            enterPressed = true;
            shootDirection2 *= -1;
            rechargeTime2 = 0;
            LaunchRay(shootRay2, playerPosition2.first, playerPosition2.second, playerShootGradus2, secondRay);
        }
    }
    else enterPressed = false;

    if (GetAsyncKeyState(VK_ESCAPE))
    {
        if (escapePressed == false)
        {
            GamePause = !GamePause;

            escapePressed = true;

            if (GamePause) soundtrack->stop();
            else soundtrack->play();
        }
    }
    else escapePressed = false;
}

Rect GetIconRect(int index)
{
    if (index < 0 || index > 23) index = 0;
    
    int col = index % 6;
    int row = index / 6;

    int x = col * IconPixelsSize + ImageStepX * col;
    int y = row * IconPixelsSize + ImageStepY * row;

    return Rect(x, y, IconPixelsSize, IconPixelsSize);
}

void DrawIcon(HDC hdc, int x, int y, int iconIndex, int size)
{
    Graphics graphics(hdc);

    
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBoardBrush);

    HPEN hPen = CreatePen(PS_SOLID, IconBoard * 0.2, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    if (playerIcons && playerIcons->GetLastStatus() == Ok)
    {
        Rect scrRect = GetIconRect(iconIndex);
        Rect destRect(x - size / 2, y - size / 2, size, size);
        Ellipse(hdc, x - size / 2 - IconBoard, y - size / 2 - IconBoard, x + size / 2 + IconBoard, y + size / 2 + IconBoard);

        graphics.DrawImage(playerIcons, destRect, scrRect.X, scrRect.Y, scrRect.Width, scrRect.Height, UnitPixel);
    }

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);

    DeleteObject(hPen);
}

void DrawShoots(HDC hdc)
{
    HPEN hOldPen = (HPEN)SelectObject(hdc, hShootPen);

    MoveToEx(hdc, playerPosition.first, playerPosition.second, NULL);
    int xTo = playerPosition.first + cos(playerShootGradus * 3.14159 / 180) * shootLength;
    int yTo = playerPosition.second + sin(playerShootGradus * 3.14159 / 180) * shootLength;
    
    LineTo(hdc, xTo, yTo);

    MoveToEx(hdc, playerPosition2.first, playerPosition2.second, NULL);
    xTo = playerPosition2.first + cos(playerShootGradus2 * 3.14159 / 180) * shootLength;
    yTo = playerPosition2.second + sin(playerShootGradus2 * 3.14159 / 180) * shootLength;

    LineTo(hdc, xTo, yTo);

    SelectObject(hdc, hOldPen);
}

void DrawMenu(HDC hdc)
{
    DrawIcon(hdc, 300, 300, currentIconIndex, 200);
    DrawIcon(hdc, 900, 300, currentIconIndex2, 200);

    WCHAR buffer[100];
    swprintf_s(buffer, 100, L"%d/23", currentIconIndex);
    TextOut(hdc, 280, 170, buffer, wcslen(buffer));

    swprintf_s(buffer, 100, L"%d/23", currentIconIndex2);
    TextOut(hdc, 880, 170, buffer, wcslen(buffer));

    if (currentIconIndex == currentIconIndex2) EnableWindow(MenuButtons[4], false);
    else EnableWindow(MenuButtons[4], true);
}

void DrawGame(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    HPEN hWhitePen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hWhitePen);
    
    Rectangle(hdc, PlaneStartX - 50, PlaneStartY - 100, PlaneEndX + 50, PlaneEndY + 100);

    SelectObject(hdc, hPen);
    Rectangle(hdc, PlaneStartX, PlaneStartY, PlaneEndX, PlaneEndY);

    DrawShoots(hdc);

    DrawIcon(hdc, playerPosition.first, playerPosition.second, currentIconIndex, IconSize);
    DrawIcon(hdc, playerPosition2.first, playerPosition2.second, currentIconIndex2, IconSize);

    HBRUSH hHealthBrush = CreateSolidBrush(RGB(255, 3, 31));
    HBRUSH hBoardBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(54, 54, 54));
    HBRUSH hRechargeBrush = CreateSolidBrush(RGB(225, 110, 14));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBoardBrush);

    Rectangle(hdc, HealthPosX - HealthBoard, HealthPosY - HealthBoard, HealthPosX + HealthPieceLengthX * 5 + HealthBoard + 4, HealthPosY + HealthPieceLengthY + HealthBoard); // 4 - between space
    Rectangle(hdc, HealthPosX2 - HealthBoard, HealthPosY - HealthBoard, HealthPosX2 + HealthPieceLengthX * 5 + HealthBoard + 4, HealthPosY + HealthPieceLengthY + HealthBoard);

    Rectangle(hdc, rechargePosX - rechargeBoard, rechargePosY - rechargeBoard, rechargePosX + rechargePieceLengthX + rechargeBoard, rechargePosY + rechargePieceLengthY + rechargeBoard);
    Rectangle(hdc, rechargePosX2 - rechargeBoard, rechargePosY - rechargeBoard, rechargePosX2 + rechargePieceLengthX + rechargeBoard, rechargePosY + rechargePieceLengthY + rechargeBoard);

    SelectObject(hdc, hBackgroundBrush);

    Rectangle(hdc, rechargePosX, rechargePosY, rechargePosX + rechargePieceLengthX, rechargePosY + rechargePieceLengthY);
    Rectangle(hdc, rechargePosX2, rechargePosY, rechargePosX2 + rechargePieceLengthX, rechargePosY + rechargePieceLengthY);

    SelectObject(hdc, hHealthBrush);

    for (int i = 0; i < playerHealth; i++) Rectangle(hdc, HealthPosX + HealthPieceLengthX * i, HealthPosY, HealthPosX + HealthPieceLengthX * (i + 1) + i, HealthPosY + HealthPieceLengthY);
    for (int i = 0; i < playerHealth2; i++) Rectangle(hdc, HealthPosX2 + HealthPieceLengthX * i, HealthPosY, HealthPosX2 + HealthPieceLengthX * (i + 1) + i, HealthPosY + HealthPieceLengthY);

    SelectObject(hdc, hRechargeBrush);

    float rechargeProgress = rechargeTime > rechargeTargetTime ? 1 : rechargeTime / rechargeTargetTime;
    float rechargeProgress2 = rechargeTime2 > rechargeTargetTime ? 1 : rechargeTime2 / rechargeTargetTime;

    Rectangle(hdc, rechargePosX, rechargePosY, rechargePosX + rechargePieceLengthX * rechargeProgress, rechargePosY + rechargePieceLengthY);
    Rectangle(hdc, rechargePosX2, rechargePosY, rechargePosX2 + rechargePieceLengthX * rechargeProgress2, rechargePosY + rechargePieceLengthY);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hRechargeBrush);
    DeleteObject(hBackgroundBrush);
    DeleteObject(hHealthBrush);
    DeleteObject(hBoardBrush);
    DeleteObject(hWhitePen);
    DeleteObject(hPen);

    WCHAR buffer[100];
    swprintf_s(buffer, 100, L"%d", playerShootGradus);
    TextOut(hdc, 50, 50, buffer, wcslen(buffer));

    if (GamePause)
    {
        swprintf_s(buffer, 100, L"PAUSE");
        TextOut(hdc, 150, 120, buffer, wcslen(buffer));
    }

    DrawRay(shootRay, hdc);
    DrawRay(shootRay2, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        playerIcons = new Image(L"PlayerIcons.png");

        MenuButtons[0] = CreateWindow(L"BUTTON", L"<", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 130, 230, 30, 100, hWnd, (HMENU)1000, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        MenuButtons[1] = CreateWindow(L"BUTTON", L">", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 430, 230, 30, 100, hWnd, (HMENU)1001, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        MenuButtons[2] = CreateWindow(L"BUTTON", L"<", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 730, 230, 30, 100, hWnd, (HMENU)1002, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        MenuButtons[3] = CreateWindow(L"BUTTON", L">", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 1030, 230, 30, 100, hWnd, (HMENU)1003, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        MenuButtons[4] = CreateWindow(L"BUTTON", L"Start Game", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 600, 50, 100, 50, hWnd, (HMENU)900, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        
        GameButtons[0] = CreateWindow(L"BUTTON", L"Pause", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 400, 50, 100, 50, hWnd, (HMENU)901, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        GameButtons[1] = CreateWindow(L"BUTTON", L"Exit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 600, 50, 100, 50, hWnd, (HMENU)902, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
    
        playerPosition = { PlaneStartX + 100, (PlaneEndY - PlaneStartY) / 2 + PlaneStartY };
        playerPosition2 = { PlaneEndX - 100, (PlaneEndY - PlaneStartY) / 2 + PlaneStartY};

        SetTimer(hWnd, 1, 16, NULL);
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            if (wmId == 1000)
            {
                currentIconIndex--;
                if (currentIconIndex < 0) currentIconIndex = 23;
            }
            else if (wmId == 1001)
            {
                currentIconIndex++;
                if (currentIconIndex > 23) currentIconIndex = 0;
            }
            else if (wmId == 1002)
            {
                currentIconIndex2--;
                if (currentIconIndex2 < 0) currentIconIndex2 = 23;
            }
            else if (wmId == 1003)
            {
                currentIconIndex2++;
                if (currentIconIndex2 > 23) currentIconIndex2 = 0;
            }
            else if (wmId == 900)
            {
                GameStarted = true;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            if (wmId == 901)
            {
                GamePause = !GamePause;

                if (GamePause) soundtrack->stop();
                else soundtrack->play();
            }
            else if (wmId == 902)
            {
                GameStarted = false;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            InvalidateRect(hWnd, NULL, FALSE);

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
        if (wParam == 1 && !GamePause && GameStarted)
        {
            playerShootGradus += gradusStepInMiliseconds * 16 * shootDirection;
            playerShootGradus2 += gradusStepInMiliseconds * 16 * shootDirection2;
            if (playerShootGradus > 360) playerShootGradus %= 360;
            if (playerShootGradus < 0) playerShootGradus = 360 - playerShootGradus % 360;
            
            if (playerShootGradus2 > 360) playerShootGradus2 %= 360;
            if (playerShootGradus2 < 0) playerShootGradus2 = 360 - playerShootGradus2 % 360;

            if (shootRay.active) UpdateRay(shootRay, hWnd);
            if (shootRay2.active) UpdateRay(shootRay2, hWnd);

            rechargeTime += 16;
            rechargeTime2 += 16;

            InvalidateRect(hWnd, NULL, FALSE);
        }
        KeyboardListen(hWnd);
        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            

            if (GameStarted)
            {
                if (hBoardBrush == NULL)
                {
                    hBoardBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
                    GamePause = false;
                    soundtrack->play();

                    playerHealth = 5;
                    playerHealth2 = 5;
                    rechargeTime = 0;
                    rechargeTime2 = 0;

                    shootRay.active = false;
                    shootRay2.active = false;
                }

                for (HWND button : GameButtons) ShowWindow(button, SW_SHOW);
                for (HWND button : MenuButtons) ShowWindow(button, SW_HIDE);
                DrawGame(hdc);
            }
            else
            {
                if (hBoardBrush != NULL)
                {
                    hBoardBrush = NULL;
                    playerPosition = { PlaneStartX + 100, (PlaneEndY - PlaneStartY) / 2 + PlaneStartY };
                    playerPosition2 = { PlaneEndX - 100, (PlaneEndY - PlaneStartY) / 2 + PlaneStartY };
                    soundtrack->stop();
                    soundtrack->reset();
                }

                for (HWND button : MenuButtons) ShowWindow(button, SW_SHOW);
                for (HWND button : GameButtons) ShowWindow(button, SW_HIDE);
                DrawMenu(hdc);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        GdiplusShutdown(gdiplusToken);
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
