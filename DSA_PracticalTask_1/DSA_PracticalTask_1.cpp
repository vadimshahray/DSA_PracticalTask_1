#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

/// <summary>
/// Структура представляет собой модель члена полинома.
/// </summary>
struct term {
   term(UCHAR _n = 0, short _a = 0, term *_next = NULL) : n(_n), a(_a), next(_next) { }

   /// <summary>
   /// Степень.
   /// </summary>
   UCHAR n;
   /// <summary>
   /// Коэффициент.
   /// </summary>
   short a;
   /// <summary>
   /// Указатель на следующий член полинома.
   /// </summary>
   term *next;
};

/// <summary>
/// Запрашивает ввод количества членов (n) полинома с консоли. При некорректном n, запрашивает повторный ввод.
/// </summary>
/// <returns>Количество членов полинома.</returns>
UCHAR scanN();

/// <summary>
/// Функция генерирует случайное число в диапазоне [CHAR_MIN; CHAR_MAX], не равное 0.
/// </summary>
/// <returns>Случайное число.</returns>
char random();

/// <summary>
/// Функция складывает 2 полинома друг с другом. 
/// </summary>
/// <param name="a">Указатель на первый полином.</param>
/// <param name="b">Указатель на второй полином.</param>
/// <returns>Указатель на полином, который будет результатом сложения первого и второго полиномов.</returns>
term * addPolynomials(term *a, term *b);

/// <summary>
/// Функция создает полином степени N. Принцип действия функции: рекуррентное создание членов полинома до N-ого члена, начиная с конца (с 0-го члена).
/// </summary>
/// <param name="n">Количество членов, которые еще нужно создать.</param>
/// <param name="next">Член полинома, который должен быть следующим по отношению к члену, создаваемому этой функцией.</param>
/// <returns>Указатель на первый член получившегося полинома.</returns>
term * createPolynomial(UCHAR n, term *next = NULL);

/// <summary>
/// Выводит полином на консоль.
/// </summary>
/// <param name="p">Указатель на первый член полинома.</param>
/// <param name="c">Имя полинома.</param>
void printPolynomial(term* p, char c);


int main()
{
   setlocale(0, "");
   
   // Настройка генерации случайных чисел.
   time_t t;
   srand((UINT) time(&t));

   UCHAR n = scanN();
   term *P = createPolynomial(n), *Q = createPolynomial(n), *R = addPolynomials(P, Q);

   printPolynomial(P, 'P');
   printPolynomial(Q, 'Q');
   printPolynomial(R, 'R');

   return 0 * _getch();
}

char random()
{
   // Генерируем случайное число и ограничиваем его диапазоном [CHAR_MIN; CHAR_MAX].
   char r = rand() % (CHAR_MAX + 1 - CHAR_MIN) + CHAR_MIN;
   return r == 0 ? random() : r;
};

term * createPolynomial(UCHAR n, term *next)
{
   // Если количество членов полинома, которые нужно создать, меньше либо равно 0, то мы возвращаем указатель на первый член с высшей степенью, иначе - снова вызываем эту функцию, передав уменьшенное на 1 количество членов, которые нужно создать, а также новый созданный член, у которого следующим после него членом является *next. Так, заполнение полинома происходит с конца и связи членом полинома можно представить таким образом: n0 <- n1 <- ... <- nN, где n0 <- n1 обозначает, что член n1 имеет ссылку на n0.
   return n > 0 ? createPolynomial(n - 1, new term(next ? next->n + 1 : 0, random(), next)) : next;
}
term * addPolynomials(term *a, term *b)
{
   // Указатель на первый элемент полинома суммы.
   term *r = new term();
   // На каждой итерации переходит к следующему элементу каждого полинома, пока не дойдет до последнего, у которого указатель на следующий равен NULL.
   for (term *p = r; a; p = p->next, a = a->next, b = b->next)
   {
      p->n = a->n;
      p->a = a->a + b->a;
      p->next = a->next ? new term() : NULL;
   }

   return r;
}
void printPolynomial(term *p, char c)
{
   // Вывод на консоль имени полинома и его первого члена. Первый член выводится вне цикла, чтобы упростить общую логику вывода отсальных членов полинома (знак '+' не должен выводится у первого члена, что возможно в логике вывода в цикле).
   printf_s("%cn(x) = %dx^%d", c, p->a, p->n);
   for (p = p->next; p; p = p->next)
   {
      // Вывод части члена полинома вида " + a" или " - а", в записимости от знака коэффициента.
      printf_s(" %c %d", p->a > 0 ? '+' : '-', p->a > 0 ? p->a : -1 * p->a);
      // Вывод оставшейся части члена полинома вида "x^n", или "x" (n = 1, показатель степени равный 1 опускается при записи), или "." (n = 0, можно опустить 'x').
      p->n > 1 ? printf_s("x^%d", p->n) : p->n == 1 ? printf_s("x") : printf_s(".\n");
   }
}
UCHAR scanN()
{
   UCHAR n = 0;
   do
   {
      printf_s("Введите N (1 < [N] < %d): ", UCHAR_MAX + 1);
      scanf_s("%hhi", &n);
   } while(n < 2); // Если введено 1 или 0, то запрашиваем повторный ввод.

   return n;
}