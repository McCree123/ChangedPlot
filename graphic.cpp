#include "graphic.hpp"

Graphic::Graphic()
{
    // Создание массива в формате QVector, проинициализированного нулями
    for (int i = 0; i < CountPointsInGraphic; i++)
    {
        valuesY.push_back(0);
    }
}

QRectF
Graphic::boundingRect() const
{
    return m_boundingRect; // размеры QGraphicsItem
}

void
Graphic::setBoundingRect(QRectF boundingRect)
{
    m_boundingRect = boundingRect;
}

QVector<int>
Graphic::getBaseValuesX() const
{
    return baseValuesX;
}

int*
Graphic::sortBaseValues()
{
    // Сортировка массива baseValuesX методом Шелла (упорядочиваем по X)
    int size = baseValuesX.size();
    int step = size / 2;
    int tmp, i, j;
    int* ind = new int[size];

    for (i = 0; i < size; i++)
        ind[i] = i;

    while (step > 0)
    {
        for (i = 0; i < (size - step); i++)
        {
            j = i;
            while (j >= 0 && baseValuesX[ind[j]] > baseValuesX[ind[j + step]])
            {
                tmp           = ind[j];
                ind[j]        = ind[j + step];
                ind[j + step] = tmp;
                j--;
            }
        }
        step = step / 2;
    }

    QVector<int> baseX;
    QVector<int> baseY;

    for (i = 0; i < size; i++)
    {
        baseX.push_back(baseValuesX[ind[i]]);
        baseY.push_back(baseValuesY[ind[i]]);
    }

    baseValuesX.clear();
    baseValuesY.clear();

    for (i = 0; i < size; i++)
    {
        baseValuesX.push_back(baseX[i]);
        baseValuesY.push_back(baseY[i]);
    }

    return ind;
}

int*
Graphic::setBaseValuesX(const QVector<int>& value)
{
    baseValuesX = value;
    return sortBaseValues();
}

void
Graphic::setBaseValuesXWithNoSort(const QVector<int>& value)
{
    baseValuesX = value;
}

QVector<int>
Graphic::getBaseValuesY() const
{
    return baseValuesY;
}

void
Graphic::setBaseValuesY(const QVector<int>& value)
{
    baseValuesY = value;
}

void
Graphic::setScaleStepX(int step)
{
    Xstep = step;
}

void
Graphic::setScaleStepY(int step)
{
    Ystep = step;
}

