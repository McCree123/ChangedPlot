#ifndef LABEL_HPP
#define LABEL_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>

// class Label : public QGraphicsItem, public QObject //error
class Label : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:
    QPen pen{QColor(0xC0, 0xC0, 0xC0), 0.8, Qt::SolidLine};
    QBrush brush{QColor(0xC0, 0xB0, 0xA0), Qt::SolidPattern};

    int mode{-10}; // то значение по умолчанию, которого точно нет
    int size{0};

    bool bCustomSize{false}; // Если false, то используем стандартные размеры
                             // для указателей
    static constexpr const auto defaultSize1 = 20;
    static constexpr const auto defaultSize2 = 15;

    // Если false, то перо и кисть стандартные
    bool bCustomPen{false};
    bool bCustomBrush{false};

    // размеры внутри QGraphicsView по умолчанию
    QRectF m_boundingRect{QRectF(-50, -50, 100, 100)};

    // Положение Label относительно какого - то контейнера
    // от 0 до 1. Используется только пользователем данного класса
    double horizontalBias{0};
    double verticalBias{0};

    void drawArrow(QPainter* painter, int startPointX, int startPointY,
                   int width, int heightHead, int height);

    void drawFatArrow(QPainter* painter, int startPointX, int startPointY,
                      int width, int heightHead, int height,
                      bool inTheMiddle = false);

protected:
    virtual QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* /*widget*/);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pe);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pe);

    // Будет только с нажатой кнопкой мыши если выставили ItemIsMovable объекту,
    // образованному от QGraphicsItem
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* pe);

public:
    Label();

    void setBoundingRect(const QRectF& boundingRect);

    void setPen(QPen pen);

    void setBrush(QBrush brush);

    void setDefaultPen();

    void setDefaultBrush();

    /* setSize(int size) - задать размер в неких единицах */
    void setSize(int size);

    void setDefaultSize();

    /* setMode(int mode) - задать режим рисования
     * mode = -1 - забавная хрень
     * mode = 0 - кружочек
     * mode = 1 - стрелка
     * mode = 2 - домик
     * mode = 3 - ромбик */
    void setMode(int mode);

    // Сохранить относительные координаты в пропорции по горизонтали от 0 до 1
    void setHorizontalBias(double x);

    // Сохранить относительные координаты в пропорции по вертикали от 0 до 1
    void setVerticalBias(double y);

    // Получить относительные координаты в пропорции по горизонтали от 0 до 1
    double getHorizontalBias();

    // Получить относительные координаты в пропорции по вертикали от 0 до 1
    double getVerticalBias();

signals:
    void clickLabel(Label* label);
    void endedMoveLabel(Label* label);
    void movedLabel(Label* label);
};

#endif // LABEL_HPP
