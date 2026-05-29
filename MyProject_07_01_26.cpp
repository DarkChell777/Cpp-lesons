#include <iostream>
using namespace std;

const int length = 5;
int box[length][length];
int original_box[length][length];
int bombs_box[length][length];

int CheckCell(int y, int x)
{
    if (original_box[y][x] == 1) return 1;
    return 0;
}

int CalculateNearBombs(int y, int x)
{
    int sumBomb = 0;

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

void UnlockNullCells(int y, int x)
{
    bool up = y != 0;
    bool down = y != length - 1;
    bool right = x != length - 1;
    bool left = x != 0;

    if (up) box[y - 1][x] = -1; // up
    if (down) box[y + 1][x] = -1; // down
    if (right) box[y][x + 1] = -1; // right
    if (left) box[y][x - 1] = -1; // left
}

void PrintPlane(bool hide)
{
    cout << "\n\n";
    cout << "  | 0 1 2 3 4 |" << endl;
    cout << "_______________" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << i << " | ";
        for (int k = 0; k < length; k++)
        {
            int cell = box[i][k];

            if (hide)
            {
                if (cell == 0 || cell == 1) cout << "# ";

                else if (cell == -1)
                {
                    cout << bombs_box[i][k] << " ";

                    if (bombs_box[i][k] == 0) UnlockNullCells(i, k);
                }

                else if (cell == -2) cout << "$ ";

                else cout << "error";
            }

            else cout << original_box[i][k] << " ";
        }
        cout << endl;
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

int main()
{
    setlocale(LC_ALL, "Russian");

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

    cout << "Добро пожаловать в Сапёр! Найдите бомбы и выйграйте игру. \n# - неизвестное поле \n$ - флажок \n0 - выкопать клетку\n1 - поставить флаг \n2 - отмена действия \n3 - выход\n\n";

    while (true)
    {
        PrintPlane(false);
        PrintPlane(true);
        int x, y, action;
        cout << "\nВведи номер по X(0-4): ";
        cin >> x;
        cout << "Введи номер по Y(0-4): ";
        cin >> y;
        cout << "Введи действие(0,1,2): ";
        cin >> action;

        if (action == 3) exit(0);

        else if (action == 2) continue;

        else if (action == 1) box[y][x] = -2;

        else if (action == 0)
        {
            if (original_box[y][x] == 1)
            {
                cout << "\nИгра окончена! Ты проиграл!!!\n\n";
                PrintPlane(false);
                exit(0);
            }
            else if (original_box[y][x] == 0) box[y][x] = -1;
        }

        if (CheckWin())
        {
            cout << "Поздравляю! Ты выйграл!";

            exit(0);
        }
    }


}

/*
 cout << "Введи длину массива(1-100): ";
    int N, min = INT_MAX, max = INT_MIN, sum = 0;
    cin >> N;

    if (N < 1 || 100 < N) exit(0);

    int* array1 = new int[N];
    int* array2 = new int[N];
    int* array3 = new int[N];

    for (int i = 0; i < N; i++) array1[i] = rand() % 20 + 10;
    for (int i = 0; i < N; i++) array2[i] = rand() % 20 + 10;
    for (int i = 0; i < N; i++) array3[i] = array1[i] + array2[i];

    for (int i = 0; i < N; i++)
    {
        int a = array3[i];

        if (a < min) min = a;
        if (a > max) max = a;

        sum += a;
    }

    cout << "Максимальное число: " << max << " Минимальное число: " << min << " Среднее арефметическое: " << sum / N << "\n\n";
*/

