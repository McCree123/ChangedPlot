#include "shkala.hpp"

#include <QPainter>

Shkala::Shkala() {}

QRectF
Shkala::boundingRect() const
{
    return m_boundingRect;
}

void
Shkala::setBoundingRect(const QRectF& boundingRect)
{
    m_boundingRect = boundingRect;
}

void
Shkala::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // - 20, поскольку не затрагиваем риски шкал длинной
    // smeschenie_dlja_riski и smeschenie_dlja_riski*2
    painter->fillRect(0,
                      0,
                      m_boundingRect.width() - 20,
                      m_boundingRect.height(),
                      QBrush(backgroundColor));

    draw_shkala(painter,
                heightMeters,
                heightPixels,
                41,
                1,
                scaleStep,
                3,
                true,
                reverseNum);

    // delete brush;
}

double
Shkala::getHeightMeters() const
{
    return heightMeters;
}

void
Shkala::setHeightMeters(double value)
{
    heightMeters = value;
}

int
Shkala::getHeightPixels() const
{
    return heightPixels;
}

void
Shkala::setHeightPixels(int value)
{
    heightPixels = value;
}

double
Shkala::getPixelsInMeter() const
{
    return m_pixelsInMeter;
}

void
Shkala::setPixelsInMeter(double pixelsInMeter)
{
    m_pixelsInMeter = pixelsInMeter;
}

int
Shkala::getScaleStep() const
{
    return scaleStep;
}

void
Shkala::setScaleStep(int value)
{
    scaleStep = value;
}

int
Shkala::getReverseNum() const
{
    return reverseNum;
}

void
Shkala::setReverseNum(bool isReverse)
{
    reverseNum = isReverse;
}

QColor
Shkala::getColor() const
{
    return color;
}

void
Shkala::setColor(const QColor& value)
{
    color = value;
}

QColor
Shkala::getBackgroundColor() const
{
    return backgroundColor;
}

void
Shkala::setBackgroundColor(const QColor& value)
{
    backgroundColor = value;
}

void
Shkala::draw_shkala(QPainter* painter, double height_meters, int height_pixels,
                    int start_point_x, int start_point_y, int scaleStep,
                    int kratnost_dl_riski, bool bRight, bool bReverseNum,
                    int num_repeat_for_other_channels, int width_shkala)
{
    // Каждые пять метров будет чёрточка от точки (x,y) = (50, 50), все чёрточки
    // смотрят вправо, с кратностью три будет длинная чёрточка, длинна шкалы 290
    // м, цифры расположены сверху вниз в порядке возрастания, через каждые 175
    // пикселов вставлено 18 шкал для каждого окошка канала
    // draw_shkala(painter, 290, 50, 50, 5, 3, true, false, 18, 175);
    // ---------------- </Использование этой функции> ----------------

    int num_meters, smeschenie_dlja_riski, a, dist_num_to_scale,
        height_in_pixels;
    int size_font_num = 10, size_compensation_length = 3;

    painter->setPen(QPen(QBrush(color), 2));

    QFont font;
    for (int i = 0; i < num_repeat_for_other_channels; i++)
    {
        painter->setPen(QPen(QBrush(color), 3));

        height_in_pixels = height_meters * m_pixelsInMeter;
        if (height_in_pixels > height_pixels)
        {
            height_in_pixels = height_pixels;
        }

        painter->drawLine(start_point_x + (width_shkala * i),
                          start_point_y,
                          start_point_x + (width_shkala * i),
                          start_point_y + height_in_pixels);

        a                     = 0;
        smeschenie_dlja_riski = 10;

        if (!bRight)
        {
            smeschenie_dlja_riski = -smeschenie_dlja_riski;
        }
        font = painter->font();
        font.setPointSize(size_font_num);
        painter->setFont(font);

        QString str;
        for (int iMeter = 0; iMeter < height_meters; iMeter += scaleStep, a++)
        {
            if (iMeter * m_pixelsInMeter > height_pixels)
            {
                break;
            }
            // Если индекс a кратен двум, то
            if ((a % kratnost_dl_riski) == 0)
            {
                // Линия подлинее
                painter->setPen(QPen(QBrush(color), 3));
                painter->drawLine(start_point_x + (width_shkala * i)
                                      + smeschenie_dlja_riski * 2,
                                  start_point_y + iMeter * m_pixelsInMeter,
                                  start_point_x + (width_shkala * i),
                                  start_point_y + iMeter * m_pixelsInMeter);
            }
            // Иначе
            else
            {
                painter->setPen(QPen(QBrush(color), 2));
                painter->drawLine(start_point_x + (width_shkala * i)
                                      + smeschenie_dlja_riski,
                                  start_point_y + iMeter * m_pixelsInMeter,
                                  start_point_x + (width_shkala * i),
                                  start_point_y + iMeter * m_pixelsInMeter);
            }

            dist_num_to_scale = 10;

            int* compensation_length = new int[size_compensation_length];
            if (bReverseNum)
            {
                num_meters = height_meters - iMeter;
            }
            else
            {
                num_meters = iMeter;
            }
            str.setNum(num_meters);

            if (bRight)
            {
                // Увеличим этими значениями dist_num_to_scale на длинну самого
                // текста compensation_length[0] для однозначного,
                // compensation_length[1] для двухзначного,
                // compensation_length[2] для трёхзначного, и т.д.
                for (int i = 0; i < size_compensation_length; i++)
                {
                    compensation_length[i] = 7 + 8 * i;
                }
            }
            else
            {
                dist_num_to_scale = -dist_num_to_scale;
                for (int i = 0; i < size_compensation_length; i++)
                {
                    compensation_length[i] = 0;
                }
            }
            if (num_meters < 10)
            {
                painter->drawText(start_point_x + (width_shkala * i)
                                      - dist_num_to_scale
                                      - compensation_length[0],
                                  start_point_y + iMeter * m_pixelsInMeter
                                      + size_font_num + 5,
                                  str);
            }
            else if (num_meters < 100)
            {
                painter->drawText(start_point_x + (width_shkala * i)
                                      - dist_num_to_scale
                                      - compensation_length[1],
                                  start_point_y + iMeter * m_pixelsInMeter
                                      + size_font_num + 5,
                                  str);
            }
            else
            {
                painter->drawText(start_point_x + (width_shkala * i)
                                      - dist_num_to_scale
                                      - compensation_length[2],
                                  start_point_y + iMeter * m_pixelsInMeter
                                      + size_font_num + 5,
                                  str);
            }
        }
    }
}
