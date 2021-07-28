#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include "mymath.hpp"
#include <QGraphicsItem>
#include <QPainter>

class Graphic : public QGraphicsItem
{
private:
    static constexpr const int CountPointsInGraphic = 300;

    // Цвета (0 - цвет фона, 1 = ColorYellow, и т.д.)
    QColor ColorTable[5] = {QColor(0, 0, 100), // цвет фона
                            QColor(255, 255, 0), // цвет линии графика
                            QColor(0, 255, 0),
                            QColor(255, 0, 0),
                            QColor(128, 128, 128)}; // цвет сетки

    // для QGraphicsScene и QGraphicsView
    QRectF m_boundingRect{
        QRectF(0, 0, 400, 400)}; // размеры внутри QGraphicsView по умолчанию

    // Через эти точки идет кривая
    QVector<int> valuesY;

    // По этим точкам кривая будет строиться
    QVector<int> baseValuesX;
    QVector<int> baseValuesY;

    // Максимальное значение координаты X
    int Xmax{300};

    // Максимальное значение координаты Y
    int Ymax{2000};

    // Шаги, через которые должна рисоваться сетка чтобы совпадать с рисками на
    // шкалах
    int Xstep{1};
    int Ystep{1};

    // Призвать великий инкрементный восьмисвязный алгоритм Брезенхема,
    // который побеждает ктулху и строит по двум точкам
    // линию в массивах valuesX и valuesY
    void brezenhemAlg(int x0, int y0, int x1, int y1);

    // Просто МНК, сколько базовых точек, такая и степень полинома
    void calcValuesYOnMnk();

    // Сортируем по baseValuesX все точки (baseValuesX[i], baseValuesY[i])
    // при добавлении новых базовых точек в график (по возрастанию)
    int* sortBaseValues();

protected:
    virtual QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* /*widget*/);

public:
    Graphic();
    void setBoundingRect(QRectF boundingRect);
    QVector<int> getBaseValuesX() const;
    // Добавление значений по X будет проходить с сортировкой,
    // что бы значения шли как на графике от большего к меньшему
    // Поэтому сначала надо добавлять значения базовых точек по Y,
    // потом по X, и тогда отсортируются точки по X вместе со значениями Y
    int* setBaseValuesX(const QVector<int>& value);
    void setBaseValuesXWithNoSort(const QVector<int>& value);
    QVector<int> getBaseValuesY() const;
    void setBaseValuesY(const QVector<int>& value);

    // Для рисования сетки
    void setScaleStepX(int step);
    void setScaleStepY(int step);

    QVector<int> getValuesY() const;

    // Получить Y по X
    int getValueY(int X);

    // Получить размер шкалы значений по X
    int getXmax() const;

    // Установить размер шкалы значений по X
    void setXmax(int value);

    // Получить размер шкалы значений по Y
    int getYmax() const;

    // Установить размер шкалы значений по Y
    void setYmax(int value);
};

#endif // GRAPHIC_HPP
