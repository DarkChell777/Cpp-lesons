#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <list>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    /*
    cout << "Введите кол-во: ";
    int iter;
    cin >> iter;

    cout << "Введите числа(напиши 0 для остановки): ";
    int a = 0, max = INT_MIN, min = INT_MAX;

    do {
        cin >> a;

        if (a > max) max = a;

        if (a < min) min = a;

        iter--;

    } while (iter >= 1);

    cout << "Максимальное число: " << max << "\nМинимальное число: " << min << endl;

    //-------------------------
    
    cout << "\nВведите число: ";
    string a2;
    int max2 = 0;
    cin >> a2;

    for (char symbol : a2)
    {
        int val = symbol - '0';

        if (val > max2)
        {
            max2 = val;
        }
    }

    cout << "Максимальная цифра из числа: " << max2 << endl;
    
    //-------------------------

    cout << "\nВведите число звёздочек(0-100): ";
    int stars, i = 0;
    char space = ' ';
    char star = '*';
    cin >> stars;

    if (stars < 0) stars *= -1;

    if (stars > 100) exit(0);

    cout << endl;

    do {
        cout << string(stars - i, space) << star << string((i + 1) * 2, space) << star << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        i++;
    } while (i < stars);

    i = stars;

    do {
        cout << string(stars - i, space) << star << string((i + 1) * 2, space) << star << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        i--;
    } while (i > 0);
    */
    //-------------------------

    cout << "\nВведите число(1-9): ";

    int stars2, i = 1, k2 = 0;
    char space2 = ' ';
    string line, reverse_line;
    list<string> lines;
    cin >> stars2;
    stars2++;

    if (stars2 < 0) stars2 *= -1;

    if (stars2 < 0 || stars2 > 10) exit(0);

    cout << endl;

    do {
        k2 = 2;
        while (k2 <= i)
        {
            line = line + to_string(k2) + ' ';
            k2++;
        }
        if (i == 1) line = string(2, space2);
        reverse_line = line;
        reverse(reverse_line.begin(), reverse_line.end());

        string out = string((stars2 - i) * 1.6, space2) + reverse_line + "1" + line + "\n";
        cout << out;
        
        this_thread::sleep_for(chrono::milliseconds(200));
        i++;

        if (i != stars2) lines.push_back(out);
        line = ' ';

    } while (i < stars2);

    i = 0;
    lines.reverse();

    auto symbol2 = lines.begin();
    do {
        cout << *symbol2;
        symbol2++;
        this_thread::sleep_for(chrono::milliseconds(200));


    } while (symbol2 != lines.end());
}
