#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customrect.h"
#include <QtCore>
#include <QtGui>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->listView->setMaximumWidth(300);
}


void MainWindow::on_AddRect_clicked()
{
    CustomRect *rec = new CustomRect();
    scene->addItem(rec);

}


















MainWindow::~MainWindow()
{
    delete ui;
}


