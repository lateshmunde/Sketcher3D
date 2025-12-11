//#pragma once
//
//#include <QOpenGLWidget> // Base OpenGL widget
//#include <QOpenGLFunctions> // OpenGL functions
//#include <QOpenGLShaderProgram> // Shader program wrapper
//#include <QOpenGLBuffer> // VBO
//#include <QOpenGLVertexArrayObject> // VAO
//#include <QMatrix4x4> // 4�4 matrices
//#include <QPoint> // For mouse position
//#include <vector>
//
//#include "Shape.h"  
//
//class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//    Q_OBJECT
//
//public:
//    explicit OpenGLWidget(QWidget* parent = nullptr);
//    ~OpenGLWidget();
//
//    void drawShape(const std::vector<float>& vec, const std::vector<float>& normal = {});
//    void clearShape(); 
//    void startCuboidTool();
//
//protected:
//    void initializeGL() override;     // Create VAO, VBO, shaders
//    void resizeGL(int w, int h) override; // Update projection
//    void paintGL() override;          // Render the shape
//
//    void mousePressEvent(QMouseEvent* event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void wheelEvent(QWheelEvent* event) override;
//
//private:
//    std::vector<float> mVertices;
//    std::vector<float> mNormals;
//
//    //Buffers
//    QOpenGLVertexArrayObject mShapeVAO;
//    QOpenGLBuffer mShapeVBO;
//    //QOpenGLVertexArrayObject mNormalVAO;
//    QOpenGLBuffer mNormalVBO;
//
//    //Shader program
//    QOpenGLShaderProgram mShader;
//
//    // Mouse + rotation controls
//    QPoint mLastMousePos;
//    float mRotationX;
//    float mRotationY;
//    float mRotationZ;
//    float mZoom;
//
//    QMatrix4x4 mProjection;
//
//    // Light + object color
//    QVector3D mLightDir;
//    QVector3D mLightColor;
//    QVector3D mObjectColor;
//    QVector3D mCameraPos;
//
//    //cuboid tool
//    bool firstPointSelected = false;
//    bool drawingRectangle = false;
//    bool extruding = false;
//
//};
//



#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QMouseEvent>
#include <vector>

struct Cuboid1 {
    QVector3D minCorner; // base min (x,z) on ground, y is 0
    QVector3D maxCorner; // base max (x,z) on ground, y is 0
    float height;
};

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget() override;

    // call when user clicks "Cuboid" tool
    void startCuboidTool();

    void drawShape(const std::vector<float>& vec, const std::vector<float>& normal = {});
    void clearShape();

protected:
    // GL lifecycle
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Input
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // helpers
    QVector3D unprojectScreenToGround(const QPoint& screenPos); // intersects ray with y=0 plane
    void updatePreviewVertices();
    void finalizeCuboid();

    // GL objects
    QOpenGLShaderProgram* m_prog = nullptr;
    QOpenGLBuffer m_vbo; // dynamic VBO for wireframe lines
    QOpenGLVertexArrayObject m_vao;

    int m_viewportW = 1, m_viewportH = 1;

    // camera / view
    float m_zoom = 200.0f;
    float m_rotX = 20.0f; // degrees
    float m_rotY = -20.0f;
    float m_panX = 0.0f;
    float m_panY = 0.0f;

    // scene
    std::vector<Cuboid1> m_cuboids;
    std::vector<float> mVertices;
    std::vector<float> mNormals;

    // boolean flags for state machine (your chosen OPTION 3)
    bool firstPointSelected = false;
    bool drawingRectangle = false;
    bool extruding = false;

    // creation state
    QVector3D firstCornerWorld;   // copy of base first corner (y=0)
    QVector3D secondCornerWorld;  // changes while drawing rectangle (y=0)
    float startExtrudeScreenY = 0.0f;
    float currentExtrudeHeight = 0.0f;

    // preview geometry (line segments)
    std::vector<float> m_lineVertices; // x,y,z triples (GL_LINES)
};
