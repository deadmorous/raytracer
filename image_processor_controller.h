#ifndef IMAGEPROCESSORCONTROLLER_H
#define IMAGEPROCESSORCONTROLLER_H

#include <QObject>
#include <QWidgetAction>

#include "image_processor.h"

class QMenu;
class QAction;
class QCheckBox;
class QSpinBox;
class QDoubleSpinBox;

namespace raytracer {

class RayTracer;



class ClampImageAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit ClampImageAction(QObject *parent = nullptr);
    int clampValue() const;
    bool isCustomChecked();

signals:
    void valueChanged();
    bool customToggled(bool);

private:
    QCheckBox *m_check;
    QSpinBox *m_clampValue;
};



class LogScaleImageImageAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit LogScaleImageImageAction(QObject *parent = nullptr);
    float threshold() const;
    bool isCustomChecked();

signals:
    void valueChanged();
    bool customToggled(bool);

private:
    QCheckBox *m_check;
    QDoubleSpinBox *m_threshold;
};



class ImageProcessorController : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessorController(QObject *parent = 0);
    QMenu *menu();
    ImageProcessor::Ptr imageProcessor() const;

signals:
    void imageProcessorChanged(raytracer::ImageProcessor::Ptr imgProc);

public slots:
    void setSceneImageProcessor(raytracer::ImageProcessor::Ptr sceneImageProcessor);

private slots:
    void changeImageProcessor();

private:
    ImageProcessor::Ptr m_sceneImageProcessor;
    ImageProcessor::Ptr m_imageProcessor;
    std::unique_ptr<QMenu> m_menu;
    QAction *m_actionSceneImgProc;
    QAction *m_actionAverage;
    ClampImageAction *m_actionClampImage;
    LogScaleImageImageAction *m_actionLogScale;

    void updateActions();
};

} // end namespace raytracer

#endif // IMAGEPROCESSORCONTROLLER_H
