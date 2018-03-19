#include "mainwindow.h"
#include <QApplication>
#include "graphwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    GraphWidget *widget = new GraphWidget;
    QMainWindow mainWindow;
    mainWindow.setCentralWidget(widget);

    mainWindow.show();

    return a.exec();
}
