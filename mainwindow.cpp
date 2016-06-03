#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serial.h"
#include <QApplication>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, SIGNAL(triggered(bool)), SLOT(close()));
    connect(ui->actionOpenScene, SIGNAL(triggered(bool)), SLOT(openScene()));
    connect(ui->actionSaveRaytracerImage, SIGNAL(triggered(bool)), SLOT(saveRayTracerImage()));
    ui->actionSaveRaytracerImage->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openScene()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open scene"), QString(), tr("Scenes (*.scn)"));
    if (fileName.isEmpty())
        return;

    using namespace raytracer;
    FileReader::Ptr f = FileReader::newInstance("JsonFileReader");
    m_rayTracer = RayTracer();
    m_rayTracer.read(f->read(fileName));
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    m_rayTracer.run();
    qApp->restoreOverrideCursor();
    Camera::Ptr cam = m_rayTracer.camera();
    if (cam) {
        ui->label->setText(QString());
        ui->label->setPixmap(QPixmap::fromImage(m_rayTracer.camera()->image()));
        ui->actionSaveRaytracerImage->setEnabled(true);
    }
    else {
        ui->label->setPixmap(QPixmap());
        ui->label->setText(tr("There is no camera, nothing to show here"));
        ui->actionSaveRaytracerImage->setEnabled(false);
    }
}

void MainWindow::saveRayTracerImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ray tracer image"), QString(), tr("Images (*.png *.jpg *.jpeg)"));
    if (fileName.isEmpty())
        return;
    if (fileName.indexOf(QRegExp("\\.png$|\\.jpe?g$")) == -1)
        fileName += ".png";
    m_rayTracer.camera()->image().save(fileName);
}
