#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include <QString>
#include "Point.h"
#include "Shape.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    std::vector<float> mVertices;

    float rotationX;
    float rotationY;
    float rotationZ;
    float zoom;
    QPoint lastMousePos;

public:
    explicit GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    void drawShape(std::vector<Point>& vec);
    void clearShape();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;


};

#endif // GLWIDGET_H