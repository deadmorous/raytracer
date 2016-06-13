/// \file
/// \brief Declaration of the Camera class.

#ifndef CAMERA_H
#define CAMERA_H

#include "primitive.h"
#include "ray.h"

class QImage;

namespace raytracer {

/// \brief Interface for a camera.
class Camera :
        public Readable,
        public FactoryMixin<Camera>
{
public:
    class Canvas
    {
    public:
        Canvas() : m_size(fsmx::zero<v2i>()) {}
        Canvas(const v2i& size) : m_size(size), m_data(size[0]*size[1], fsmx::zero<v3f>()) {}
        const v2i& size() const { return m_size; }
        std::vector< v3f >::size_type length() const { return m_data.size(); }
        bool empty() const { return m_data.empty(); }
        bool contains(const v2i& xy) const {
            return xy[0] >= 0   &&   xy[0] < m_size[0]   &&   xy[1] >= 0   &&   xy[1] < m_size[1];
        }
        int index(const v2i& xy) const {
            return xy[0] + xy[1]*m_size[0];
        }
        v2i xy(int index) const { return mkv2i(index%m_size[0], index/m_size[0]); }
        v3f& pixel(const v2i& xy) {
            Q_ASSERT(contains(xy));
            return m_data[index(xy)];
        }
        const v3f& pixel(const v2i& xy) const {
            return const_cast<Canvas*>(this)->pixel(xy);
        }
        v3f& pixel(int index) {
            Q_ASSERT(index >= 0   &&   index < static_cast<int>(m_data.size()));
            return m_data[index];
        }
        const v3f& pixel(int index) const {
            return const_cast<Canvas*>(this)->pixel(index);
        }
        v3f& operator[](const v2i& xy) { return pixel(xy); }
        const v3f& operator[](const v2i& xy) const { return pixel(xy); }
        v3f& operator[](int index) { return pixel(index); }
        const v3f& operator[](int index) const { return pixel(index); }
        std::vector< v3f >& data() { return m_data; }
        const std::vector< v3f >& data() const { return m_data; }
        std::vector< v3f >::iterator begin() { return m_data.begin(); }
        std::vector< v3f >::iterator end() { return m_data.end(); }
        std::vector< v3f >::const_iterator begin() const { return m_data.begin(); }
        std::vector< v3f >::const_iterator end() const { return m_data.end(); }
        QImage toImage() const;
    private:
        v2i m_size;
        std::vector< v3f > m_data;
    };

    struct RayData {
        Ray ray;
        v3f collisionPos;
        RayData() {}
        RayData(const Ray& ray, const v3f collisionPos) :
            ray(ray), collisionPos(collisionPos)
        {}
        const char *rawData() const {
            return reinterpret_cast<const char*>(this);
        }
    };

    /// \brief Default constructor.
    Camera();

    /// \brief Clears canvas and re-creates primitive.
    ///
    /// \note Must be called after setTransform() and probably
    /// other methods of a derived class.
    virtual void clear() = 0;

    /// \brief Returns primitive that represents
    /// camera screen.
    virtual Primitive::Ptr cameraPrimitive() const = 0;

    /// \brief Returns camera canvas.
    virtual const Canvas& canvas() const = 0;

    /// \brief Returns primitive transformation matrix.
    const m4f& transform() const;

    /// \brief Sets primitive transformation matrix.
    void setTransform(const m4f &transform);

    /// \brief Reads camera transformation.
    void read(const QVariant& v);

    /// \brief Reads RayData from a file and processes the corresponding rays.
    void readRays(const QString& fileName);
private:
    m4f m_transform;
};

} // end namespace raytracer

#endif // CAMERA_H
