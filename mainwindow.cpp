#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QPointF>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800, 800); // окно
}

MainWindow::~MainWindow()
{
    delete ui;
}

// рекурсивка чтобы рисовать снежинку
void drawKochLine(QPainter &painter, QPointF p1, QPointF p2, int depth) {
    if (depth == 0) {
        painter.drawLine(p1, p2);
    } else {
        // промежуточные
        QPointF p3 = p1 + (p2 - p1) / 3.0;
        QPointF p4 = p1 + (p2 - p1) * 2.0 / 3.0;

        // вершина треугольника
        double angle = -M_PI / 3.0; // 60 градусов
        double dx = p4.x() - p3.x();
        double dy = p4.y() - p3.y();

        QPointF p5(
            p3.x() + dx * cos(angle) - dy * sin(angle),
            p3.y() + dx * sin(angle) + dy * cos(angle)
            );

        // тоже самое для остальных
        drawKochLine(painter, p1, p3, depth - 1);
        drawKochLine(painter, p3, p5, depth - 1);
        drawKochLine(painter, p5, p4, depth - 1);
        drawKochLine(painter, p4, p2, depth - 1);
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // цвет и толщина линий
    painter.setPen(QPen(Qt::blue, 1.5));

    // координаты вершин
    // центр посередине окна
    QPointF p1(400, 150);
    QPointF p2(150, 550);
    QPointF p3(650, 550);

    int depth = 2; // глубинв

    // три стороны
    drawKochLine(painter, p1, p2, depth);
    drawKochLine(painter, p2, p3, depth);
    drawKochLine(painter, p3, p1, depth);
}