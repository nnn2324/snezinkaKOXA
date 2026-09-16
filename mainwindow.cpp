#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText(QString("Глубина: %1").arg(currentDepth));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Рекурсивный шаг кривой Коха
void MainWindow::addKochLine(QPainterPath &path, const QPointF &p1, const QPointF &p2, int depth) {
    if (depth == 0) {
        path.lineTo(p2);
    } else {
        // Делим отрезок на 3 части
        QPointF p3 = p1 + (p2 - p1) / 3.0;
        QPointF p4 = p1 + (p2 - p1) * 2.0 / 3.0;

        // Вектор от p3 к p4
        QPointF v = p4 - p3;

        // Поворот на -60 градусов (-PI / 3) для построения вершины наружу в системе Qt (Y вниз)
        double angle = M_PI / 3.0;
        double cosA = cos(angle);
        double sinA = sin(angle);

        QPointF p5(
            p3.x() + v.x() * cosA - v.y() * sinA,
            p3.y() + v.x() * sinA + v.y() * cosA
            );

        addKochLine(path, p1, p3, depth - 1);
        addKochLine(path, p3, p5, depth - 1);
        addKochLine(path, p5, p4, depth - 1);
        addKochLine(path, p4, p2, depth - 1);
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    double cx = width() / 2.0;
    double cy = (height() - 50) / 2.0 + 10;
    double R = 200.0;

    // Вершины исходного треугольника (правильное расположение для внешней снежинки)
    QPointF p1(cx, cy - R);                                    // Верх
    QPointF p2(cx - R * sqrt(3.0) / 2.0, cy + R / 2.0);        // Низ-лево
    QPointF p3(cx + R * sqrt(3.0) / 2.0, cy + R / 2.0);        // Низ-право

    QPainterPath path;
    path.moveTo(p1);

    // Обход треугольника по часовой стрелке
    addKochLine(path, p1, p2, currentDepth);
    addKochLine(path, p2, p3, currentDepth);
    addKochLine(path, p3, p1, currentDepth);

    painter.setPen(QPen(Qt::blue, 2));
    painter.drawPath(path);
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    currentDepth = value;
    ui->label->setText(QString("Глубина: %1").arg(currentDepth));
    update();
}