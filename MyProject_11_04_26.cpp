#include <iostream>
#include <stdio.h>
#include <time.h>

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    
    int my_number = rand() % 100, you_number = -1;

    printf("Задание 7. Угадай число от 0 до 100. ");
    while (my_number != you_number)
    {
        printf("Твое число: ");
        scanf_s("%d", &you_number);

        if (you_number < 0 || 100 < you_number)
        {
            printf("Ты вышел за пределы диапозона чисел! \n");
            continue;
        }
        if (my_number > you_number) printf("Моё число больше.\n");

        else if (my_number < you_number) printf("Моё число меньше.\n");
    }
    printf("Вы угадали! Моё число %d", my_number);
    
    //------------------

    printf("\n\nЗадание 8. Загадай число от 0 до 100. \n");
    
    int unknown_num = rand() % 100, min_num = 0, max_num = 100;
    char symbol;
    while (true)
    {
        printf("\nВы загадали число %d?(y/n)", unknown_num);
        scanf_s(" %c", &symbol, 1);

        if (symbol == 'y') break;
        else if (symbol != 'n') continue;

        printf("Ваше число больше %d?(y/n)", unknown_num);
        scanf_s(" %c", &symbol, 1);

        if (symbol == 'y') min_num = unknown_num + 1;
        
        if (symbol == 'n') max_num = unknown_num - 1;
        
        if (max_num - min_num < 0) break;

        unknown_num = (max_num - min_num) / 2 + min_num;
    }
    printf("Я угадал! Ваше число: %d", unknown_num);

}

