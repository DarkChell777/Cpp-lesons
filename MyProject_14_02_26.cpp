#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
using namespace std;

int length = 10;
vector<int> libruaryBookId;
vector<string> libruaryBookName;
vector<string> libruaryBookContent;

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
        cout << "Такой ID уже существует!";
        return;
    }
    libruaryBookId.push_back(id);
    libruaryBookName.push_back(name);
    libruaryBookContent.push_back(content);
}

void AddingBook()
{
    int id;
    string name, content;
    char agree;

    cout << "Придумай ID книги(000-00000): ";
    cin >> id;

    if (id < 99 || 99999 < id)
    {
        cout << "Вы ввели неверный формат ID!";
        return;
    }
    if (FindBookIndex(id) != -1)
    {
        cout << "Такой ID уже существует!"; 
        return;
    }

    cin.ignore();

    cout << "Придумай название книги: ";
    //cin >> name;
    getline(cin, name);

    cout << "\nНапиши что-нибудь в книге: ";
    //cin >> content;
    getline(cin, content);

    cout << "\nВы уверены что хотите добавить книгу?[Y/N] ";
    cin >> agree;

    if (agree != 'Y')
    {
        cout << "Книга не добавлена! ";
        return;
    }

    AddBook(id, name, content);
    cout << "Успешно добавлена книга с ID: " << id;
}

void DeleteBook()
{
    int id, index;
    string name;
    cout << "Введи ID: ";
    cin >> id;

    index = FindBookIndex(id);
    if (index == -1)
    {
        cout << "Книга не найдена!";
        return;
    }
    name = libruaryBookName[index];

    libruaryBookId.erase(libruaryBookId.begin() + index);
    libruaryBookName.erase(libruaryBookName.begin() + index);
    libruaryBookContent.erase(libruaryBookContent.begin() + index);

    cout << "Книга \"" << name << "\" удалена!";
}

void ReplaceBook()
{
    int id, index, action;
    cout << "Введи ID книги: ";
    cin >> id;

    index = FindBookIndex(id);
    if (index == -1)
    {
        cout << "Книга не найдена!";
        return;
    }

    cout << "Что будешь менять[0 - ID][1 - название][2 - содержание][3 - выход]: ";
    cin >> action;

    if (action == 3) return;

    if (action == 0)
    {
        int temp_id;
        cout << "Придумай новое ID книги(000-00000): ";
        cin >> temp_id;

        if (temp_id < 99 || 99999 < temp_id)
        {
            cout << "Вы ввели неверный формат ID!";
            return;
        }
        if (FindBookIndex(temp_id) != -1)
        {
            cout << "Такой ID уже существует!";
            return;
        }

        libruaryBookId[index] = temp_id;
        cout << "ID изменён!";
    }
    cin.ignore();

    if (action == 1)
    {
        string name;
        cout << "Придумай новое название книги: ";
        //cin >> name;
        getline(cin, name);

        libruaryBookName[index] = name;
        cout << "Имя изменёно!";
    }
    else if (action == 2)
    {
        string content;
        cout << "Придумай новое содержание книги: ";
        //cin >> content;
        getline(cin, content);

        libruaryBookContent[index] = content;
        cout << "Содержание изменёно!";
    }
}

void PrintBookInfo(int i)
{
    cout << "|__Kнига №" << i + 1 << "." << endl;;
    cout << "|  |_____ID:" << libruaryBookId[i] << endl;
    cout << "|  |_____Name: \"" << libruaryBookName[i] << "\"" << endl;
    cout << "|        |____Content: " << libruaryBookContent[i] << endl;
    cout << "|" << endl;
}

void GetBooksList()
{
    for (int i = 0; i < libruaryBookId.size(); i++)
    {
        //if (libruaryBookId[i] != 0) 
            PrintBookInfo(i);
    }
}

void GetBook()
{
    int type;
    while (true)
    {
        cout << "\nВыбери вид поиска[0 - По ID][1 - По названию или содержанию][2 - выход]: ";
        cin >> type;

        if (type == 0)
        {
            int id, index;

            cout << "Введи ID: ";
            cin >> id;

            index = FindBookIndex(id);

            if (index == -1) cout << "Книга не найдена!";

            else PrintBookInfo(index);
        }

        else if (type == 1)
        {
            string input;
            cout << "Введи имя книги или кусок содержания: ";
            cin >> input;

            bool printed = false;
            for (int i = 0; i < libruaryBookId.size(); i++)
            {
                if (libruaryBookName[i].find(input) != string::npos || libruaryBookContent[i].find(input) != string::npos)
                {
                    PrintBookInfo(i);
                    printed = true;
                }
            }
            if (!printed) cout << "Книги по данной фразе не найдены!";
        }
        else if (type == 2) return;

        else
        {
            cout << "Неверная команда!";
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Добро пожаловать в библиотеку! Введите действие: \n\n0 - выход \n1 - список книг \n2 - получить книгу \n3 - добавить книгу \n4 - удалить книгу \n5 - заменить книгу \n";

    AddBook(3167, "Колобок", "Колобок не выжил, но был вкусным.");
    AddBook(94235, "Красная шапочка", "Девочка носила белое платье с красной шапкой.");
    AddBook(432, "Преступление и наказание", "Извлеки мораль из истории.");
    AddBook(317, "5 способов забить гвоздь", "Чтобы забить гвоздь - нужен молоток");
    AddBook(99813, "Фрукты Африки", "Их много... очень много. Больше чем вы думаете.");
    
    while (true)
    {
        int input;
        cout << "\n: ";
        cin >> input;

        switch (input)
        {
        case 0: 
            exit(0);
            break;

        case 1:
            GetBooksList();
            break;

        case 2:
            GetBook();
            break;

        case 3:
            AddingBook();
            break;

        case 4:
            DeleteBook();
            break;

        case 5:
            ReplaceBook();
            break;

        default:
            cout << "Такой команды не существует!";
            break;
        }
    }
}