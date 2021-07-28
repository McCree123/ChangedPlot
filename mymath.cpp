#include "mymath.hpp"

#include <QVector>

MyMath::MyMath() {}

double*
MyMath::solveSystem(double a11, double a12, double a13, double b1, double a21,
                    double a22, double a23, double b2, double a31, double a32,
                    double a33, double b3)
{
    double det = a11 * a12 * a13 + a12 * a23 * a31 + a13 * a21 * a32
                 - a13 * a22 * a31 - a11 * a23 * a32 - a33 * a21 * a12;
    double detX = b1 * a22 * a33 + a12 * a23 * b3 + a13 * b2 * a32
                  - a13 * a22 * b3 - b1 * a23 * a32 - a12 * b2 * a33;
    double detY = a11 * b2 * a33 + b1 * a23 * a31 + a13 * a21 * b3
                  - a13 * b2 * a31 - a11 * a23 * b3 - b1 * a21 * a33;
    double detZ = a11 * a22 * b3 + a12 * b2 * a31 + b1 * a21 * a32
                  - b1 * a22 * a31 - a11 * b2 * a32 - a12 * a21 * b3;

    double* solution = new double[3];
    solution[0]      = detX / det;
    solution[1]      = detY / det;
    solution[2]      = detZ / det;

    return solution;
}

double*
MyMath::calcPolynomCoeff(double x1, double y1, double x2, double y2, double x3,
                         double y3)
{
    return solveSystem(x1 * x1 * x1,
                       x1 * x1,
                       x1,
                       y1,
                       x2 * x2 * x2,
                       x2 * x2,
                       x2,
                       y2,
                       x3 * x3 * x3,
                       x3 * x3,
                       x3,
                       y3);
}

double*
MyMath::calcCoeffOnMnk(double x1, double y1, double x2, double y2, double x3,
                       double y3)
{
    int m = 3; // Количество точек
    double *x, *y;
    x    = new double[m];
    x[0] = x1;
    x[1] = x2;
    x[2] = x3;
    y    = new double[m];
    y[0] = y1;
    y[1] = y2;
    y[2] = y3;

    double* a = calcCoeffOnMnk(x, y, m);

    delete[] x;
    delete[] y;

    return a;
}

double*
MyMath::calcCoeffOnMnk(const QVector<int>& x, const QVector<int>& y, int* s)
{
    int size = 0;

    double* xR;
    double* yR;

    if (x.size() < y.size())
    {
        size = x.size();
    }
    else
    {
        size = y.size();
    }
    xR = new double[size];
    yR = new double[size];

    for (int i = 0; i < size; i++)
    {
        xR[i] = x[i];
        yR[i] = y[i];
    }

    double* a = calcCoeffOnMnk(xR, yR, size);

    delete[] xR;
    delete[] yR;

    if (s)
    {
        *s = size;
    }
    return a;
}

double*
MyMath::calcCoeffOnMnk(double* x, double* y, int m)
{
    int i, j;
    double** c;

    int n;
    // cout << "Введите степень полинома:" << endl;
    // cin >> n;
    // n = n + 1;
    n = m + 1;
    c = new double*[n];
    for (i = 0; i < n; i++)
        c[i] = new double[n + 1];
    int k;
    double s;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            s = 0;
            for (k = 0; k < m; k++)
            {
                s = s + pow(x[k], (i + j));
            }
            c[i][j] = s;
        }

    for (i = 0; i < n; i++)
    {
        s = 0;
        for (k = 0; k < m; k++)
        {
            s = s + y[k] * pow(x[k], i);
        }
        c[i][n] = s;
    }

    for (k = 0; k < n; k++)
    {
        for (j = n; j >= k; j--)
            c[k][j] /= c[k][k];
        for (i = k + 1; i < n; i++)
            for (j = n; j >= k; j--)
                c[i][j] -= c[k][j] * c[i][k];
    }
    double* a;
    a = new double[n];
    // обратный ход
    for (i = 0; i < n; i++)
        a[i] = c[i][n];
    for (i = n - 2; i >= 0; i--)
        for (j = i + 1; j < n; j++)
            a[i] -= a[j] * (c[i][j]);
    // результат
    // cout << "Итоговые коэффициенты:\n";
    // for (j = 0; j < n; j++)
    //    cout << "a" << j << "=" << a[j] << endl;

    delete[] c;

    return a;
}
