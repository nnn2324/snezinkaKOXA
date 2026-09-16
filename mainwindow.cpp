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
    resize(1500, 1500); // окно
}

MainWindow::~MainWindow()
{
    delete ui;
}

// рекурсивная функция для рисования
void drawKochLine(QPainter &painter, QPointF p1, QPointF p2, int depth) {
    if (depth == 0) {
        painter.drawLine(p1, p2);
    } else {
        // делим отрезок на 3 части
        QPointF p3 = p1 + (p2 - p1) / 3.0;
        QPointF p4 = p1 + (p2 - p1) * 2.0 / 3.0;

        // 60 градусов
        double angle = -M_PI / 3.0;
        double dx = p4.x() - p3.x();
        double dy = p4.y() - p3.y();

        QPointF p5(
            p3.x() + dx * cos(angle) - dy * sin(angle),
            p3.y() + dx * sin(angle) + dy * cos(angle)
            );

        // рекурсивно вызываем
        drawKochLine(painter, p1, p3, depth - 1);
        drawKochLine(painter, p3, p5, depth - 1);
        drawKochLine(painter, p5, p4, depth - 1);
        drawKochLine(painter, p4, p2, depth - 1);
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::blue, 1));

    // центр правильного треугольника
    double cx = width() / 2.0;
    double cy = height() / 2.0;
    double R = 500.0; // радиус описанной окружности

    // вершины исходного треугольника
    QPointF p1(cx, cy - R); // верхняя вершина
    QPointF p2(cx - R * cos(M_PI / 6.0), cy + R * sin(M_PI / 6.0)); // нижняя левая
    QPointF p3(cx + R * cos(M_PI / 6.0), cy + R * sin(M_PI / 6.0)); // нижняя правая

    int depth = 10; // глубина, больше 10 принудительное завершение

    // рисуем три стороны снежинки (соблюдать порядок - по часовой стрелке)
    drawKochLine(painter, p1, p3, depth);
    drawKochLine(painter, p3, p2, depth);
    drawKochLine(painter, p2, p1, depth);
}