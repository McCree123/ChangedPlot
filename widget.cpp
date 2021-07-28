#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->view->setScene(&scene);
}

Widget::~Widget()
{
    delete ui;
}

QVector<int>
Widget::getValuesY()
{
    return graphic->getValuesY();
}

void
Widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    if (bRedrawScales)
    {
        // Получается, что обновляем сцену с шкалой вот так:
        // bRedrawScales = true; // требуем обновления шага шкал
        // maxYValue = 50; maxXValue = 500; // чему то там приравняли
        // repaint(); // перерисовали окно

        // или
        // что то поменяли в сцене
        // repaint(); // просто перерисовали окно, шаг на шкалах менять не надо
        // Оставляем флаг bRedrawScales = false;

        scene.clear(); // уберём старое, в т. ч. и все labels
        labels.clear(); // забудем на него указатели
        // Если зашли сюда для изменения шкал уже нарисованной сцены, то не
        // будет предыдущих basePoints на graphic на основе оставленных ранее
        // labels, поэтому опять же надо отчистить все лежащие на scene labels'ы

        // ------ Это просто примерный задний фон ------
        // scene.addRect(0, 0, 2400, 2400, QPen(), QBrush(QColor(0, 0, 150)));
        // ---------------------------------------------
        // ------------------ График -------------------
        graphic = new Graphic;
        graphic->setPos(40, 0);
        // graphic->setBoundingRect не нужен - graphic сам определит свои
        // размеры при его перерисовке по paint()
        graphic->setXmax(maxXValue);
        graphic->setYmax(maxYValue);
        scene.addItem(graphic);
        // ---------------------------------------------
        sh = new Shkala;
        sh->setPos(0, 0);
        double maxPixels = ui->view->size().height() - 5 - 40;
        sh->setHeightMeters(maxYValue);
        sh->setHeightPixels(maxPixels);
        int step = maxYValue / 5;
        if (step == 0)
            step = 1;
        sh->setScaleStep(step);
        // Для рисования сетки
        graphic->setScaleStepY(step);
        sh->setPixelsInMeter(maxPixels / maxYValue);
        // sh->setReverseNum(true); // true по умолчанию
        // 40 - ширина item шкалы, +20 - максимальная ширина рисок(значения
        // smeschenie_dlja_riski и smeschenie_dlja_riski*2 в Shkala)
        // При + 20, риски будут перерисовываться
        // полностью при наезде на них других элементов, иначе будут частично
        // стираться, т.к. самые большие риски на шкале имеют размер
        // smeschenie_dlja_riski*2.
        sh->setBoundingRect(
            QRectF(0, 0, 40 + 20, ui->view->size().height() - 3));
        sh->setColor(QColor(0, 150, 255));
        scene.addItem(sh);

        sh1 = new Shkala;
        sh1->setPos(40, ui->view->size().height() - 3);
        maxPixels = ui->view->size().width() - 5 - 40;
        sh1->setHeightMeters(maxXValue);
        sh1->setHeightPixels(maxPixels);
        step = int(maxXValue / 10);
        if (step == 0)
            step = 1;
        sh1->setScaleStep(step);
        // Для рисования сетки
        graphic->setScaleStepX(step);
        sh1->setPixelsInMeter(maxPixels / maxXValue);
        sh1->setReverseNum(false);
        sh1->setBoundingRect(
            QRectF(0, 0, 40 + 20, ui->view->size().width() - 3 - 40));
        sh1->setColor(QColor(0, 150, 255));
        scene.addItem(sh1);
        sh1->rotate(-90);

        // Если был paintEvent() в Widget, значит был инициирован и paint()
        // в каждом QGraphicItem, находящемся в дочернем QGraphicsView - поэтому
        // уже увидим и сетку, и шкалы, и фон графика

        // теперь перед перерисовкой будем просто задавать обновлённые из
        // размеров сцены параметры
        bRedrawScales = false;
    }
    else
    {
        // Перерисовка Item'ов шкал в новых размерах QGraphicsView
        scene.setSceneRect(
            0, 0, ui->view->size().width() - 5, ui->view->size().height() - 5);

        double maxPixels = ui->view->size().height() - 5 - 40;
        sh->setHeightPixels(maxPixels);
        sh->setPixelsInMeter(maxPixels / maxYValue);
        sh->setBoundingRect(
            QRectF(0, 0, 40 + 20, ui->view->size().height() - 3));

        maxPixels = ui->view->size().width() - 5 - 40;
        sh1->setPos(40, ui->view->size().height() - 3);
        sh1->setHeightPixels(maxPixels);
        sh1->setPixelsInMeter(maxPixels / maxXValue);
        sh1->setBoundingRect(
            QRectF(0, 0, 40 + 20, ui->view->size().width() - 3 - 40));
    }
    // Перерисовка labels в новых размераx
    for (int i = 0; i < labels.size(); i++)
    {
        QPointF bias;
        bias.setX(labels[i]->getHorizontalBias());
        bias.setY(labels[i]->getVerticalBias());

        // !!!
        // int xStart = 40;
        // int yStart = 0;
        // int xEnd   = ui->view->size().width() - 5;
        // int yEnd   = ui->view->size().height() - 5 - 40;

        // Посчитаем новые координаты по bias labels[i]
        // в новых размерах квадрата под график в QGraphicsView
        QPoint currentCoord = calcCoordOnBias(bias);
        labels[i]->setPos(currentCoord);
    }
}

