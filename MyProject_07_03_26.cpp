#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <vector>
using namespace std;

// -------------| 1 задание |--------------

map<int, string> worlds_map = { {0, "ноль"}, {1, "один"}, {2, "два"}, {3, "три"}, {4, "четыре"}, {5, "пять"}, {6, "шесть"}, {7, "семь"}, {8, "восемь"}, {9, "девять"}, {10, "десять"},
        {11, "одинадцать"}, {12, "двенадцать"}, {13, "тринадцать"}, {14, "четырнадцать"}, {15, "пятнадцать"}, {16, "шестнадцать"}, {17, "семнадцать"}, {18, "восемнадцать"}, {19, "девятнадцать"}, {20, "двадцать"},
        {30, "тридцать"}, {40, "сорок"}, {50, "пятьдесят"}, {60, "шесдесят"}, {70, "семьдесят"}, {80, "восемьдесят"}, {90, "девяносто"}, {100, "сто"},
        {200, "двести"}, {300, "триста"}, {400, "четыреста"}, {500, "пятьсот"}, {600, "шестьсот"}, {700, "семьсот"}, {800, "восемьсот"}, {900, "девятсот"}, {1000, "тысяча"}, {1000000, "миллион"} };


string GetValueFromMap(int num)
{
    auto search = worlds_map.find(num);

    if (search != worlds_map.end()) return search->second;

    else return "";
}

int CalculateNumType(int i)
{
    if (i == 6 || i == 3) return 3;

    if (i == 5 || i == 2) return 2;

    if (i == 4 || i == 1) return 1;
}

// -------------| 2 задание |--------------

bool isPalindrome(const string& s, int start, int end)
{
    while (start < end)
    {
        if (s[start] != s[end]) return false;

        start++;
        end--;
    }
    return true;
}

// ==============| Основной метод |=============

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (true)
    {
        string number;
        vector<string> values;

        cout << "Введи число от 0 до 1 000 000: ";
        cin >> number;

        if (number == "a") break;

        if (stoi(number) < 0 || 1000000 < stoi(number)) exit(0);

        string tryingGet = GetValueFromMap(stoi(number));
        if (tryingGet != "")
        {
            cout << GetValueFromMap(stoi(number));
            continue;
        }

        for (int i = 0; i < number.length(); i++)
        {
            int val = number[i] - '0';
            string symbol(1, number[i]);
            values.push_back(symbol);
        }

        bool bigNum = values.size() > 3;
        for (int i = 0; i < values.size(); i++)
        {
            int system = CalculateNumType(values.size() - i);

            string symbols(1, values[i][0]);
            int num = stoi(symbols);

            if (symbols == "0")
            {
                if (bigNum && values.size() - i == 4) cout << "тысяч ";
                continue;
            }

            if (system == 3)
            {
                symbols += "00";
                cout << GetValueFromMap(stoi(symbols)) << " ";
            }
            else if (system == 2)
            {
                symbols += "0";
                cout << GetValueFromMap(stoi(symbols)) << " ";
            }
            else if (system == 1)
            {
                if (bigNum && values.size() - i == 4)
                {
                    if (num == 1) cout << "одна ";
                    if (num == 2) cout << "две ";
                    else if (num > 2) cout << GetValueFromMap(stoi(symbols)) << " ";
                }
                else cout << GetValueFromMap(stoi(symbols)) << " ";
            }

            if (bigNum && values.size() - i == 4)
            {
                if (num == 1) cout << "тысяча ";

                else if (1 < num && num < 5) cout << "тысячи ";

                else if (4 < num) cout << "тысяч ";
            }
        }

        cout << endl;
    }
    // -------------| 2 задание |--------------

    vector<string> palindromes;

    cin.ignore();
    cout << "Введи текст: ";
    string text;
    getline(cin, text);

    for (int i = 0; i < text.length(); i++)
    {
        for (int k = 0; k < text.length(); k++)
        {
            if (isPalindrome(text, i, k))
            {
                palindromes.push_back(text.substr(i, k - i + 1));
                cout << "-- \n";
            }
            else cout << "== \n";
        }
    }
    cout << "Палиндромы: " << endl;
    for (int i = 0; i < palindromes.size(); i++)
    {
        cout << "\"" << palindromes[i] << "\"" << endl;
    }
}