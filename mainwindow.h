#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QStack> // Новый include
#include <QToolBar>
#include <QPushButton>
#include <QColorDialog>
enum Tool { Pencil, Eraser, Fill };

class MyPaintWidget : public QWidget {
    Q_OBJECT

public:
    enum Tool { Pencil, Eraser,Fill  };

    MyPaintWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QImage canvas;
    QPoint lastPoint;
    bool drawing = false;

    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

    Tool currentTool = Pencil;
    QColor currentColor = Qt::black;
    void pushUndo();
    void undo();
    void redo();

    void setupToolbar(QWidget* parent); // новая функция
    void chooseColor();
    void floodFill(QPoint startPoint, QColor targetColor, QColor newColor);
};

#endif // MAINWINDOW_H
