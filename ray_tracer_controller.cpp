#include "ray_tracer_controller.h"
#include "ray_tracer.h"
#include "cxx_exception.h"

#include <QPixmap>

namespace raytracer {

RayTracerThread::RayTracerThread(RayTracer& rt) :
    m_rt(rt)
{
}

void RayTracerThread::run()
{
    m_rt.run();
}



RayTracerController::RayTracerController(RayTracer &rt, QObject *parent) :
    QObject(parent),
    m_rt(rt),
    m_rtThread(rt),
    m_imageProcessor(rt.imageProcessor())
{
    connect(&m_rtThread, SIGNAL(finished()), SIGNAL(rayTracerFinished()));
}

void RayTracerController::start()
{
    if (isRunning())
        return;
    Camera::Ptr camera = m_rt.camera();
    if (!camera)
        throw cxx::exception("There is no camera in the raytracer scene");
    m_rt.setProgressCallback([camera, this](float progress, bool, quint64 raysProcessed) {
        emit rayTracerImageUpdated(QPixmap::fromImage((*m_imageProcessor)(camera->canvas()).toImage()));
        emit rayTracerProgress(progress, raysProcessed);
    });
    m_rtThread.start();
}

void RayTracerController::stop()
{
    if (!isRunning())
        return;
    m_rt.requestTermination();
    m_rtThread.wait();
}

bool RayTracerController::isRunning() const
{
    return m_rtThread.isRunning();
}

void RayTracerController::setImageProcessor(const ImageProcessor::Ptr& imageProcessor)
{
    m_imageProcessor = imageProcessor;
}

ImageProcessor::Ptr RayTracerController::imageProcessor() const
{
    return m_imageProcessor;
}

} // end namespace raytracer
