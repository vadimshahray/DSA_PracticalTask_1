#include <conio.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>

/// <summary>
/// Структура представляет собой модель одночлена.
/// </summary>
struct term {
   term(UCHAR _n = 0, float _a = 0, term *_next = NULL) : n(_n), a(_a), next(_next) { }

   /// <summary>
   /// Степень.
   /// </summary>
   UCHAR n;
   /// <summary>
   /// Коэффициент.
   /// </summary>
   float a;
   /// <summary>
   /// Указатель на следующий одночлен полинома.
   /// </summary>
   term *next;
};


/// <summary>
/// Функция возвращает знак числа.
/// </summary>
/// <param name="n">Число, у которого нужно определить знак.</param>
/// <returns>'+' или '-'.</returns>
char sign(float n);

/// <summary>
/// Функция возвращает модуль числа.
/// </summary>
/// <param name="n">Число.</param>
/// <returns>Модуль числа.</returns>
float abs(float n);

/// <summary>
/// Выводит полином на консоль.
/// </summary>
/// <param name="p">Указатель на первый одночлен полинома.</param>
/// <param name="c">Имя полинома.</param>
void printPolynomial(term *p, char c);

/// <summary>
/// Функция складывает 2 полинома друг с другом. 
/// </summary>
/// <param name="a">Указатель на первый полином.</param>
/// <param name="b">Указатель на второй полином.</param>
/// <returns>Указатель на полином, который будет результатом сложения первого и второго полиномов.</returns>
term * addPolynomials(term *a, term *b);

/// <summary>
/// Функция считывает из определенного файла 2 полинома.
/// </summary>
/// <param name="path">Путь к файлу.</param>
/// <param name="p">Указатель на указатель, в который нужно записать первый одночлен одного полинома.</param>
/// <param name="q">Указатель на указатель, в который нужно записать первый одночлен другого полинома.</param>
void scanPolynomials(const char *path, term **p, term **q);


int main()
{
   setlocale(0, "");
   
   term *P = new term(), *Q = new term(), *R;
   
   scanPolynomials("D:/polynomials.txt", &P, &Q);
   R = addPolynomials(P, Q);

   printPolynomial(P, 'P');
   printPolynomial(Q, 'Q');
   printPolynomial(R, 'R');

   return 0 * _getch();
}

void scanPolynomials(const char *path, term **p, term **q)
{
   FILE *stream;
   fopen_s(&stream, path, "r");

   if (stream)
   {
      // sign нужен для определения знака коэффициента, end - "флаг", обозначающий, что полином прочитан в файле до конца, если end = '\n'.
      char sign = '+', end = ' ';
      
      float a = 0;          // Коэффициент только что прочитанного одночлена.
      UCHAR N = 0, n = 0;   // N - наибольшая степень одночлена любого полинома, n - степень только что прочитанного одночлена.

      bool p_end = false;   // Означает, что полином p построен.
      // r - указатель, с помощью которого осуществляется доступ к каждому одночлену полинома p или q. Читаем каждый одночлен полиномов отдельно, пока не дойдем до конца файла (EOF). В конце каждой итерации проверяем, заполнен ли первый полином до конца. Если заполнен, то начинаем заполнять второй полином, иначе - переходим к следующему одночлену полинома, который заполняем сейчас.
      for (term *r = new term(); fscanf_s(stream, " %c %fx^%hhi%c", &sign, 1, &a, &n, &end, 1) != EOF; )
      {
         // Если степень у прочитанного одночлена больше ранее зафиксированной, то зафиксировать эту степень, как новый максимум.
         if (N < n) N = n;
         // Если перед одночленом стоит знак '-', отрицаем коэффициент.
         if (sign == '-') a *= -1;
         
         // Если первый одночлен полинома меньше n, то в полином нужно добавить этот одночлен с большей степенью в самое начало полинома.
         if (n > r->n)
         {
            // Добавляем новые одночлены в полином на первое место, пока не добавим тот, чья степень стала максимальной для этого полинома.
            for ( ; r->n != n; )
               r = new term(r->n + 1, 0, r);
            // Совершен переход к нужному одночлену, прибавляем его коэффициент. *Такой способ задания коэффициента (+=) для одночлена позволяет складывать подобные одночлены.
            r->a += a;
         }
         else // Иначе, значит, что прочитанный одночлен имеет равную или меньшую степень относительно первого одночлена полинома.
         {
            term *t = r; // Указатель нужен, чтобы не сбить ссылку на первый одноодночлен у r.
            // Перемещаемся по полиному r от начала до одночлена со степенью равной n.
            for ( ; t->n != n; t = t->next)
               if (!t->next) // Если ранее не был создан одночлен (допустим, читались одночлены, у которых степень была выше n), то создаем его.
                  t->next = new term(t->n - 1);
            t->a += a; // Совершен переход к нужному одночлену, прибавляем его коэффициент.
         }
         // Если первый полином еще не был прочитан, но уже дочитали до конца первого полинома, говрим, что первый полином построен, и переназначм r, чтобы заполнять второй полином.
         if (end == '\n' && !p_end)
         {
            p_end = true;
            r = new term();
         }
         p_end ? *q = r : *p = r; // Обновляем полином p или q, в зависимости от того, заполнен ли уже полином p.
      }

      // Если степень одного из полиномов (p1) больше, чем степень другого (p2), то добавляем одночлены в начало полинома p2, пока степень p2 < p1.
      for (term **z = (*p)->n < N ? p : (*q)->n < N ? q : NULL; z && (*z)->n < N; )
         *z = new term((*z)->n + 1, 0, *z);

      fclose(stream);
   }
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
   bool f = false; // Индикатор "есть одночлен перед текущим".
   printf_s("%cn(x) = ", c);
   for ( ; p; p = p->next)
   {
      if (p->n && p->a) // Если степень != 0 и коэф != 0, то
      {
         if (abs(p->a) != 1) // если |коэф| != 1, то
            // если есть предодночлен, пишем " %с |%а|", иначе пишем "%а".
            f ? printf_s(" %c %g", sign(p->a), abs(p->a)) : printf_s("%g", p->a);
         // иначе, если есть предодночлен пишем " %с ", иначе, "%с".
         else printf_s(f ? " %c " : "%c", sign(p->a));

         // если степень больше 1, то пишем "x^%n", иначе - "х".
         p->n > 1 ? printf_s("x^%d", p->n) : printf_s("x");
         f = true;
      }
      else if (!p->n) // иначе, если степень == 0, то
      {
         if (p->a && f) // если коэф != 0 и есть предодночлен, то
            printf_s(" %c %g", sign(p->a), abs(p->a));
         else if(!f) printf_s("%g", p->a); // иначе, если нет предодночлена, "%a".
         printf_s(".\n");
      }
   }
}
char sign(float n)
{
   return n > 0 ? '+' : '-';
}
float abs(float n)
{
   return sign(n) == '-' ? -1 * n : n;
}