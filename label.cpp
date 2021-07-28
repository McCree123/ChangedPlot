#include "label.hpp"

#include <QGraphicsSceneMouseEvent>

Label::Label() {}

void
Label::drawArrow(QPainter* painter, int startPointX, int startPointY, int width,
                 int heightHead, int height)
{
    int halfWidth = width / 2;
    int x2        = startPointX - halfWidth;
    int x3        = startPointX + halfWidth;
    int y2        = startPointY - height + heightHead;
    int y3        = startPointY - height;

    if (heightHead != height)
    {
        painter->drawLine(startPointX, startPointY, startPointX, y2);
    }

    // Треугольник
    QPolygon poly;
    poly << QPoint(x2, y2);
    poly << QPoint(startPointX, y3);
    poly << QPoint(x3, y2);
    painter->drawPolygon(poly); //  рисуем контур фигуры
    QPainterPath path;
    path.addPolygon(poly);
    painter->fillPath(path, painter->brush()); //  заливаем эту фигуру
}

void
Label::drawFatArrow(QPainter* painter, int startPointX, int startPointY,
                    int width, int heightHead, int height, bool inTheMiddle)
{
    int startPY;
    if (inTheMiddle)
    {
        startPY = startPointY - heightHead - (height / 2)
                  + 2; // + 2 для центрирования
        //(толщина пера + погрешности деления)
    }
    else
    {
        startPY = startPointY;
    }
    int halfWidth = width / 2;

    int y2 = startPY + heightHead;
    int x2 = startPointX - halfWidth;
    int x3 = startPointX + halfWidth;

    QPolygon poly;
    poly << QPoint(x2, y2);
    poly << QPoint(startPointX, startPY);
    poly << QPoint(x3, y2);

    painter->drawPolygon(poly); //  рисуем контур фигуры

    QPainterPath path;
    path.addPolygon(poly);

    painter->fillPath(path, painter->brush()); //  заливаем эту фигуру

    int y3 = startPY + height;

    poly.remove(0, 3);

    poly << QPoint(x2, y2 + 1);
    poly << QPoint(x3, y2 + 1);
    poly << QPoint(x3, y3 + 1);
    poly << QPoint(x2, y3 + 1);

    /*
        // Так нарисует рыбку
        poly << QPoint(x2, y2);
        poly << QPoint(x3, y2);
        poly << QPoint(x2, y3);
        poly << QPoint(x3, y3);
    */

    painter->drawPolygon(poly); //  рисуем контур фигуры

    path.addPolygon(poly);
    painter->fillPath(path, painter->brush()); //  заливаем эту фигуру
}

QRectF
Label::boundingRect() const
{
    return m_boundingRect; // размеры QGraphicsItem
}

void
Label::setBoundingRect(const QRectF& boundingRect)
{
    m_boundingRect = boundingRect;
}

void
Label::setPen(QPen pen)
{
    bCustomPen = true;
    this->pen  = pen;
}

void
Label::setBrush(QBrush brush)
{
    bCustomBrush = true;
    this->brush  = brush;
}

void
Label::setDefaultPen()
{
    bCustomPen = false;
}

void
Label::setDefaultBrush()
{
    bCustomBrush = false;
}

void
Label::setSize(int size)
{
    bCustomSize = true; // меняем размер на свой, игнорируем те что по умолчанию
    this->size  = size;
}

void
Label::setDefaultSize()
{
    bCustomSize = false; // устанавливаем размеры по умолчанию
}

void
Label::setMode(int mode)
{
    this->mode = mode;
}

void
Label::setHorizontalBias(double x)
{
    horizontalBias = x;
}

void
Label::setVerticalBias(double y)
{
    verticalBias = y;
}

double
Label::getHorizontalBias()
{
    return horizontalBias;
}

double
Label::getVerticalBias()
{
    return verticalBias;
}

