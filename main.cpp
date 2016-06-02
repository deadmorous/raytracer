#include "mainwindow.h"
#include <QApplication>

#include "serial.h"
#include "ray_tracer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    {
        using namespace raytracer;
        FileReader::Ptr f = FileReader::newInstance("JsonFileReader");
        RayTracer rayTracer;
        rayTracer.read(f->read("/home/stepan/edu/raytracer/scenes/scene_01.scn"));
    }

    return a.exec();
}
