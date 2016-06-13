/// \file
/// \brief Implementation of the Camera class.

#include "camera.h"
#include "transform.h"
#include "cxx_exception.h"

#include <QFile>
#include <QFileInfo>
#include <QImage>

namespace raytracer {

Camera::Camera() :
    m_transform(fsmx::identity<m4f>())
{
}

const m4f &Camera::transform() const
{
    return m_transform;
}

void Camera::setTransform(const m4f& transform)
{
    m_transform = transform;
}

void Camera::read(const QVariant& v)
{
    m_transform = fsmx::identity<m4f>();
    Transform::Ptr t;
    if (readOptionalTypedProperty(t, v, "transform"))
        (*t)(m_transform);
}

void Camera::readRays(const QString& fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        QFileInfo fi(fileName);
        throw cxx::exception(std::string("Failed to open rays input '") + fi.absoluteFilePath().toStdString() + "'");
    }
    SurfaceProperties *surfProp = cameraPrimitive()->surfaceProperties().get();
    Q_ASSERT(surfProp);
    SurfacePoint sp = fsmx::zero<SurfacePoint>();
    RayTracer *rt = nullptr;
    const quint64 RayDataSize = sizeof(RayData);
    forever {
        RayData rd;
        if (f.read(reinterpret_cast<char*>(&rd), RayDataSize) != RayDataSize)
            break;
        sppos(sp) = rd.collisionPos;
        surfProp->processCollision(rd.ray, sp, *rt);
    }
}

QImage Camera::Canvas::toImage() const
{
    QImage image(m_size[0], m_size[1], QImage::Format_RGB32);
    if (empty())
        return image;

    // Determine canvas color range
    auto minIntensity = m_data[0][0];
    auto maxIntensity = minIntensity;
    std::for_each(begin(), end(), [&minIntensity, &maxIntensity](const v3f& color) {
        for (int i=0; i< 3; ++i) {
            auto intensity = color[i];
            if (minIntensity > intensity)
                minIntensity = intensity;
            else if (maxIntensity < intensity)
                maxIntensity = intensity;
        }
    });

    // In case of monotonous color, return all-black or all-gray image
    if (minIntensity == maxIntensity) {
        image.fill(minIntensity == 0.f ?   0xff000000u :   0xff888888u);
        return image;
    }

    // Copy canvas pixels to the image
    uchar *bits = image.bits();
    std::for_each(begin(), end(), [&minIntensity, &maxIntensity, &bits](const v3f& color) {
        bits[3] = 0xff;
        for (int i=0; i<3; ++i) {
            auto normalizedIntensity = (color[i] - minIntensity) / (maxIntensity - minIntensity);
            bits[2-i] = static_cast<uchar>(normalizedIntensity * 255.999);
        }
        bits += 4;
    });
    return image;
}

} // end namespace raytracer
