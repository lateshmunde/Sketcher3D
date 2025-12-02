//#ifndef GLWIDGET_H
//#define GLWIDGET_H
//
//#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
//#include <QMouseEvent>
//#include <QWheelEvent>
//#include <vector>
//#include <QString>
//
//class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
//{
//    Q_OBJECT
//
//public:
//    GLWidget(QWidget* parent = nullptr);
//    ~GLWidget();
//
//    void loadDATFile(const QString& filename);
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
//private:
//    std::vector<float> vertices;
//
//    float rotationX;
//    float rotationY;
//    float zoom;
//    QPoint lastMousePos;
//};
//
//#endif // GLWIDGET_H



#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include <QString>

struct Triangle {
    float v1[3];  // vertex 1: x, y, z
    float v2[3];  // vertex 2: x, y, z
    float v3[3];  // vertex 3: x, y, z
    float normal[3];  // normal vector for lighting
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    void loadDATFile(const QString& filename);
    void clearShape();
    void setWireframeMode(bool wireframe);
    void setShowAxes(bool show);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void calculateNormal(const float* v1, const float* v2, const float* v3, float* normal);
    void generateTrianglesFromPoints();

    std::vector<float> vertices;
    std::vector<Triangle> triangles;

    float rotationX;
    float rotationY;
    float zoom;
    QPoint lastMousePos;

    bool wireframeMode;
    bool showAxes;
};

#endif // GLWIDGET_H