void
Graphic::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget*)
{
    Q_UNUSED(option);

    // Шаги на графике по x и y для одной единицы координат
    // 40 - ширина поля шкалы с числами снизу
    // Возможно это поле, на которое нельзя залезать надо сюда передавать
    double kx = static_cast<double>(painter->window().width() - 40)
                / static_cast<double>(Xmax);
    double ky = static_cast<double>(painter->window().height() - 40)
                / static_cast<double>(Ymax);

    // Задаем область до конца окна QGraphicsView, залезая снизу под шкалу
    // шириной в 40 пикселей
    setBoundingRect(
        QRectF(0, 0, painter->window().width(), painter->window().height()));

    // Заполняем всю область фоном
    painter->fillRect(0,
                      0,
                      painter->window().width(),
                      painter->window().height(),
                      QBrush(ColorTable[0]));

    // Таким образом нижняя шкала никогда не оторвётся от заднего фона графика
    // при перерисовке всей сцены в изменённых размерах (например при растяжении
    // адаптивной формы)

    // Нарисуем сетку
    // Установка соответствующего пера
    painter->setPen(QPen(QBrush(ColorTable[4]), 1, Qt::DashLine));
    for (int Y = 0; Y < Ymax; Y += Ystep)
    {
        painter->drawLine(0,
                          static_cast<double>(painter->window().height() - 40)
                              - (static_cast<double>(Y) * ky),
                          painter->window().width(),
                          static_cast<double>(painter->window().height() - 40)
                              - (static_cast<double>(Y) * ky));
    }
    for (int X = 0; X < Xmax; X += Xstep)
    {
        painter->drawLine(static_cast<double>(X) * kx,
                          0,
                          static_cast<double>(X) * kx,
                          painter->window().height());
    }

    int size = 0;
    if (baseValuesX.size() == 0 || baseValuesX.size() == 1
        || baseValuesY.size() == 0 || baseValuesY.size() == 1)
        return;
    if (baseValuesX.size() == 2)
    {
        // Обнуляем старые данные перед перерасчётом
        for (int i = 0; i < CountPointsInGraphic; i++)
        {
            valuesY[i] = 0;
        }
        // Строим линию от baseValuesX[0] до baseValuesX[1]
        brezenhemAlg(
            baseValuesX[0], baseValuesY[0], baseValuesX[1], baseValuesY[1]);
    }
    else
    {
        // Обнуляем старые данные перед перерасчётом
        for (int i = 0; i < CountPointsInGraphic; i++)
        {
            valuesY[i] = 0;
        }
        // Чтобы кривая шла точно через точки, фильтр Калмана должен быть
        // отключен (в аргумент передать false)
        // Так кривая может стать с острыми углами
        // Можно включить фильтр Калмана, тогда получим некое подобие
        // моделирования сплайновой кривой

        calcValuesYOnMnk();
    }

    size = valuesY.size();

    // Установка пера в начальную точку и рисование линии
    double startX = 0;
    double startY = static_cast<double>(painter->window().height() - 40)
                    - static_cast<double>(valuesY[0]) * ky;
    double drawToX, drawToY;
    // Установка соответствующего цвета пера
    painter->setPen(QPen(QBrush(ColorTable[1]), 1));
    for (int X = 0; X < size; X++)
    {
        drawToX = static_cast<double>(X) * kx;
        drawToY = static_cast<double>(painter->window().height() - 40)
                  - (static_cast<double>(valuesY[X]) * ky);

        painter->drawLine(static_cast<int>(startX),
                          static_cast<int>(startY),
                          static_cast<int>(drawToX),
                          static_cast<int>(drawToY));
        startX = drawToX;
        startY = drawToY;
    }
}

QVector<int>
Graphic::getValuesY() const
{
    return valuesY;
}

int
Graphic::getValueY(int X)
{
    return valuesY[X];
}

int
Graphic::getXmax() const
{
    return Xmax;
}

void
Graphic::setXmax(int value)
{
    Xmax = value;
}

int
Graphic::getYmax() const
{
    return Ymax;
}

void
Graphic::setYmax(int value)
{
    Ymax = value;
}

void
Graphic::brezenhemAlg(int x0, int y0, int x1, int y1)
{
    // Инкрементный восьмисвязный алгоритм Брезенхема
    int xerr, yerr, d, dx, dy, x, y, i;
    int incX = 0, incY = 0;
    xerr = 0;
    yerr = 0;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dx > 0)
        incX = 1;
    if (dx == 0)
        incX = 0;
    if (dx < 0)
        incX = -1;

    if (dy > 0)
        incY = 1;
    if (dy == 0)
        incY = 0;
    if (dy < 0)
        incY = -1;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    if (dx > dy)
        d = dx;
    else
        d = dy;

    x = x0;
    y = y0;

    i = d;
    while (i)
    {
        xerr = xerr + dx;
        yerr = yerr + dy;

        if (xerr >= d)
        {
            xerr = xerr - d;
            x    = x + incX;
        }
        if (yerr >= d)
        {
            yerr = yerr - d;
            y    = y + incY;
        }

        if (x >= Xmax)
            break;

        valuesY[x] = y;
        i--;
    }
}

void
Graphic::calcValuesYOnMnk()
{
    int powPolynome = 0;
    double* koeff
        = MyMath::calcCoeffOnMnk(baseValuesX, baseValuesY, &powPolynome);

    switch (powPolynome)
    {
    case 0:
        return;
    case 1:
        for (int X = baseValuesX[0]; X < baseValuesX[baseValuesX.size() - 1];
             X++)
        {
            valuesY[X] = koeff[1] * static_cast<double>(X) + koeff[0];
        }
        break;
    default:
        double res;
        for (int X = baseValuesX[0]; X < baseValuesX[baseValuesX.size() - 1];
             X++)
        {
            res = koeff[1] * static_cast<double>(X) + koeff[0];
            for (int p = powPolynome; p > 1; --p)
            {
                res += koeff[p] * pow(static_cast<double>(X), p);
            }
            valuesY[X] = res;
        }
        break;
    }
}
