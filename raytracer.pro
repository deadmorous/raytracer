#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T17:35:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raytracer
TEMPLATE = app
CONFIG += c++14

# TODO: Fix the code and get this warning back
gcc:QMAKE_CXXFLAGS += -Wno-deprecated-declarations

gcc:QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

SOURCES += main.cpp\
        mainwindow.cpp \
    primitive_search.cpp \
    ray_tracer.cpp \
    scene.cpp \
    camera.cpp \
    primitives/sphere.cpp \
    primitive.cpp \
    json_file_reader.cpp \
    json_parser.cpp \
    primitives/rectangle.cpp \
    transform.cpp \
    simple_camera.cpp

HEADERS  += mainwindow.h \
    compile_assert.h \
    fsmx.h \
    cxx_exception.h \
    m_const.h \
    surf_mesh_common.h \
    surf_mesh_extruded.h \
    surf_mesh_revolved.h \
    primitive.h \
    ray.h \
    bounding_sphere.h \
    common.h \
    primitive_search.h \
    surface_point.h \
    surface_properties.h \
    ray_tracer.h \
    scene.h \
    camera.h \
    light_source.h \
    primitives/sphere.h \
    serial.h \
    factory.h \
    json_file_reader.h \
    json_parser.h \
    primitives/rectangle.h \
    transform.h \
    simple_camera.h

FORMS    += mainwindow.ui
