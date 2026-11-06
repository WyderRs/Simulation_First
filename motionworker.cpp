#include "motionworker.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QtMath>

MotionWorker::MotionWorker(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))          // таймер создаётся в рабочем потоке
    , m_bounds(0, 0, 800, 600)
    , m_itemSize(50, 50)
{
    m_timer->setInterval(16);            // ~60 FPS
    connect(m_timer, &QTimer::timeout, this, &MotionWorker::tick);
}

MotionWorker::~MotionWorker() = default;

void MotionWorker::start()
{
    // Инициализируем состояния, если ещё не сделано
    if (m_states.isEmpty()) {
        auto *rng = QRandomGenerator::global();
        for (int i = 0; i < 10; ++i) {
            ItemState s;
            s.pos = QPointF((i % 5) * 120 + 50, (i / 5) * 150 + 50);
            // случайная скорость в диапазоне [-2, 2]
            s.vel = QPointF(rng->generateDouble() * 4 - 2.0,
                            rng->generateDouble() * 4 - 2.0);
            m_states.append(s);
        }
    }
    m_timer->start();
}

void MotionWorker::stop()
{
    m_timer->stop();
}

void MotionWorker::tick()
{
    QVector<QPointF> newPositions;
    newPositions.reserve(m_states.size());

    const qreal minX = m_bounds.left();
    const qreal maxX = m_bounds.right() - m_itemSize.width();
    const qreal minY = m_bounds.top();
    const qreal maxY = m_bounds.bottom() - m_itemSize.height();

    for (ItemState &s : m_states) {
        // Перемещение
        s.pos += s.vel;

        // Отскок от границ (инвертируем скорость и корректируем позицию)
        if (s.pos.x() < minX) { s.pos.setX(minX); s.vel.setX(-s.vel.x()); }
        if (s.pos.x() > maxX) { s.pos.setX(maxX); s.vel.setX(-s.vel.x()); }
        if (s.pos.y() < minY) { s.pos.setY(minY); s.vel.setY(-s.vel.y()); }
        if (s.pos.y() > maxY) { s.pos.setY(maxY); s.vel.setY(-s.vel.y()); }

        newPositions.append(s.pos);
    }

    emit positionsReady(newPositions);
}
