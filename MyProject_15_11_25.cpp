#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    /*
    cout << "2ое задание. \nВведи число(1 - чётное, 2 - нечёт): ";
    int a;
    cin >> a;
    cout << (a % 2 == 0 ? "1" : "2");

    //-------------------------
    
    cout << "\n\n4ое задание. \nВведи 3 числa: ";
    int b, b1, b2;
    cin >> b;
    cin >> b1;
    cin >> b2;

    int b_count = 0;

    if (b == b1) {b_count = b_count + 1;}
    if (b1 == b2) {b_count = b_count + 1;}
    if (b == b2) { b_count = b_count + 1;}
    cout << ": " << b_count;
    
    //-------------------------
    
    cout << "\n\n6ое задание. \nВведи число: ";
    int c, c_num;
    cin >> c;
    
    c_num = c % 10;
    cout << c << (c == 1 || c_num ==1 ? " бочка" : (1 < c_num && c_num < 5 ? " бочки" : " бочек"));
    
    //-------------------------

    cout << "\n\nкакое то задание. \nВведи 3 числа(кол-во котлет на сковородку, время обжаривания 1ой строны, кол-во котлет): ";
    int k, m, n;
    cin >> k >> m >> n;

    int skovorodok = ceil(n / k);
    cout << "Вы приготовили котлеты за " << skovorodok * m * 2 << " минут. (они сгорели)";
    */
    //-------------------------

    cout << "\nкакое то задание. \nВведи число: "; // 1п - 15р, 10п - 125р, 60п - 440р
    int n;
    cin >> n;

    int bilet_1, bilet_2, bilet_3;
    bilet_3 = n / 60;
    bilet_2 = n % 60 / 10;
    bilet_1 = n % 60 % 10;

    cout << "билеты: " << bilet_3 << " - на 60 поездок, " << bilet_2 << " - на 10 поездок, " << bilet_1 << " - на 1 поездку";
}

