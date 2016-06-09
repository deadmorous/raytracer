#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serial.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

template< class F >
inline void wrapSignalHandler(QWidget* parent, F f) {
    try { f(); }
    catch( const std::exception& e ) {
        QMessageBox::critical(parent, QString(), QString::fromUtf8(e.what()));
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_rayTracerController(m_rayTracer)
{
    ui->setupUi(this);
    connect(ui->actionQuit, SIGNAL(triggered(bool)), SLOT(close()));
    connect(ui->actionOpenScene, SIGNAL(triggered(bool)), SLOT(openScene()));
    connect(ui->actionSaveRaytracerImage, SIGNAL(triggered(bool)), SLOT(saveRayTracerImage()));
    ui->actionSaveRaytracerImage->setEnabled(false);

    connect(&m_rayTracerController, SIGNAL(rayTracerImageUpdated(QPixmap)), ui->label, SLOT(setPixmap(QPixmap)), Qt::QueuedConnection);
    connect(&m_rayTracerController, SIGNAL(rayTracerProgress(float,quint64)), SLOT(rayTracerProgress(float,quint64)), Qt::QueuedConnection);
    connect(&m_rayTracerController, SIGNAL(rayTracerFinished()), SLOT(rayTracerFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openScene()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open scene"), QString(), tr("Scenes (*.scn)"));
    if (!fileName.isEmpty())
        openScene(fileName);
}

void MainWindow::openScene(const QString& fileName)
{
    wrapSignalHandler(this, [this, fileName]() {
        using namespace raytracer;
        FileReader::Ptr f = FileReader::newInstance("JsonFileReader");
        m_rayTracerController.stop();
        m_rayTracer = RayTracer();
        m_rayTracer.read(f->read(fileName));
        Camera::Ptr cam = m_rayTracer.camera();
        if (cam) {
            ui->label->setText(QString());
            ui->label->setPixmap(QPixmap::fromImage(cam->image()));
            m_startTime.start();
            m_rayTracerController.start();
            ui->actionSaveRaytracerImage->setEnabled(true);
        }
        else {
            ui->label->setPixmap(QPixmap());
            ui->label->setText(tr("There is no camera, nothing to show here"));
            ui->actionSaveRaytracerImage->setEnabled(false);
        }
    });
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_rayTracerController.stop();
    event->accept();
}

void MainWindow::rayTracerProgress(float progress, quint64 raysProcessed)
{
    ui->statusBar->showMessage(
                tr("Tracing scene: %1 of %2 rays, %3% done, %4 s")
                .arg(QString::number(static_cast<double>(raysProcessed), 'e', 3))
                .arg(QString::number(static_cast<double>(m_rayTracer.options().totalRayLimit), 'e', 3))
                .arg(progress*100)
                .arg(m_startTime.elapsed()/1000.));
}

void MainWindow::rayTracerFinished()
{
    ui->statusBar->showMessage(tr("Raytracer finished, %1 s elapsed").arg(m_startTime.elapsed()/1000.));
}
