#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // диапазон ползунка
    ui->horizontalSlider->setRange(0, 10);
    ui->horizontalSlider->setValue(currentDepth);

    ui->label->setText(QString("Глубина: %1").arg(currentDepth));
    updateSnowflakePath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addKochLine(QPainterPath &path, const QPointF &p1, const QPointF &p2, int depth) {
    if (depth == 0) {
        path.lineTo(p2);
    } else {
        QPointF p3 = p1 + (p2 - p1) / 3.0;
        QPointF p4 = p1 + (p2 - p1) * 2.0 / 3.0;

        QPointF v = p4 - p3;

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

void MainWindow::updateSnowflakePath() {
    cachedPath = QPainterPath();

    double cx = width() / 2.0;
    double cy = (height() - 50) / 2.0 + 10;
    double R = 200.0;

    QPointF p1(cx, cy - R);
    QPointF p2(cx - R * sqrt(3.0) / 2.0, cy + R / 2.0);
    QPointF p3(cx + R * sqrt(3.0) / 2.0, cy + R / 2.0);

    cachedPath.moveTo(p1);
    addKochLine(cachedPath, p1, p2, currentDepth);
    addKochLine(cachedPath, p2, p3, currentDepth);
    addKochLine(cachedPath, p3, p1, currentDepth);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // цвет и толщина
    painter.setPen(QPen(Qt::blue, 1));
    painter.drawPath(cachedPath);
}

void MainWindow::on_horizontalSlider_valueChanged(int value) {
    currentDepth = value;
    ui->label->setText(QString("Глубина: %1").arg(currentDepth));
    updateSnowflakePath(); // пересчитываем один раз
    update();              // быстрый вызов перерисовки
}