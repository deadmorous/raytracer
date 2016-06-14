#include "image_processor_controller.h"

#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include "ray_tracer_controller.h"

namespace raytracer {

ClampImageAction::ClampImageAction(QObject *parent) :
    QWidgetAction(parent)
{
    QWidget *widget = new QWidget;
    auto layout = new QHBoxLayout(widget);
    layout->addWidget(m_check = new QCheckBox);
    auto label = new QLabel(tr("&Clamp color values"));
    layout->addWidget(label);
    layout->addWidget(m_clampValue = new QSpinBox);
    label->setBuddy(m_clampValue);
    m_clampValue->setValue(10);
    m_clampValue->setMinimum(1);
    m_clampValue->setMaximum(10000);
    setDefaultWidget(widget);
    setCheckable(true);
    connect(m_check, SIGNAL(toggled(bool)), SIGNAL(customToggled(bool)));
    connect(m_clampValue, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
}

int ClampImageAction::clampValue() const {
    return m_clampValue->value();
}

bool ClampImageAction::isCustomChecked() {
    return m_check->isChecked();
}



LogScaleImageImageAction::LogScaleImageImageAction(QObject *parent) :
    QWidgetAction(parent)
{
    QWidget *widget = new QWidget;
    auto layout = new QHBoxLayout(widget);
    layout->addWidget(m_check = new QCheckBox);
    auto label = new QLabel(tr("&Log scale values"));
    layout->addWidget(label);
    layout->addWidget(m_threshold = new QDoubleSpinBox);
    label->setBuddy(m_threshold);
    m_threshold->setMinimum(0);
    m_threshold->setMaximum(1);
    m_threshold->setSingleStep(0.01);
    m_threshold->setDecimals(4);
    m_threshold->setValue(0.1);
    setDefaultWidget(widget);
    setCheckable(true);
    connect(m_check, SIGNAL(toggled(bool)), SIGNAL(customToggled(bool)));
    connect(m_threshold, SIGNAL(valueChanged(double)), SIGNAL(valueChanged()));
}

float LogScaleImageImageAction::threshold() const
{
    return static_cast<float>(m_threshold->value());
}

bool LogScaleImageImageAction::isCustomChecked() {
    return m_check->isChecked();
}



ImageProcessorController::ImageProcessorController(QObject *parent) :
    QObject(parent),
    m_sceneImageProcessor(new IdentityImageProcessor),
    m_imageProcessor(m_sceneImageProcessor),
    m_menu(new QMenu(tr("&Image")))
{
    m_actionSceneImgProc = m_menu->addAction(tr("Use processor from scene"));
    m_actionSceneImgProc->setCheckable(true);
    m_actionSceneImgProc->setChecked(true);

    m_menu->addSeparator();

    m_actionClampImage = new ClampImageAction(this);
    m_menu->addAction(m_actionClampImage);

    m_actionAverage = m_menu->addAction(tr("&Apply 3x3 averaging"));
    m_actionAverage->setCheckable(true);

    m_actionLogScale = new LogScaleImageImageAction(this);
    m_menu->addAction(m_actionLogScale);

    connect(m_actionSceneImgProc, SIGNAL(toggled(bool)), SLOT(changeImageProcessor()));
    connect(m_actionClampImage, SIGNAL(customToggled(bool)), SLOT(changeImageProcessor()));
    connect(m_actionClampImage, SIGNAL(valueChanged()), SLOT(changeImageProcessor()));
    connect(m_actionAverage, SIGNAL(toggled(bool)), SLOT(changeImageProcessor()));
    connect(m_actionLogScale, SIGNAL(customToggled(bool)), SLOT(changeImageProcessor()));
    connect(m_actionLogScale, SIGNAL(valueChanged()), SLOT(changeImageProcessor()));
    updateActions();
}

ImageProcessorController::~ImageProcessorController() {
    // Note: Empty destructor is defined here because
    // the destructor of std::unique_ptr<QMenu> wants QMenu
    // to be complete.
}

QMenu *ImageProcessorController::menu() {
    return m_menu.get();
}

ImageProcessor::Ptr ImageProcessorController::imageProcessor() const {
    return m_imageProcessor;
}

void ImageProcessorController::setSceneImageProcessor(ImageProcessor::Ptr sceneImageProcessor)
{
    Q_ASSERT(sceneImageProcessor);
    m_sceneImageProcessor = sceneImageProcessor;
}

void ImageProcessorController::changeImageProcessor()
{
    updateActions();
    if (m_actionSceneImgProc->isChecked())
        m_imageProcessor = m_sceneImageProcessor;
    else
    {
        auto cip = std::make_shared<CombinedImageProcessor>();
        m_imageProcessor = cip;
        if (m_actionClampImage->isCustomChecked())
            *cip << std::make_shared<ClampImage>(static_cast<float>(m_actionClampImage->clampValue()));
        if (m_actionAverage->isChecked())
            *cip << std::make_shared<AverageImage>();
        if (m_actionLogScale->isCustomChecked())
            *cip << std::make_shared<LogScaleImage>(m_actionLogScale->threshold());
    }
    emit imageProcessorChanged(m_imageProcessor);
}

void ImageProcessorController::updateActions()
{
    bool sceneImageProc = m_actionSceneImgProc->isChecked();
    m_actionClampImage->setEnabled(!sceneImageProc);
    m_actionAverage->setEnabled(!sceneImageProc);
    m_actionLogScale->setEnabled(!sceneImageProc);
}

} // end namespace raytracer
