#include <stdio.h>
#include <io.h>

struct TZap { // описываем структуру записи
    char FIO[30];
    double s_b;
} Zap;

int size = sizeof(TZap); // структура тоже поток по этому читаем ее и получаем размер

FILE *Fz, *Ft;

char File_Zap[] = "zapisi.dat";
char File_Rez[] = "result.txt";

void outputInfo(TZap); // декларация функции, которая описана в самом конце (просто выводим данные в файл и консоль)1

int main(int argc, char *argv[])
{
    // определение переменных для сортировки
    int kod, disc_file, i = 0, j, kol;
    long length;
    TZap st, *mas_Z;

    Ft = fopen(File_Rez, "w"); // открыли файл для записи (файл result.txt)

    while(true) { // бесконечный цикл для постоянной работы
        puts("\n 1 - Create \n 2 - Add \n 3 - View \n 4 - Sort \n 0 - EXIT \n");
        scanf("%d", &kod); // ожидаем ввод числа в командную строку и передаем значение в переменную kod

        switch(kod) {  // в switch кидаем переменную kod - это то число которое мы вводим, дальше значение переменной kod проверяется в case на соответствующие действия
            case 1: // 1 - Create
                if ( (Fz = fopen(File_Zap, "wb")) == NULL) { // открываем файл для записи и там же его создаем, позже проверяем значение Fz на null, чтобы избежать ошибку
                    puts("\n Create ERROR!"); // выводим ошибку

                    return 0; // завершаем работу программы
                }

                fclose(Fz); // закрываем файл
                printf("\n Create New File %s !\n", File_Zap); // сообщение в консоль о том, что создан новый файл

                break; // завершаем действие

            case 2: // 2 - Add
                Fz = fopen(File_Zap, "ab"); // открываем файл для добавления элементом в конец в двоичном формате

                printf("\n F.I.O. - "); // печатаем в консоль про указание фио
                fflush(stdin); // очистка потока
                gets(Zap.FIO); // считываем в1вод из консоли и записываем в Zap.FIO


                printf(" Ball - "); // печатаем в консоль про указание балла
                scanf("%lf", &Zap.s_b); // ожидаем ввод числа с плавающей запятой(%f) пользователя и передаем то, что он написал в Zap.s_b

                fwrite(&Zap, size, 1, Fz); // после того как мы заполнили Zap, мы берем данные из Zap и записываем в поток Fz(файл zapisi.dat), с длинной size и кол-во объектов - 1
                fclose(Fz); // закрываем файл

                break; // завершаем действие

            case 3: //  3 - View
                if ( (Fz = fopen(File_Zap, "rb")) == NULL) {
                    puts("\n Open ERROR");

                    return 0;
                }

                printf("\n\t ------Information------"); // выводим в консоль
                fprintf(Ft, "\n\t ------Information------"); // записываем в поток Fz (файл result.txt)

                while(true) {
                    // бесконечный цикл
                    // читаем поток Fz (файл zapisi.dat)
                    // выводим все записи, которые хранит этот поток
                    if (!fread(&Zap, size, 1, Fz)) break; // если поток для чтения закончен, то завершаем действие

                    outputInfo(Zap); // выводим данные
                }

                fclose(Fz); // закрываем файл

                break; // завершаем действие

            case 4: // 4 - Sort
                Fz = fopen(File_Zap, "rb"); // открываем файл для чтения в 2ом формате
                disc_file = fileno(Fz); // получаем дискриптор файла ну типо его название

                length = filelength(disc_file); // получаем длину файла
                kol = length / size; // в результате - это кол-во объектов для сортировки

                mas_Z = new TZap[kol]; // динамический массив для реализации сортировки с размером kol и типом Tzap (будет содрежать те самые объекты)


                for (i = 0; i < kol; i++) {
                    // пробегаем по всем элементам (ну у нас их количество, это переменная kol)
                    // читаем поток Fz (файл zapisi.dat)
                    // и записываем объекты в массив mas_Z перемещаясь на шаг вперед (+ i)
                    fread( (mas_Z + i), size, 1, Fz);
                }

                fclose(Fz); // закрываем файл аосле его прочтения

                printf("\n\t ------Sort------"); // выводим в консоль
                fprintf(Ft, "\n\t ------Sort------"); // записываем в поток Fz (файл result.txt)

                for (i = 0; i < kol; i++) {
                    for (j = i + 1; j < kol; j++) {
                        // если рейтинг текущего объекта больше, то делаем:
                        if (mas_Z[i].s_b > mas_Z[j].s_b) {
                            st = mas_Z[i]; // передаем в st объект с большим рейтингом
                            mas_Z[i] = mas_Z[j]; // переопределяем значение mas_Z[i] и теперь там объект с меньшим рейтингом
                            mas_Z[j] = st; // st содержит ссылку на объект с большим рейтингом, тем самым mas_Z[j] его получает

                            // в результате мы поменяли местами два объекта и элемент с меньшим рейтингом будет находиться в начале, элемент с большим в конце
                        }
                    }
                }

                for (i = 0; i < kol; i++) {
                    outputInfo(mas_Z[i]); // выводим в консоль все объекты из динамического массива в отсортированном виде
                }

                delete []mas_Z; // Уничтожаем динамический массив т.к. он больше не нужен

                break; // завершаем действие

            case 0: // 0 - EXIT
                fclose(Ft); // завершаем работу файла

                return 0; // завершаем цикл while и работу программы
        }
    }
}

// функция для вывода переданного объекта в консоль, так же он записывает в файл (fprintf)
void outputInfo(TZap z) {
    printf("\n%20s, %6.3lf .", z.FIO, z.s_b);
    fprintf(Ft, "\n%20s, %6.3lf .", z.FIO, z.s_b);
}