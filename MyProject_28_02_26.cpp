#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

vector<int> values_count;
vector<char> values;

int GetIndex(char c)
{
    if (values.size() == 0) return -1;

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] == c) return i;
    }
    return -1;
}

int GetMaxValueIndex()
{
    int max = 0, index = 0;

    if (values_count.size() == 0) return -1;

    for (int i = 0; i < values_count.size(); i++)
    {
        if (values_count[i] > max) index = i;
    }
    return index;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);
    /*
    cout << "6 задание. Введи строку: ";
    string input, symbol, symbol2;
    int count = 0;
    getline(cin, input);
    cout << "Введи символ который нужно посчитать: ";
    cin >> symbol;
    
    for (int i = 0; i < input.length(); i++)
    {
        char a = tolower(input[i]);
        if (a == symbol[0]) count++;
    }

    cout << "В этой строке " << count << " символов! \n";

    // --------------

    cin.ignore();
    cout << "7 задание. Введи строку: ";
    getline(cin, input);
    cout << "Введи символ который нужно заменить: ";
    cin >> symbol;
    cout << "Введи, на который символ нужно заменить: ";
    cin >> symbol2;


    for (int i = 0; i < input.length(); i++)
    {
        char a = tolower(input[i]);
        if (a == symbol[0])
        {
            input[i] = symbol2[0];
        }
    }

    cout << input << endl;

    // --------------
    cin.ignore();
    cout << "8 задание. Введи строку с номером телефона: ";
    getline(cin, input);

    for (char c : input) if (isdigit(c)) cout << c;
    */

    // --------------

    cout << "какое то задание. Введи строку: ";
    string inp;
    getline(cin, inp);
    cout << "Введи сдвиг: ";
    int step;
    cin >> step;

    for (int i = 0; i < inp.length(); i++)
    {
        if (isalpha(inp[i]))
        {
            char a = inp[i] + step;
            cout << a;
        }
        else cout << inp[i];
    }

    // --------------

    cin.ignore();
    cout << "\nкакое то задание 2. Введи строку: ";
    getline(cin, inp);

    for (char c : inp)
    {
        if ((isalpha(c) || isdigit(c)) == false) continue;

        //cout << c << endl;

        c = tolower(c);
        int index = GetIndex(c);
        //cout << index << endl;
        if (index == -1)
        {
            values.push_back(c);
            values_count.push_back(1);
        }

        else values_count[index] += 1;
    }
    int length = values.size();
    for (int i = 0; i < length; i++)
    {
        int max_index = GetMaxValueIndex();

        if (max_index == -1) exit(0);

        cout << "'" << values[max_index] << "' : " << values_count[max_index] << endl;

        values.erase(values.begin() + max_index);
        values_count.erase(values_count.begin() + max_index);
    }

}

