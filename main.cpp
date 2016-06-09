#include "mainwindow.h"
#include "cxx_exception.h"
#include "ray_tracer.h"
#include "camera.h"

#include <QApplication>
#include <QFileInfo>
#include <QTime>
#include <iostream>
#include <iomanip>

int runInBatchMode(QString sceneFileName, QString imageFileName)
{
    using namespace std;
    using namespace raytracer;
    try {
        FileReader::Ptr f = FileReader::newInstance("JsonFileReader");
        RayTracer rayTracer;
        rayTracer.read(f->read(sceneFileName));
        if (imageFileName.indexOf(QRegExp("\\.png$|\\.jpe?g$")) == -1)
            imageFileName += ".png";
        if (QFileInfo(imageFileName).exists())
            throw cxx::exception(QString("Output image file %1 already exists").arg(imageFileName).toStdString());
        Camera::Ptr cam = rayTracer.camera();
        if (!cam)
            throw cxx::exception("There is no camera in the scene");
        cout << "Input scene: " << sceneFileName.toStdString() << endl;
        cout << "Output image: " << imageFileName.toStdString() << endl;
        QTime time;
        quint64 totalRays = rayTracer.options().totalRayLimit;
        cout << setprecision(3);
        rayTracer.setProgressCallback([totalRays, &time](float progress, bool, qint64 rays) {
            cout << "progress: "<< defaultfloat << progress*100 << "%, "
                 << scientific << static_cast<double>(rays) << " of " << static_cast<double>(totalRays) << " rays, "
                 << defaultfloat << time.elapsed() / 1000. << " s"
                 << endl;
        }, 10000, 1000000);
        time.start();
        rayTracer.run();
        cout << defaultfloat;
        cout << "Time elapsed (sec): " << time.elapsed() / 1000. << endl;
        cam->image().save(imageFileName);
        return 0;
    }
    catch(const std::exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QStringList args = a.arguments();
    switch (args.size())
    {
    case 2:
        w.openScene(args[1]);
        break;
    case 3:
        return runInBatchMode(args[1], args[2]);
    default:
        break;
    }

    w.show();
    return a.exec();
}
