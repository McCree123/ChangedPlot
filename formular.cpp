#include "formular.hpp"

int
Formular::getValueX() const
{
    return valueX;
}

void
Formular::setValueX(int value)
{
    valueX = value;
}

int
Formular::getValueY() const
{
    return valueY;
}

void
Formular::setValueY(int value)
{
    valueY = value;
}

QString
Formular::getStringMeasurementOnX() const
{
    return stringMeasurementOnX;
}

void
Formular::setStringMeasurementOnX(const QString& value)
{
    stringMeasurementOnX = value;
}

QString
Formular::getStringMeasurementOnY() const
{
    return stringMeasurementOnY;
}

void
Formular::setStringMeasurementOnY(const QString& value)
{
    stringMeasurementOnY = value;
}

void
Formular::setWidth(int value)
{
    m_boundingRect.setWidth(value);
}

void
Formular::setHeight(int value)
{
    m_boundingRect.setHeight(value);
}

int
Formular::getWidth()
{
    return m_boundingRect.width();
}

int
Formular::getHeight()
{
    return m_boundingRect.height();
}

int
Formular::getSizeFont() const
{
    return sizeFont;
}

void
Formular::setSizeFont(int value)
{
    sizeFont = value;
}

QString
Formular::getLegendX() const
{
    return legendX;
}

void
Formular::setLegendX(const QString& value)
{
    legendX = value;
}

QString
Formular::getLegendY() const
{
    return legendY;
}

void
Formular::setLegendY(const QString& value)
{
    legendY = value;
}

QColor
Formular::getColorBackground() const
{
    return colorBackground;
}

void
Formular::setColorBackground(const QColor& value)
{
    colorBackground = value;
}

QColor
Formular::getColorFont() const
{
    return colorFont;
}

void
Formular::setColorFont(const QColor& value)
{
    colorFont = value;
}

void
Formular::draw_formular(QPainter* painter, int text_point_x, int text_point_y,
                        int width_window, int height_window)
{
    // ------ Шапка
    QBrush* brush = new QBrush(colorBackground);
    painter->setBrush(*brush);
    painter->drawRect(0, 0, width_window, height_window);
    // Статический текст
    painter->setPen(colorFont);

    QFont font = painter->font();
    font.setPixelSize(sizeFont);
    painter->setFont(font);

    int lenLegendXInPx = legendX.length() * sizeFont / 1.41;
    int lenLegendYInPx = legendY.length() * sizeFont / 1.41;

    painter->drawText(text_point_x,
                      text_point_y + 5,
                      lenLegendXInPx,
                      12,
                      Qt::AlignRight,
                      legendX);

    painter->drawText(text_point_x,
                      text_point_y + 26,
                      lenLegendYInPx,
                      12,
                      Qt::AlignRight,
                      legendY);

    QString str              = "%1 %2";
    QString strMeasureValueX = str.arg(valueX).arg(stringMeasurementOnX);
    QString strMeasureValueY = str.arg(valueY).arg(stringMeasurementOnY);

    int lenStrMeasureValueX = strMeasureValueX.length() * sizeFont / 1.41;
    int lenStrMeasureValueY = strMeasureValueY.length() * sizeFont / 1.41;

    painter->drawText(text_point_x + lenLegendXInPx,
                      text_point_y + 5,
                      lenStrMeasureValueX,
                      12,
                      Qt::AlignLeft,
                      strMeasureValueX);
    painter->drawText(text_point_x + lenLegendYInPx,
                      text_point_y + 26,
                      lenStrMeasureValueY,
                      12,
                      Qt::AlignLeft,
                      strMeasureValueY);

    delete brush;
}

Formular::Formular() {}

QRectF
Formular::boundingRect() const
{
    return m_boundingRect; // размеры QGraphicsItem
}

void
Formular::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Создаём формуляр
    draw_formular(
        painter, 0, 0, m_boundingRect.width(), m_boundingRect.height());
}

void
Formular::mousePressEvent(QGraphicsSceneMouseEvent* pe)
{
    // New cursor
    // QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    // Вызываем обработчик родителя(для дальнейшей обработки события в Qt и
    // системе)
    QGraphicsItem::mousePressEvent(pe);
}

void
Formular::mouseReleaseEvent(QGraphicsSceneMouseEvent* pe)
{
    // Old cursor
    // QApplication::restoreOverrideCursor();
    QGraphicsItem::mouseReleaseEvent(pe);
}

void
Formular::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    // Вызываем обработчик родителя(для дальнейшей обработки события в Qt и
    // системе) (иначе перестанет перемещаться фигура ибо mouseMoveEvent не
    // отлавливается
    QGraphicsItem::mouseMoveEvent(event);
}
