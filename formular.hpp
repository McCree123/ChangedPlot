#ifndef FORMULAR_HPP
#define FORMULAR_HPP

#include <QGraphicsItem>
#include <QPainter>

class Formular : public QGraphicsItem
{
private:
    // Что-то возвращаем в m_boundingRect по умолчанию
    QRectF m_boundingRect{0, 0, 40, 40};

    int valueX{0};
    int valueY{0};

    QString legendX{"X:"};
    QString legendY{"Y:"};

    QString stringMeasurementOnX{""};
    QString stringMeasurementOnY{""};

    // В пикселях
    int sizeFont{10};

    QColor colorBackground{0, 30, 40};
    QColor colorFont{217, 188, 12};

    void draw_formular(QPainter* painter, int text_point_x, int text_point_y,
                       int width_window, int height_window);

public:
    Formular();

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
                       QWidget*);

    // ----------- События для мыши ------------
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pe);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pe);

    // Приходит только когда перемещается QGraphicsItem с флагом Movable
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    int getValueX() const;
    void setValueX(int value);
    int getValueY() const;
    void setValueY(int value);

    // Устанавливаем размер формуляра в пикселях
    void setWidth(int value);
    void setHeight(int value);

    // Получаем размер формуляра в пикселях
    int getWidth();
    int getHeight();

    // Получаем размер шрифта в пикселях
    int getSizeFont() const;

    // Устанавливаем размер шрифта в пикселях
    void setSizeFont(int value);

    // Получить подпись к координате X
    QString getLegendX() const;

    // Установить подпись к координате X
    void setLegendX(const QString& value);

    // Получить подпись к координате Y
    QString getLegendY() const;

    // Установить подпись к координате Y
    void setLegendY(const QString& value);

    // Получить строчку, расшифровывающую единицу измерения по X
    QString getStringMeasurementOnX() const;

    // Установить строчку, расшифровывающую единицу измерения по X
    void setStringMeasurementOnX(const QString& value);

    // Получить строчку, расшифровывающую единицу измерения по Y
    QString getStringMeasurementOnY() const;

    // Установить строчку, расшифровывающую единицу измерения по Y
    void setStringMeasurementOnY(const QString& value);

    // Получить цвет фона
    QColor getColorBackground() const;

    // Установить цвет фона
    void setColorBackground(const QColor& value);

    // Получить цвет шрифта
    QColor getColorFont() const;

    // Установить цвет шрифта
    void setColorFont(const QColor& value);
};

#endif // FORMULAR_HPP
