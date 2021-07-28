#ifndef MYMATH_H
#define MYMATH_H

#include <QVector>
#include <math.h>

class MyMath
{
public:
    MyMath();

    // Решим систему вида
    // a11*x + a12*y + a13*z = b1,
    // a21*x + a22*y + a23*z = b2,
    // a31*x + a32*y + a33*z = b3.
    // В результате возвращаем массив {x,y,z}
    static double* solveSystem(double a11, double a12, double a13, double b1,
                               double a21, double a22, double a23, double b2,
                               double a31, double a32, double a33, double b3);

    // Сюда добавляем точки (x,y), на выходе получаем массив коэффициентов
    // {a,b,c}. Сам полином будет вида a*x^3 + b*x^2 + c*x - 1
    static double* calcPolynomCoeff(double x1, double y1, double x2, double y2,
                                    double x3, double y3);

    // Сюда добавляем точки (x,y), на выходе получаем массив коэффициентов
    // {d,c,b,a}. Сам полином будет вида a*x^3 + b*x^2 + c*x - d
    static double* calcCoeffOnMnk(double x1, double y1, double x2, double y2,
                                  double x3, double y3);

    // Сюда добавляем вектора целочисленных значений. Они будут
    // преобразовываться в массивы типа double и обрабатываться функцией,
    // предназначенной для вычисления полинома по МНК
    // Возвращаться будет массив коэффициентов, самый последний для самой
    // старшей степени и далее по нисходящей
    // Если дать ссылку на int переменную в s, то по ней запишется размер
    // полных исходных данных по x и y, для которых был расчет, он же будет
    // степенью полученного полинома
    static double* calcCoeffOnMnk(const QVector<int>& x, const QVector<int>& y,
                                  int* s = nullptr);

    // Вычисление полинома по МНК для массивов действительных чисел любой
    // длинны, указанной в m. Переменная m и будет степенью посчитанного
    // полинома
    static double* calcCoeffOnMnk(double* x, double* y, int m);
};

#endif // MYMATH_H
