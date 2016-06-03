#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "ray_tracer.h"

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

private:
    raytracer::RayTracer m_rayTracer;
};

#endif // MAINWINDOW_H
