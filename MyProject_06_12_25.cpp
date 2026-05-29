#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    /*
    cout << "6ое задание. Введите число К для получения факториала этого числа: ";
    int number;
    long long factorial = 1;
    cin >> number;

    if (number < 0) // проверка на положительное число
    {
        cout << "\nВводите положительное число! \n";
        exit(0);
    }

    //for (int k = 1; k <= number; k++) factorial *= k;
    int k = 1;
    while (k <= number)
    {
        factorial *= k;
        k++;
    }

    if (number == 0) cout << "\nФакториал числа " << number << " равен: 0" << endl;
    else cout << "\nФакториал числа " << number << " равен: " << factorial << endl;
    
    //---------------------------------

    cout << "\n8ое задание. Введите число К, чтобы узнать простое ли это число: ";
    int number2, k2, prostoi = 0;
    cin >> number2;

    if (number2 < 0) number2 * -1;  //модуль числа

    k2 = number2;

    while (0 < k2)
    {
        if (number % k2 == 0) prostoi++;
        k2--;
    }

    if (prostoi <= 2) cout << "\nЧисло " << number2 << " является простым" << endl;

    else cout << "\nЧисло " << number2 << " не является простым" << endl; 
    */
    //---------------------------------
    while (true) {
        cout << "\n7ое задание. Введите число К, чтобы узнать значение Фибоначи: ";
        int input, number3 = 0;
        cin >> input;

        if (input < 0) input * -1;  //модуль числа

        if (input > 99)
        {
            cout << "\nМаксимальное число 99!";
            exit(0);
        }
        if (input == 0)
        {
            cout << "\nНулевого числа не существует!";
            exit(0);
        }

        long long fibonachi[100];

        fibonachi[0] = 0;
        fibonachi[1] = 1;

        int k = 2;
        while (k <= input)
        {
            fibonachi[k] = fibonachi[k - 1] + fibonachi[k - 2];
            k++;
        }

        cout << input << "-ое число Фибоначи равно: " << fibonachi[input - 1];
    }
}