QPointF
Widget::calcXYOnGraphicsView(int x, int y)
{
    int xOnGraphicsView = x - ui->view->pos().x();
    int yOnGraphicsView = y - ui->view->pos().y();
    if (xOnGraphicsView < 0 || yOnGraphicsView < 0
        || xOnGraphicsView > ui->view->frameRect().width()
        || yOnGraphicsView > ui->view->frameRect().height())
    {
        // Не попали в QGraphicsView
        return QPointF(-1, -1);
    }
    // Иначе
    return QPointF(xOnGraphicsView, yOnGraphicsView);
}

QPointF
Widget::calcBiasOnGraphic(int x, int y)
{
    // Расположение графика
    int xStart = 40;
    int yStart = 0;
    int xEnd   = ui->view->size().width() - 5;
    int yEnd   = ui->view->size().height() - 5 - 40;

    float horizontalBias, verticalBias;
    int X = 0, Y = 0;
    X = x - xStart;
    Y = y - yStart;
    if (X < 0)
        horizontalBias = 0;
    else
    {
        horizontalBias
            = static_cast<float>(X) / static_cast<float>(xEnd - xStart);
    }
    if (Y < 0)
        verticalBias = 0;
    else
    {
        verticalBias
            = static_cast<float>(Y) / static_cast<float>(yEnd - yStart);
    }

    if (horizontalBias > 1)
        horizontalBias = 1;
    if (verticalBias > 1)
        verticalBias = 1;

    return QPointF(horizontalBias, verticalBias);
}

QPoint
Widget::calcCoordOnBias(QPointF bias)
{
    QPoint coordInGraphicInView;

    int xStart = 40;
    int yStart = 0;
    int xEnd   = ui->view->size().width() - 5;
    int yEnd   = ui->view->size().height() - 5 - 40;

    // Посчитаем координаты для Label в графике по найденному bias
    coordInGraphicInView.setX(static_cast<float>(xEnd - xStart) * bias.x()
                              + xStart);
    coordInGraphicInView.setY(static_cast<float>(yEnd - yStart) * bias.y()
                              + yStart);

    return coordInGraphicInView;
}

void
Widget::addedBaseValueOnBias(const QPointF& bias, int i)
{
    QVector<int> vecX = graphic->getBaseValuesX();
    QVector<int> vecY = graphic->getBaseValuesY();
    // bias от 0 до 1 от верхнего угла к нижнему
    // А график идет от нижнего к верхнему, и для него надо от 1 до 0
    // по y, что бы наполнить вектор значений Y правильно
    if (i == -1)
    {
        vecX.push_back(bias.x() * static_cast<double>(maxXValue));
        vecY.push_back((1 - bias.y()) * static_cast<double>(maxYValue));
    }
    else
    {
        vecX[i] = bias.x() * static_cast<double>(maxXValue);
        vecY[i] = (1 - bias.y()) * static_cast<double>(maxYValue);
    }

    graphic->setBaseValuesY(vecY);            // сначала Y
    int* ind = graphic->setBaseValuesX(vecX); // потом X
    // По вернувшемуся индексному массиву отсортируем также и
    // соответствующий координатам базовых точек массив labels
    QVector<Label*> tmpLabels;
    for (int j = 0; j < vecX.size(); j++)
    {
        tmpLabels.push_back(labels[ind[j]]);
    }
    labels.clear();
    for (int j = 0; j < vecX.size(); j++)
    {
        labels.push_back(tmpLabels[j]);
    }
}

