#pragma once

#include <QOpenGLWidget> // Base OpenGL widget
#include <QOpenGLFunctions> // OpenGL functions
#include <QOpenGLShaderProgram> // Shader program wrapper
#include <QOpenGLBuffer> // VBO
#include <QOpenGLVertexArrayObject> // VAO
#include <QMatrix4x4> // 4�4 matrices
#include <QPoint> // For mouse position
#include <vector>

#include "Shape.h"  

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    void drawShape(const std::vector<float>& vec, const std::vector<float>& normal = {});
    void clearShape(); 

protected:
    void initializeGL() override;     // Create VAO, VBO, shaders
    void resizeGL(int w, int h) override; // Update projection
    void paintGL() override;          // Render the shape

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::vector<float> mVertices;
    std::vector<float> mNormals;

    //Buffers
    QOpenGLVertexArrayObject mShapeVAO;
    QOpenGLBuffer mShapeVBO;
    //QOpenGLVertexArrayObject mNormalVAO;
    QOpenGLBuffer mNormalVBO;

    //Shader program
    QOpenGLShaderProgram mShader;

    // Mouse + rotation controls
    QPoint mLastMousePos;
    float mRotationX;
    float mRotationY;
    float mRotationZ;
    float mZoom;

    QMatrix4x4 mProjection;

    // Light + object color
    QVector3D mLightDir;
    QVector3D mLightColor;
    QVector3D mObjectColor;
    QVector3D mCameraPos;
};