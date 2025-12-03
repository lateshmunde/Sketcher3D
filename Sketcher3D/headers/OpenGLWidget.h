#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <vector>
#include <memory>

class Shape; // forward declaration (comes from DLL)

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    // Called by Sketcher3D whenever shapes or transforms change
    void setScene(const std::vector<std::shared_ptr<Shape>>& shapes,
        const std::vector<QMatrix4x4>& transforms);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void rebuildVertexBuffer();   // apply transforms and upload vertices to GPU

private:
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    std::vector<std::shared_ptr<Shape>> mShapes;
    std::vector<QMatrix4x4>             mTransforms;

    std::vector<float> mVertices;  // x, y, z triples
    int                mVertexCount = 0;

    QMatrix4x4 mProjection;
};
