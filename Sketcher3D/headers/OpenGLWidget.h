#pragma once

#include <QOpenGLWidget>              // Base OpenGL widget
#include <QOpenGLFunctions>           // OpenGL functions
#include <QOpenGLShaderProgram>       // Shader program wrapper
#include <QOpenGLBuffer>              // VBO
#include <QOpenGLVertexArrayObject>   // VAO
#include <QMatrix4x4>                 // 4×4 matrices
#include <QPoint>                     // For mouse position
#include <vector>

#include "Point.h"                    // Your Point class

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    // Called to draw a new shape (triangle vertices)
    void drawShape(std::vector<Point>& vec);

    // Clears the shape
    void clearShape();

protected:
    void initializeGL() override;     // Create VAO, VBO, shaders
    void resizeGL(int w, int h) override; // Update projection
    void paintGL() override;          // Render the shape

    // Camera controls (rotate + zoom)
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // GPU objects for shape
    QOpenGLVertexArrayObject mShapeVAO;
    QOpenGLBuffer            mShapeVBO;

    // CPU-side vertex storage
    std::vector<float> mVertices;

    // Shader program
    QOpenGLShaderProgram mShader;

    // Matrices
    QMatrix4x4 mProjection;

    // Mouse + rotation controls
    QPoint mLastMousePos;
    float mRotationX;
    float mRotationY;
    float mZoom;

    // Light + object color
    QVector3D mLightDir;
    QVector3D mObjectColor;
};
