#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    /*
    cout << "1) Введите 4-хзначное число:";
    int num;
    cin >> num;

    cout << "Сумма чисел: ";
    cout << num / 1000 + num / 100 % 10 + num / 10 % 10 + num % 10; // 1234 -> 1 + 2 + 3 + 4

    // ----------------------------

    cout << "\n\n2) Введите число(0 - чёт, 1 - нечёт): ";
    int num2;
    cin >> num2;

    cout << num2 % 2;

    // ----------------------------
    
    cout << "\n\n3) Введите 1ое, 2ое, 3ее число: ";
    int num3, num3_1, num3_2;
    cin >> num3;
    cin >> num3_1;
    cin >> num3_2;

    int num_min, num_max;

    num_min = min(num3, num3_1);
    num_min = min(num_min, num3_2);

    num_max = max(num3, num3_1);
    num_max = max(num_max, num3_2);

    cout << "Наименьшее число: " << num_min << " Наибольшее число: " << num_max;
    
    // ----------------------------

    cout << "\n\n4) Введите 1ое, 2ое, 3ее число: ";
    int num4, num4_1, num4_2;
    cin >> num4 >> num4_1 >> num4_2;

    
    int temp = num4;
    num4 = num4_1;
    num4_1 = num4_2;
    num4_2 = temp;
    
    num4 = num4 + num4_1 + num4_2;
    num4_1 = num4 - (num4_1 + num4_2);
    num4_2 = num4 - (num4_1 + num4_2);
    num4 = num4 - (num4_1 + num4_2);

    cout << num4 << " " << num4_1 << " " << num4_2;

    // ----------------------------

    cout << "\n\n5) Введите 4-хзначное число(0 - делится, 1 - не делится): ";
    int num5;
    cin >> num5;

    cout << ": " << (num5 / 1000) / (num5 / 100 % 10) % 2;
    */

    cout << "1) Введите число(1 - больше нуля, 0 - меньше):";
    int num;
    cin >> num;

    cout << (num >= 0);

}
