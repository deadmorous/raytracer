#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTime>

#include "ray_tracer.h"
#include "ray_tracer_controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void openScene();
    void openScene(const QString& fileName);
    void saveRayTracerImage();
    void imageProcessorChanged(raytracer::ImageProcessor::Ptr imgProc);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void rayTracerProgress(float progress, quint64 raysProcessed);
    void rayTracerFinished(QString error);

private:
    raytracer::RayTracer m_rayTracer;
    raytracer::RayTracerController m_rayTracerController;
    QTime m_startTime;

    void reloadImage();
};

#endif // MAINWINDOW_H
