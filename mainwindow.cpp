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
    resize(800, 800); // Фиксированный размер для начала
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Рекурсивная функция для одной стороны снежинки Коха
void drawKochLine(QPainter &painter, QPointF p1, QPointF p2, int depth) {
    if (depth == 0) {
        painter.drawLine(p1, p2);
    } else {
        // Делим отрезок на 3 части
        QPointF p3 = p1 + (p2 - p1) / 3.0;
        QPointF p4 = p1 + (p2 - p1) * 2.0 / 3.0;

        // Вычисляем вершину выступа (угол -60 градусов для правильного роста наружу)
        double angle = -M_PI / 3.0;
        double dx = p4.x() - p3.x();
        double dy = p4.y() - p3.y();

        QPointF p5(
            p3.x() + dx * cos(angle) - dy * sin(angle),
            p3.y() + dx * sin(angle) + dy * cos(angle)
            );

        // Рекурсивно вызываем для 4 получившихся сегментов
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
    painter.setPen(QPen(Qt::blue, 1.5));

    // Центр и радиус для правильного равностороннего треугольника
    double cx = width() / 2.0;
    double cy = height() / 2.0;
    double R = 250.0; // Радиус описанной окружности

    // Вершины исходного треугольника (направленного вверх)
    QPointF p1(cx, cy - R);                                     // Верхняя вершина
    QPointF p2(cx - R * cos(M_PI / 6.0), cy + R * sin(M_PI / 6.0)); // Нижняя левая
    QPointF p3(cx + R * cos(M_PI / 6.0), cy + R * sin(M_PI / 6.0)); // Нижняя правая

    int depth = 1; // Попробуйте начать с 3 или 4

    // Рисуем три стороны снежинки (обратите внимание на порядок точек по часовой стрелке)
    drawKochLine(painter, p1, p3, depth);
    drawKochLine(painter, p3, p2, depth);
    drawKochLine(painter, p2, p1, depth);
}