void
Widget::deleteBaseValueOnIndex(int i)
{
    QVector<int> vecX = graphic->getBaseValuesX();
    QVector<int> vecY = graphic->getBaseValuesY();
    // Удаляем соответствующую базовую точку на графике
    vecX.remove(i);
    vecY.remove(i);
    graphic->setBaseValuesY(vecY);
    graphic->setBaseValuesXWithNoSort(vecX);
    // Если исключить точку из сортированного по X ряда,
    // ряд так и останется сортированным по X, поэтому нам не нужны
    // возвращаемые номера элементов ind, образующих сортированный ряд,
    // да и вообще сортировка уже сортированного
}

void
Widget::initializeFormularPoint(Label* label)
{
    // Считается значение по оси X из относительного расположения нажатого
    // label по горизонтали графика в сцене с удивительными костылями,
    // без которых это работает плохо
    double magicNum = 1.6;
    double kostil   = 1 + 1. / (static_cast<double>(maxXValue) / magicNum);
    double kostil1  = 1. / (static_cast<double>(maxXValue) / magicNum / 0.55);

    formular->setValueX(((label->getHorizontalBias() / kostil) + kostil1)
                        * static_cast<double>(maxXValue));

    /*
    // Что бы такого не было, надо расчитывать и передавать bias (относительное
    // расположение) в label[i] по центру его площади
    formular->setValueX(label->getHorizontalBias()
                        * static_cast<double>(maxXValue));
    */

    // По значению на оси X выберем Y из значений графика, а не значение на
    // оси Y, соответствующее расположению label на графике
    if (formular->getValueX() < 0)
    {
        formular->setValueX(0);
    }
    else if (formular->getValueX() >= maxXValue)
    {
        formular->setValueX(299);
    }
    formular->setValueY(graphic->getValueY(formular->getValueX()));
}

void
Widget::mouseMoveEvent(QMouseEvent* event)
{
}

void
Widget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (modeWork == MODE_ADD_POINTS)
        {
            QPointF posMouseInView
                = calcXYOnGraphicsView(event->x(), event->y());

            // Если не попали в QGraphicsView
            if (posMouseInView.x() == -1 || posMouseInView.y() == -1)
                return;
            // Если попали
            labels.push_back(new Label());

            // Последний добавленный Label
            Label* lastLabel = labels[labels.size() - 1];

            lastLabel->setMode(0);
            lastLabel->setSize(17);

            // Костыли что бы label[i] появился по центру кончика указателя мыши
            posMouseInView.setX(posMouseInView.x() - 3);
            posMouseInView.setY(posMouseInView.y() - 3);

            QPointF bias
                = calcBiasOnGraphic(posMouseInView.x(), posMouseInView.y());
            lastLabel->setHorizontalBias(bias.x());
            lastLabel->setVerticalBias(bias.y());

            // Если зашли на границу, где начинается поле шкалы,
            // то calcBiasOnGraphic не посчитает bias > 1,
            // соотв. по bias можно найти расположение lastLabel, которое
            // должно быть

            // Так же добавляем в конец соответствующую базовую точку на графике
            // по относительному расположению на графике (по bias), которое 0 <
            // bias < 1
            addedBaseValueOnBias(bias);

            // Перерисуем график
            graphic->update();

            // Посчитаем координаты для lastLabel внутри шкал (в graphic) по
            // найденному bias
            QPoint posInGraphic = calcCoordOnBias(bias);

            // После чего задаётся позиция каждого Label в графике в
            // QGraphicsView
            lastLabel->setPos(posInGraphic);

            lastLabel->setFlags(QGraphicsItem::ItemIsMovable);

            // Свяжем сигналы из Label с слотами Widget
            connect(lastLabel,
                    SIGNAL(clickLabel(Label*)),
                    this,
                    SLOT(onClickLabel(Label*)));

            connect(lastLabel,
                    SIGNAL(endedMoveLabel(Label*)),
                    this,
                    SLOT(onEndedMoveLabel(Label*)));

            connect(lastLabel,
                    SIGNAL(movedLabel(Label*)),
                    this,
                    SLOT(onMovedLabel(Label*)));

            // ----- Добавляем каждый новый Label в сцену -----
            scene.addItem(lastLabel);
            // ------------------------------------------------
        }
    }
}

