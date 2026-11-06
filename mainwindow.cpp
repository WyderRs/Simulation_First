#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

#include "motionworker.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scene(new QGraphicsScene(this))   // сцена принадлежит окну
{
    ui->setupUi(this);

    // Сцена принадлежит MainWindow

    // Привязываем сцену к уже существующему graphicsView из UI
    ui->graphicsView->setScene(m_scene);

    // Область сцены = размеру представления (или задайте вручную)
    m_scene->setSceneRect(0, 0, 800, 600);

    // Плавная отрисовка и корректное обновление при перетаскивании
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag); // опционально

    createItems();


    // --- Настройка рабочего потока ---
    m_workerThread = new QThread(this);
    m_worker = new MotionWorker();              // без родителя — переедем в поток
    m_worker->moveToThread(m_workerThread);

    // Сигнал из потока → слот в главном потоке (автоматически QueuedConnection)
    connect(m_worker, &MotionWorker::positionsReady,
            this, &MainWindow::applyPositions);

    // Запускаем таймер worker-а уже в его потоке
    connect(m_workerThread, &QThread::started,
            m_worker, &MotionWorker::start);

    // Корректное завершение при закрытии окна
    connect(m_workerThread, &QThread::finished,
            m_worker, &QObject::deleteLater);

    m_workerThread->start();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createItems()
{
    for (int i = 0; i < 10; ++i) {
        QGraphicsRectItem *item = m_scene->addRect(0, 0, 50, 50);
        item->setPos((i % 5) * 120 + 50, (i / 5) * 150 + 50);
        item->setBrush(QColor::fromHsv((i * 36) % 360, 200, 200));
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setFlag(QGraphicsItem::ItemIsSelectable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        m_items.append(item);
    }
}


void MainWindow::applyPositions(const QVector<QPointF> &positions)
{
    // ВСЕГДА в главном потоке — можно безопасно двигать элементы
    const int n = qMin(positions.size(), m_items.size());
    for (int i = 0; i < n; ++i) {
        m_items[i]->setPos(positions[i]);
    }
}


