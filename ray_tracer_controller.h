#ifndef RAY_TRACER_CONTROLLER_H
#define RAY_TRACER_CONTROLLER_H

#include <QObject>
#include <QThread>

namespace raytracer {

class RayTracer;

class RayTracerThread : public QThread
{
public:
    explicit RayTracerThread(RayTracer& rt);

protected:
    void run();

private:
    RayTracer& m_rt;
};



class RayTracerController : public QObject
{
    Q_OBJECT
public:
    explicit RayTracerController(RayTracer& rt, QObject *parent = nullptr);

    void start();
    void stop();
    bool isRunning() const;

signals:
    void rayTracerImageUpdated(const QPixmap& pixmap);
    void rayTracerProgress(float progress, quint64 raysProcessed);
    void rayTracerFinished();

private:
    RayTracer& m_rt;
    RayTracerThread m_rtThread;
};

} // end namespace raytracer

#endif // RAY_TRACER_CONTROLLER_H