void
Widget::onClickLabel(Label* label)
{
    if (modeWork == MODE_DELETE_POINTS)
    {
        for (int i = 0; i < labels.size(); i++)
        {
            if (label->x() == labels[i]->x() && label->y() == labels[i]->y())
            {
                // Забудем указатель на него
                labels.remove(i);
                // Удалим из сцены
                scene.removeItem(label);
                // Удалим такое значение из базовых точек в graphic на scene
                deleteBaseValueOnIndex(i);

                // Перерисуем график
                graphic->update();

                // Если осталась какая то ещё точка с точно такими же
                // координатами, не трогаем
                break;
            }
        }
    }
    else
    {
        formular = new Formular();
        formular->setPos(label->x() - 40, label->y());
        initializeFormularPoint(label);
        scene.addItem(formular);
    }
}

void
Widget::recalcBaseValueOnGraphic(Label* label)
{
    // Делаем перерасчет bias для перемещенного Label внутри графика
    QPointF bias = calcBiasOnGraphic(label->x(), label->y());
    // Если была попытка выйти на поле шкал, то принудительно bias
    // ограничится значением 1 и по paintEvent label будет на границе графика
    label->setHorizontalBias(bias.x());
    label->setVerticalBias(bias.y());

    // Вычисление новых значений
    // базовых точек для графика, если менялось расположение labels
    // Находим соответствующие изменяемым координатам базовой точки labels[i]
    for (int i = 0; i < labels.size(); i++)
    {
        if (label->x() == labels[i]->x() && label->y() == labels[i]->y())
        {
            // Если нашли, то пересчитываем по новым bias соответствующие
            // базовые точки на графике
            addedBaseValueOnBias(bias, i);
        }
    }
}

void
Widget::onEndedMoveLabel(Label* label)
{
    recalcBaseValueOnGraphic(label);

    scene.removeItem(formular);

    // Перерисуем график
    graphic->update();

    // Снизу должен быть уже перерисованный график по graphic->update(), так как
    // поймали перемещение label по mouseMoveEvent в Label, так
    // же посчитали и записали в label bias относительно графика и в
    // paintEvent() по repaint() этот label должен встать на свое место по
    // шкалам
    repaint();
}

void
Widget::onMovedLabel(Label* label)
{
    recalcBaseValueOnGraphic(label);

    formular->setPos(label->x() - 40, label->y());
    initializeFormularPoint(label);

    // Перерисуем график
    graphic->update();

    repaint();
}

int
Widget::getMaxXValue() const
{
    return maxXValue;
}

void
Widget::setMaxXValue(int value)
{
    maxXValue     = value;
    bRedrawScales = true; // Требуем обновления шага шкал
                          // (и графика на основе этих шкал)
    repaint();
}

int
Widget::getMaxYValue() const
{
    return maxYValue;
}

void
Widget::setMaxYValue(int value)
{
    maxYValue     = value;
    bRedrawScales = true; // Требуем обновления шага шкал
                          // (и графика на основе этих шкал)
    repaint();
}

void
Widget::on_btnAddPoints_clicked()
{
    modeWork = MODE_ADD_POINTS;
}

void
Widget::on_btnDeletePoints_clicked()
{
    modeWork = MODE_DELETE_POINTS;
}

void
Widget::on_btnChangePosPoints_clicked()
{
    modeWork = MODE_CHANGE_POSITION_POINTS;
}
