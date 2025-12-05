#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QPointF>
#include <QTimer>
#include <vector>

#include "Point.h"

class QMouseEvent;
class QWheelEvent;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    // Input / output of raw vertex positions (no normals required)
    void setVertices(const std::vector<Point>& points);
    std::vector<Point> getVertices() const;

    // Camera / light setters (optional to use)
    void setCameraPosition(const QVector3D& pos);
    void setCameraTarget(const QVector3D& target);
    void setLightPosition(const QVector3D& pos);
    void setObjectColor(const QVector3D& color);
    void setLightColor(const QVector3D& color);

    // Auto-rotation control
    void setAutoRotate(bool enabled);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Mouse interaction
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void updateViewMatrix();
    void updateModelMatrix();
    QVector3D mapToTrackball(const QPointF& p) const;

    QOpenGLShaderProgram shader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    // Interleaved: [Px, Py, Pz, Nx, Ny, Nz, ...]
    std::vector<float> mVertices;

    // Transformations
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_proj;

    // Camera + light
    QVector3D m_cameraPos;
    QVector3D m_cameraTarget;
    QVector3D m_lightPos;
    QVector3D m_objectColor;
    QVector3D m_lightColor;

    // Interaction state
    QPointF m_lastMousePos;
    bool m_leftButtonPressed = false;
    bool m_middleButtonPressed = false;
    bool m_rightButtonPressed = false;

    QQuaternion m_rotation;   // trackball rotation
    float m_zoom;             // uniform scale
    QVector3D m_pan;          // translation in x/y plane

    // Auto rotation
    bool m_autoRotate;
    QTimer* m_autoRotateTimer;
};
