#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget window;
    auto *layout = new QVBoxLayout(&window);
    window.setLayout(layout);

    auto *paintWidget = new MyPaintWidget(&window);
    layout->addWidget(paintWidget);

    window.setWindowTitle("Qt Paint");
    window.show();

    return a.exec();
}
