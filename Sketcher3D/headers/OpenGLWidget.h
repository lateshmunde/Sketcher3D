//#ifndef GLWIDGET_H
//#define GLWIDGET_H
//
//#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
//#include <QOpenGLShaderProgram>
//#include <QOpenGLBuffer>
//#include <QOpenGLVertexArrayObject>
//#include <QMouseEvent>
//#include <QWheelEvent>
//#include <vector>
//#include <QMatrix4x4>
//#include <QDebug>
//#include <QString>
//#include "Point.h"
//#include "Shape.h"
//
//class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//    Q_OBJECT
//
//private:
//    QOpenGLShaderProgram shader;
//    QOpenGLBuffer vbo;
//    QOpenGLVertexArrayObject vao;
//
//    std::vector<float> mVertices;
//
//    QMatrix4x4 projection;
//    float rotationX;
//    float rotationY;
//    float rotationZ;
//    float zoom;
//    QPoint lastMousePos;
//    int mvpLoc;
//
//public:
//    explicit OpenGLWidget(QWidget* parent = nullptr);
//    ~OpenGLWidget();
//
//    void drawShape(std::vector<Point>& vec);
//    void clearShape();
//
//protected:
//    void initializeGL() override;
//    void resizeGL(int w, int h) override;
//    void paintGL() override;
//
//    void mousePressEvent(QMouseEvent* event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void wheelEvent(QWheelEvent* event) override;
//
//
//};
//
//#endif // GLWIDGET_H



//#pragma once
//
//// Core Qt + OpenGL includes
//#include <QOpenGLWidget>             // Base class for OpenGL widget
//#include <QOpenGLFunctions>          // Gives access to OpenGL functions (glDrawArrays, glEnable, etc.)
//#include <QOpenGLShaderProgram>      // Wrapper for vertex + fragment shaders
//#include <QOpenGLBuffer>             // Wrapper for VBO (Vertex Buffer Object)
//#include <QOpenGLVertexArrayObject>  // Wrapper for VAO (Vertex Array Object)
//#include <QMatrix4x4>                // 4x4 matrix (model, view, projection)
//#include <QPoint>                    // For storing mouse position
//#include <vector>
//
//#include "Point.h"                   // Your Point class with getX(), getY(), getZ()
//
//class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//    Q_OBJECT
//
//public:
//    explicit OpenGLWidget(QWidget* parent = nullptr);   // Constructor
//    ~OpenGLWidget();                                    // Destructor
//
//    // Called by your app to update geometry to draw (expects triangles)
//    void drawShape(std::vector<Point>& vec);
//
//    // Clears geometry
//    void clearShape();
//
//protected:
//    // OpenGL lifecycle functions from QOpenGLWidget
//    void initializeGL() override;         // Called once: create shaders, buffers, etc.
//    void resizeGL(int w, int h) override; // Called on window resize: update projection
//    void paintGL() override;              // Called every time widget must be redrawn
//
//    // Input events to control camera
//    void mousePressEvent(QMouseEvent* event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void wheelEvent(QWheelEvent* event) override;
//
//private:
//    // ---------- Rendering objects ----------
//    QOpenGLShaderProgram shader;       // Single shader program for shapes + axes
//
//    QOpenGLBuffer vbo;                 // VBO for shape vertices
//    QOpenGLVertexArrayObject vao;      // VAO for shape vertex layout
//
//    QOpenGLBuffer axisVBO;             // VBO for axis vertices
//    QOpenGLVertexArrayObject axisVAO;  // VAO for axis vertex layout
//
//    // CPU-side storage of shape vertices: [x0,y0,z0, x1,y1,z1, ...]
//    std::vector<float> mVertices;
//
//    // ---------- Matrices ----------
//    QMatrix4x4 projection;             // Projection matrix (perspective)
//
//    // ---------- Camera parameters ----------
//    float camDistance;                 // Distance of camera from origin
//    float camYaw;                      // Rotation around Y-axis (in degrees)
//    float camPitch;                    // Rotation up/down (in degrees)
//
//    QPoint lastMousePos;               // Last mouse position (for drag delta)
//
//    // ---------- Lighting parameters ----------
//    QVector3D lightPos;                // Position of light in world space
//    QVector3D lightColor;              // Color of light (affects intensity)
//    QVector3D objectColor;             // Base color of shape
//};


#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>
#include <vector>

#include "Point.h"  // Your own Point class with getX(), getY(), getZ()

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    // Provide geometry (as triangles) to be drawn
    void drawShape(std::vector<Point>& vec);

    // Clear geometry
    void clearShape();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // Shader program (vertex + fragment)
    QOpenGLShaderProgram m_program;

    // Shape geometry
    QOpenGLVertexArrayObject m_shapeVAO;
    QOpenGLBuffer            m_shapeVBO;
    std::vector<float>       m_vertices;   // x,y,z,x,y,z,...

    // Axes geometry
    QOpenGLVertexArrayObject m_axesVAO;
    QOpenGLBuffer            m_axesVBO;

    // Matrices
    QMatrix4x4 m_projection;

    // Camera (orbit)
    float   m_camDistance;   // radius
    float   m_camYaw;        // degrees
    float   m_camPitch;      // degrees
    QPoint  m_lastMousePos;

    // Lighting
    QVector3D m_lightPos;
    QVector3D m_lightColor;
    QVector3D m_shapeColor;
};