void
Label::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget*)
{
    Q_UNUSED(option)

    painter->setRenderHint(QPainter::Antialiasing, true);

    if (bCustomPen)
        painter->setPen(pen);
    if (bCustomBrush)
        painter->setBrush(brush);

    QPolygon poly;

    int size{0};
    if (bCustomSize)
        size = this->size;

    switch (mode)
    {
    case -1:
        // ------------- Рисуем забавную хрень -------------
        if (!bCustomSize)
            size = defaultSize2;
        if (!bCustomPen)
            painter->setPen(QPen(QColor(100, 110, 150), 3));
        if (!bCustomBrush)
        {
            painter->setBrush(QBrush(QColor(0, 230, 0)));
        }
        drawArrow(painter, 0, 0, size / 2, 5 * size / 10, 15 * size / 10);
        drawFatArrow(painter, 0, 0, size / 2, 5 * size / 10, 10 * size / 10);
        /* 15+10 = 25 */
        m_boundingRect
            = QRectF(-size / 4, -15 * size / 10, size / 2, 25 * size / 10 + 2);
        //--------------------------------------------------
        break;
    case 0:
        // ------------- Рисуем кружочек -------------------
        if (!bCustomSize)
            size = defaultSize1;
        if (!bCustomPen)
            painter->setPen(QPen(QColor(100, 110, 150), 3));
        if (!bCustomBrush)
            painter->setBrush(QBrush(QColor(0, 255, 255)));
        painter->drawEllipse(-size / 4, -size / 4, size / 2, size / 2);
        m_boundingRect = QRectF(-size / 4, -size / 4, size / 2, size / 2);
        //--------------------------------------------------
        break;
    case 1:
        // ------------- Рисуем стрелку --------------------
        if (!bCustomSize)
            size = defaultSize1;
        if (!bCustomPen)
            painter->setPen(QPen(QColor(100, 110, 150), 3));
        if (!bCustomBrush)
            painter->setBrush(QBrush(QColor(230, 230, 0)));
        drawArrow(painter, 0, size / 4, size / 2, size / 2, size / 2);
        m_boundingRect = QRectF(-size / 4, -size / 4, size / 2, size / 2);
        //--------------------------------------------------
        break;
    case 2:
        // ------------- Рисуем домик ----------------------
        if (!bCustomSize)
            size = defaultSize2;
        if (!bCustomPen)
            painter->setPen(QPen(QColor(100, 110, 150), 3));
        if (!bCustomBrush)
            painter->setBrush(QBrush(QColor(0, 230, 0)));
        drawFatArrow(
            painter, 0, 0, size / 2, 5 * size / 10, 15 * size / 10, true);
        m_boundingRect = QRectF(
            -size / 4,
            -(5 * size / 10 + ((15 * size / 10 - 5 * size / 10) / 2) + 2),
            size / 2,
            15 * size / 10); // + 2 было в drawShip на центрирование
        // внутри if (inTheMiddle)

        // Или:
        // drawShip(painter, 0, 0, size / 2, 5 * size / 10, 15 * size / 10);
        // m_boundingRect = QRectF(-size / 4, 0, size / 2, 15 * size / 10);
        //--------------------------------------------------
        break;
    case 3:
        // ---------------------- Рисуем ромбик --------------------------
        if (!bCustomSize)
            size = defaultSize1;
        if (!bCustomPen)
            painter->setPen(QPen(QColor(100, 110, 150), 3));
        if (!bCustomBrush)
            painter->setBrush(QBrush(QColor(255, 190, 80)));

        // ---- Здесь рисуется какой-то неправильный ромб ----
        QPolygon poly;
        poly << QPoint(0, -size / 2);
        poly << QPoint(size / 4, 0);
        poly << QPoint(0, size / 4);
        poly << QPoint(-size / 4, 0);
        painter->drawPolygon(poly);
        QPainterPath path;
        path.addPolygon(poly);
        painter->fillPath(path, painter->brush()); //  заливаем эту фигуру
        m_boundingRect = QRectF(-size / 4, -size / 2, size / 2, 3 * size / 4);
        // ---------------------------------------------------------------

        //-------------------------------------------------------
        break;
        // default:
        // break;
    }
}

void
Label::mousePressEvent(QGraphicsSceneMouseEvent* pe)
{
    // QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    if (pe->button() == Qt::MouseButton::LeftButton)
    {
        emit clickLabel(this);
    }
    QGraphicsItem::mousePressEvent(pe);
}

void
Label::mouseReleaseEvent(QGraphicsSceneMouseEvent* pe)
{
    // QApplication::restoreOverrideCursor();
    if (pe->button() == Qt::MouseButton::LeftButton)
    {
        emit endedMoveLabel(this);
    }
    QGraphicsItem::mouseReleaseEvent(pe);
}

void
Label::mouseMoveEvent(QGraphicsSceneMouseEvent* pe)
{
    emit movedLabel(this);
    QGraphicsItem::mouseMoveEvent(pe);
}
