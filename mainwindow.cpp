#include "mainwindow.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>
#include <queue>
MyPaintWidget::MyPaintWidget(QWidget *parent) : QWidget(parent) {
    canvas = QImage(800, 600, QImage::Format_RGB32);
    canvas.fill(Qt::white);
    setFixedSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);

    setupToolbar(parent);
}

void MyPaintWidget::setupToolbar(QWidget* parent) {
    auto *toolbar = new QToolBar("Tools", parent);

    auto *pencilAction = toolbar->addAction("Карандаш");
    auto *eraserAction = toolbar->addAction("Ластик");
    auto *colorAction = toolbar->addAction("Цвет");
    auto *fillAction = toolbar->addAction("Заливка");
    connect(pencilAction, &QAction::triggered, [this]() {
        currentTool = Pencil;
    });

    connect(eraserAction, &QAction::triggered, [this]() {
        currentTool = Eraser;
    });

    connect(colorAction, &QAction::triggered, [this]() {
        chooseColor();
    });

    if (auto *layout = dynamic_cast<QVBoxLayout*>(parent->layout())) {
        layout->insertWidget(0, toolbar);
    }
    connect(fillAction, &QAction::triggered, [this]() {
        currentTool = Fill;
    });
}

void MyPaintWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
}

void MyPaintWidget::pushUndo() {
    undoStack.push(canvas);
    redoStack.clear();
}

void MyPaintWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (currentTool == Fill) {
            QColor targetColor = canvas.pixelColor(event->pos());
            if (targetColor != currentColor) {
                pushUndo();
                floodFill(event->pos(), targetColor, currentColor);
                update();
            }
        } else {
            pushUndo();
            lastPoint = event->pos();
            drawing = true;
        }
    }
}


void MyPaintWidget::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        QPainter painter(&canvas);
        QColor color = (currentTool == Eraser) ? Qt::white : currentColor;
        painter.setPen(QPen(color, 10, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint, event->pos());
        lastPoint = event->pos();
        update();
    }
}
void MyPaintWidget::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z) {
        undo();
    } else if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) &&
               event->key() == Qt::Key_Z) {
        redo();
    }
}

void MyPaintWidget::undo() {
    if (!undoStack.isEmpty()) {
        redoStack.push(canvas);
        canvas = undoStack.pop();
        update();
    }
}

void MyPaintWidget::redo() {
    if (!redoStack.isEmpty()) {
        undoStack.push(canvas);
        canvas = redoStack.pop();
        update();
    }
}
void MyPaintWidget::chooseColor() {
    QColor color = QColorDialog::getColor(currentColor, this, "Выберите цвет");
    if (color.isValid()) {
        currentColor = color;
    }
}
void MyPaintWidget::floodFill(QPoint startPoint, QColor targetColor, QColor newColor) {
    if (targetColor == newColor)
        return;

    std::queue<QPoint> points;
    points.push(startPoint);

    while (!points.empty()) {
        QPoint p = points.front();
        points.pop();

        if (!canvas.rect().contains(p))
            continue;

        if (canvas.pixelColor(p) != targetColor)
            continue;

        canvas.setPixelColor(p, newColor);

        points.push(QPoint(p.x() + 1, p.y()));
        points.push(QPoint(p.x() - 1, p.y()));
        points.push(QPoint(p.x(), p.y() + 1));
        points.push(QPoint(p.x(), p.y() - 1));
    }
}
