#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

/// <summary>
/// Структура представляет собой модель члена полинома.
/// </summary>
struct term {
   term(char _n = 0, short _a = 0, term *_next = NULL) : n(_n), a(_a), next(_next) { }

   /// <summary>
   /// Степень.
   /// </summary>
   char n;
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
/// Функция генерирует случайное число в диапазоне [CHAR_MIN; CHAR_MAX], не равное 0.
/// </summary>
/// <returns>Случайное число.</returns>
char random();

/// <summary>
/// Функция складывает 2 полинома друг с другом. 
/// </summary>
/// <param name="a">Указатель на первый полином, с которым будет складываться второй полином.</param>
/// <param name="b">Указатель на второй полином, с которым будет складываться первый полином.</param>
/// <returns>Указатель на полином, который будет результатом сложения первого и второго полиномов.</returns>
term * addPolynomials(term *a, term *b);

/// <summary>
/// Функция создает полином степени N. Принцип действия функции: рекуррентное создание членов полинома до N-ого члена, начиная с конца (с 0-го члена).
/// </summary>
/// <param name="n">Количество членов, которые еще нужно создать.</param>
/// <param name="next">Член полинома, который должен быть следующим по отношению к члену, создаваемому этой функцией.</param>
/// <returns>Указатель на первый член получившегося полинома.</returns>
term * createPolynomial(char n, term *next = NULL);

/// <summary>
/// Количество членов в полиноме.
/// </summary>
const char N = 6;

int main()
{
   // Настройка генерации случайных чисел.
   time_t t;
   srand((UINT) time(&t));

   term *P = createPolynomial(N), *Q = createPolynomial(N), *R = addPolynomials(P, Q);

   return 0 * _getch();
}

char random()
{
   // Генерируем случайное число и ограничиваем его диапазоном [CHAR_MIN; CHAR_MAX].
   char r = rand() % (CHAR_MAX + 1 - CHAR_MIN) + CHAR_MIN;
   return r == 0 ? random() : r;
};

term * createPolynomial(char n, term *next)
{
   // Если количество членов полинома, которые нужно создать, меньше либо равно 0, то мы возвращаем указатель на первый член с высшей степенью, иначе, снова вызываем эту функцию, передав уменьшенное на 1 количество членов, которые нужно создать, а также новый созданный член, у которого следующим после него членом является *next. Так, заполнение полинома происходит с конца и связи членом полинома можно представить таким образом: n0 <- n1 <- ... <- nN, где n0 <- n1 обозначает, что член n1 имеет ссылку на n0.
   return n > 0 ? createPolynomial(n - 1, new term(N - n, random(), next)) : next;
}
term * addPolynomials(term *a, term *b)
{
   term *r = new term(); // Указатель на первый элемент полинома суммы.
   for (term *p = r; a; p = p->next, a = a->next, b = b->next) // На каждой итерации переходит к следующему элементу каждого полинома, пока не дойдет до последнего, у которого указатель на следующий равен NULL.
   {
      p->n = a->n;
      p->a = a->a + b->a;
      p->next = a->next ? new term() : NULL;
   }

   return r;
}