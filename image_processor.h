#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "camera.h"
#include "factory.h"
#include "serial.h"

namespace raytracer {

class ImageProcessor :
        public Readable,
        public FactoryMixin<ImageProcessor>
{
public:
    virtual Camera::Canvas operator()(const Camera::Canvas& canvas) const = 0;
};

class IdentityImageProcessor : public ImageProcessor
{
    DECL_GENERATOR(IdentityImageProcessor)
public:
    Camera::Canvas operator()(const Camera::Canvas& canvas) const {
        return canvas;
    }
    void read(const QVariant &) {}
};

class AverageImage : public ImageProcessor
{
    DECL_GENERATOR(AverageImage)
public:
    Camera::Canvas operator()(const Camera::Canvas& canvas) const
    {
        auto size = canvas.size();
        Camera::Canvas result(canvas.size());

        // Postprocess image
        v2i xy;
        for (xy[1]=1; xy[1]+1<size[1]; ++xy[1])
            for (xy[0]=1; xy[0]+1<size[0]; ++xy[0])
            {
                v3f& dst = result[xy];
                v2i dxy;
                for (dxy[1]=-1; dxy[1]<2; ++dxy[1])
                    for (dxy[0]=-1; dxy[0]<2; ++dxy[0])
                        dst += canvas[xy+dxy];
            }
        return result;
    }
    void read(const QVariant &) {}
};

class ClampImage : public ImageProcessor
{
    DECL_GENERATOR(ClampImage)
public:
    ClampImage() : m_clampValue(1.f) {}
    explicit ClampImage(float clampValue) : m_clampValue(clampValue) {}
    Camera::Canvas operator()(const Camera::Canvas& canvas) const
    {
        Camera::Canvas result = canvas;
        for (v3f& pixel : result)
            for (int i=0; i<3; ++i)
                if (pixel[i] > m_clampValue)
                    pixel[i] = m_clampValue;
        return result;
    }
    void read(const QVariant &v) {
        m_clampValue = fromVariant<float>(v);
    }

private:
    float m_clampValue;
};

class LogScaleImage : public ImageProcessor
{
    DECL_GENERATOR(LogScaleImage)
public:
    LogScaleImage() : m_threshold(0.1f) {}
    explicit LogScaleImage(float threshold) : m_threshold(threshold) {}
    Camera::Canvas operator()(const Camera::Canvas& canvas) const
    {
        if (m_threshold <= 0.f || m_threshold > 1.f)
            return canvas;
        float maxValue = 0.f;
        for (const v3f& pixel : canvas)
            for (int i=0; i<3; ++i)
                if (maxValue < pixel[i])
                    maxValue = pixel[i];
        Camera::Canvas result = canvas;
        for (v3f& pixel : result)
            for (int i=0; i<3; ++i) {
                float& v = pixel[i];
                v /= maxValue;
                if (v <= m_threshold)
                    v = 0.f;
                else
                    v = log(v/m_threshold);
            }
        return result;
    }
    void read(const QVariant &v) {
        m_threshold = fromVariant<float>(v);
    }

private:
    float m_threshold;
};

class CombinedImageProcessor : public ImageProcessor
{
    DECL_GENERATOR(CombinedImageProcessor)
public:
    CombinedImageProcessor& addImageProcessor(const ImageProcessor::Ptr proc) {
        m_imageProcessors.push_back(proc);
        return *this;
    }
    CombinedImageProcessor& operator<<(const ImageProcessor::Ptr proc) {
        return addImageProcessor(proc);
    }
    Camera::Canvas operator()(const Camera::Canvas& canvas) const {
        Camera::Canvas result = canvas;
        for (ImageProcessor::Ptr proc : m_imageProcessors)
            result = (*proc)(result);
        return result;
    }
    void read(const QVariant &v) {
        readTypedInstances(m_imageProcessors, v);
    }

private:
    std::vector<ImageProcessor::Ptr> m_imageProcessors;
};

} // end namespace raytracer

#endif // IMAGE_PROCESSOR_H
