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
/// Функция складывает 2 полинома друг с другом. Результат сложения почленно записывается в полином, на который указывает r. 
/// </summary>
/// <param name="r">Указатель на полином, который будет результатом сложения первого и второго полиномов.</param>
/// <param name="a">Указатель на первый полином, с которым будет складываться второй полином.</param>
/// <param name="b">Указатель на второй полином, с которым будет складываться первый полином.</param>
void addPolynomials(term *r, term *a, term *b);

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
const char N = 3;

int main()
{
   // Настройка генерации случайных чисел.
   time_t t;
   srand((UINT) time(&t));

   term *P = createPolynomial(N), *Q = createPolynomial(N), *R = new term();
   addPolynomials(R, P, Q);

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
   // Если количество членов полинома, которые нужно создать, равно 0, то мы возвращаем указатель на первый член с большей степенью, иначе вызываем эту функцию, передав уменьшенное на 1 количество членов, которые нужно создать, а также новый созданный член, являющийся следующим для текущего, так как заполнение полинома происходит от конца (n0 <- n1 <- ... <- nN).
   return n == 0 ? next : createPolynomial(n - 1, new term(N - n, random(), next));
}
void addPolynomials(term *r, term *a, term *b)
{
   if (r != NULL) // Если мы еще не дошли до последнего n0 члена полиномов, 
   {
      // то создаем новый член и сразу же добавляем его в качеcтве члена полинома суммы a и b.
      *r = *new term(a->n, a->a + b->a, a->next == NULL ? NULL : new term());
      // Снова вызываем функцию, передав указатели на следующие элементы полиномов. Таким образом мы заполняем полином суммы от его первого члена к его последнему (nN -> n(N-1) -> ... -> n0).
      addPolynomials(r->next, a->next, b->next);
   }
}