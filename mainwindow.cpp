#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surf_mesh_extruded.h"
#include "surf_mesh_revolved.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
