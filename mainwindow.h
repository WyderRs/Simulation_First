#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/*=========================================*/
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>
/*=========================================*/
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*=========================================*/
class QGraphicsScene;
class QGraphicsRectItem;
class MotionWorker;
class QThread;
/*=========================================*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /*=========================================*/
    QGraphicsScene              *m_scene;
    QGraphicsView               *m_view;
    QList<QGraphicsRectItem*>   m_items;

    QThread *m_workerThread = nullptr;
    MotionWorker *m_worker = nullptr;


    void createItems();

private slots:
    void applyPositions(const QVector<QPointF> &positions);



    /*=========================================*/

};
#endif // MAINWINDOW_H
