#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWidget>

#include "formular.hpp"
#include "graphic.hpp"
#include "label.hpp"
#include "shkala.hpp"

#define MODE_ADD_POINTS 0
#define MODE_DELETE_POINTS 1
#define MODE_CHANGE_POSITION_POINTS 2

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

    // То, ради чего все затевалось - пользователь построил график по базовым
    // точкам, а потом получил обратно 300 значений от 0 до 2000
    QVector<int> getValuesY();

    int getMaxYValue() const;

    // Полностью уберет график, и предыдущие значения,
    // значения на шкале обновятся на экране
    void setMaxYValue(int value);

    int getMaxXValue() const;

    // Полностью уберет график, и предыдущие значения,
    // значения на шкале обновятся на экране
    void setMaxXValue(int value);

protected:
    void paintEvent(QPaintEvent* event) override;
    // Переместили мышь
    void mouseMoveEvent(QMouseEvent*) override;
    // Пользователь нажал кнопку
    void mousePressEvent(QMouseEvent*) override;
    // Пользователь отпустил кнопку
    // void mouseReleaseEvent(QMouseEvent*) override;

private slots:
    void on_btnAddPoints_clicked();

    void on_btnDeletePoints_clicked();

    void on_btnChangePosPoints_clicked();

    void onClickLabel(Label* label);
    void onEndedMoveLabel(Label* label);
    void onMovedLabel(Label* label);

private:
    Ui::Widget* ui;

    int maxYValue{2000};
    int maxXValue{300};

    QGraphicsScene scene;
    Shkala* sh{nullptr};
    Shkala* sh1{nullptr};
    Graphic* graphic{nullptr};
    QVector<Label*> labels;
    Formular* formular;
    bool bRedrawScales{true};

    // Режим работы виджета
    // Или MODE_ADD_POINTS, или MODE_DELETE_POINTS, или
    // MODE_CHANGE_POSITION_POINTS
    int modeWork{-1};

    // Здесь вычисляются координаты на QGraphicsView
    // Возвращаем QPointF(-1,-1) если они не над QGraphicsView
    // Возвращаются координаты в пикселях
    QPointF calcXYOnGraphicsView(int x, int y);

    // Сюда передаются координаты от (0,0) на QGraphicsView
    // Возвращается QPointF(horizontalBias, verticalBias)
    // в дроби, характеризующей относительное расположение координат
    // (от 0 до 1 соответственно) по осям графика
    QPointF calcBiasOnGraphic(int x, int y);

    // Возвращается QPoint(x, y) - посчитанные координаты на QGraphicView внутри
    // графика по переданному bias
    QPoint calcCoordOnBias(QPointF bias);

    // По измененному label пересчитаем baseValuesX и baseValuesY в graphic
    void recalcBaseValueOnGraphic(Label* label);

    // Добавим значение в baseValueX и baseValueY по bias какого то из labels на
    // графике Если указан индекс i, то добавляем на конкретное место в уже
    // существующем baseValueX и baseValueY, если не указан то добавляем новое
    // значение в конец
    void addedBaseValueOnBias(const QPointF& bias, int i = -1);

    // Удалим значение из базовых точек в graphic на scene по индексу i
    void deleteBaseValueOnIndex(int i);

    // Проинициализировать формуляр точки значениями, на которых она находится
    // по графику
    // Соответственно на вход подается точка в виде объекта Label
    void initializeFormularPoint(Label* label);
};

#endif // WIDGET_H
