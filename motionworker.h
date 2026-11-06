#ifndef MOTIONWORKER_H
#define MOTIONWORKER_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QRectF>

class QTimer;

// Состояние одного элемента: позиция + скорость
struct ItemState {
    QPointF pos;
    QPointF vel;
};

class MotionWorker : public QObject
{
    Q_OBJECT

public:
    explicit MotionWorker(QObject *parent = nullptr);
    ~MotionWorker() override;

public slots:
    void start();   // запускает таймер в рабочем потоке
    void stop();    // останавливает таймер

signals:
    // Передаём в главный поток только новые позиции
    void positionsReady(const QVector<QPointF> &positions);

private slots:
    void tick();

private:
    QTimer *m_timer = nullptr;
    QVector<ItemState> m_states;
    QRectF m_bounds;            // границы сцены
    QSizeF m_itemSize;          // размер одного прямоугольника
};

#endif // MOTIONWORKER_H
