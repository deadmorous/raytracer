#ifndef RAY_TRACER_CONTROLLER_H
#define RAY_TRACER_CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "image_processor.h"

namespace raytracer {

class RayTracer;

class RayTracerThread : public QThread
{
public:
    explicit RayTracerThread(RayTracer& rt);
    QString error() const;

protected:
    void run();

private:
    RayTracer& m_rt;
    QString m_error;
};



class RayTracerController : public QObject
{
    Q_OBJECT
public:
    explicit RayTracerController(RayTracer& rt, QObject *parent = nullptr);

    void start();
    void stop();
    bool isRunning() const;

    void setImageProcessor(const ImageProcessor::Ptr& imageProcessor);
    ImageProcessor::Ptr imageProcessor() const;

    RayTracer& rayTracer() const;

signals:
    void rayTracerImageUpdated(const QPixmap& pixmap);
    void rayTracerProgress(float progress, quint64 raysProcessed);
    void rayTracerFinished(const QString& error);

private slots:
    void rayTracerThreadFinished();

private:
    mutable QMutex m_mutex;
    RayTracer& m_rt;
    RayTracerThread m_rtThread;
    ImageProcessor::Ptr m_imageProcessor;
};

} // end namespace raytracer

#endif // RAY_TRACER_CONTROLLER_H
