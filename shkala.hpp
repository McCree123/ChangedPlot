#ifndef SHKALA_HPP
#define SHKALA_HPP

#include <QGraphicsItem>

class Shkala : public QGraphicsItem
{
private:
    double m_pixelsInMeter{1.0};

    // размеры внутри QGraphicsView по умолчанию
    QRectF m_boundingRect{0, 0, 500, 500};

    double heightMeters{0};
    int heightPixels{0};
    int scaleStep{0};
    bool reverseNum{true};

    // Цвет рисок и подписей
    QColor color{250, 210, 1};

    // Цвет фона
    QColor backgroundColor{0, 50, 1};

    void draw_shkala(QPainter* painter, double height_meters, int height_pixels,
                     int start_point_x, int start_point_y, int scaleStep,
                     int kratnost_dl_riski, bool bRight = true,
                     bool bReverseNum                  = true,
                     int num_repeat_for_other_channels = 1,
                     int width_shkala                  = 100);

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
                       QWidget*);

public:
    Shkala();
    void setBoundingRect(const QRectF& boundingRect);
    double getHeightMeters() const;
    void setHeightMeters(double value);
    int getHeightPixels() const;
    void setHeightPixels(int value);
    double getPixelsInMeter() const;
    void setPixelsInMeter(double pixelsInMeter);
    int getScaleStep() const;
    void setScaleStep(int value);
    int getReverseNum() const;
    void setReverseNum(bool isReverse);

    // Получить цвет рисок и подписей
    QColor getColor() const;

    // Установить цвет рисок и подписей
    void setColor(const QColor& value);

    // Получить цвет фона
    QColor getBackgroundColor() const;

    // Установить цвет фона
    void setBackgroundColor(const QColor& value);
};

#endif // SHKALA_HPP
