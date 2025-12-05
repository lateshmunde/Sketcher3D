#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <vector>
#include "Transformation.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

public slots:
    void resetTransform();
    void applyTranslation(float dx, float dy);        // dz=0 (2D UI)
    void applyRotation(float degrees);                // rotate about Z
    void applyScale(float sx, float sy);              // sz=1
    void setTransform(const Transformation::Mat& m);

    void applyRotateArbZ(float degrees, float px, float py, float pz);         // rotate about Z in arbitrary order

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void uploadTransform();

    QOpenGLShaderProgram shader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    Transformation::Mat mTransform; // 4x4 homogeneous
    GLint uTransformLoc = -1;
};
