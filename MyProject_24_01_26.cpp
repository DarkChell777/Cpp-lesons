#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    
    cout << "3яя задача. Сколько чисел ввести: \n";

    int N1, pos_count = 0, neg_count = 0, zero_count = 0;
    cin >> N1;
   
    for (int i = 0; i < N1; i++)
    {
        int num;
        cout << ": ";
        cin >> num;

        if (0 < num) pos_count++;

        else if (num < 0) neg_count++;

        else if (num == 0) zero_count++;
    }

    cout << "Количество: положительных чисел - " << pos_count << " отрицательных - " << neg_count << " нулей - " << zero_count << "\n\n";

    // --------------------

    cout << "4ая задача. Введи числа a и b, остаток С и делитель D: \n";
    int a, b, C, D;
    cin >> a >> b >> C >> D;

    if (a > b) 
    {
        int temp;
        temp = a;
        a = b;
        b = temp;
    }
    if (D == 0)
    {
        cout << "На ноль делить нельзя!";
        exit(0);
    }

    for (a; a <= b; a++)
    {
        if (a % D == C) cout << a << " / " << D << " = " << a / D << " Остаток: " << C << "                ----| " << a << " |----" << endl;
    }

    // --------------------

    cout << "\n6ая задача. Введи количество выводимых чисел: \n";
    int N2;
    cin >> N2;

    if (N2 < 0) N2 *= -1;

    for (int i = 0; true; i++)
    {
        if (N2 <= 0) break;

        for (int k = 0; k < i; k++)
        {
            cout << i << " ";
            N2--;

            if (N2 <= 0) break;
        }
        cout << endl;
    }
    
    // --------------------

    cout << "\n\nКакая то задача. Введи кол-во строк: ";
    int lines;
    cin >> lines;

    if (lines < 0) lines *= -1;

    for (int i = 1; i <= lines; i++)
    {
        for (int k = 1; k <= i; k++)
        {
            cout << k << " ";
        }

        cout << endl;
    }

    
    // --------------------

    cout << "\n\nОпять какая то задача. Введи кол-во строк: ";
    int lines2;
    cin >> lines2;

    if (lines2 < 0) lines2 *= -1;

    for (int i = lines2; true; i--)
    {
        if (i <= 0) break;

        for (int k = 1; k <= i; k++)
        {
            cout << k << " ";
        }

        cout << endl;
    }
}
