#include "stdafx.h"
#include "OpenGLWidget.h"

#include "Shape.h"
#include "Point.h"

#include <QVector4D>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Vertex + fragment shaders (very simple)
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(
            attribute vec3 position;
            uniform mat4 uProjection;

            void main()
            {
                gl_Position = uProjection * vec4(position, 1.0);
                gl_PointSize = 5.0;      // <- set point size in shader, no glPointSize() call
            }
        )");

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(
            void main()
            {
                gl_FragColor = vec4(0.0, 0.7, 1.0, 1.0);
            }
        )");

    shader.link();

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(nullptr, 0);   // no data yet

    shader.bind();
    shader.enableAttributeArray("position");
    shader.setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(float));

    shader.release();
    vbo.release();
    vao.release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    mProjection.setToIdentity();

    const float aspect = (h == 0) ? 1.0f : static_cast<float>(w) / static_cast<float>(h);
    const float fov = 45.0f;
    const float nearPlane = 0.1f;
    const float farPlane = 1000.0f;

    mProjection.perspective(fov, aspect, nearPlane, farPlane);

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -150.0f); // move camera back
    mProjection = mProjection * view;
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mVertexCount == 0)
        return;

    shader.bind();
    vao.bind();

    shader.setUniformValue("uProjection", mProjection);

    glDrawArrays(GL_POINTS, 0, mVertexCount);

    vao.release();
    shader.release();
}

void OpenGLWidget::setScene(const std::vector<std::shared_ptr<Shape>>& shapes,
    const std::vector<QMatrix4x4>& transforms)
{
    // Expect one transform per shape
    if (shapes.size() != transforms.size())
        return;

    mShapes = shapes;
    mTransforms = transforms;

    rebuildVertexBuffer();
    update();   // trigger repaint
}

void OpenGLWidget::rebuildVertexBuffer()
{
    mVertices.clear();

    // Build transformed vertices (apply each matrix to the shape's points)
    for (size_t i = 0; i < mShapes.size(); ++i)
    {
        const auto& shape = mShapes[i];
        if (!shape) continue;

        const auto coords = shape->getCoordinates();
        const QMatrix4x4& M = mTransforms[i];

        for (const Point& p : coords)
        {
            // Adjust these if your Point API is different
            QVector4D v(static_cast<float>(p.getX()),
                static_cast<float>(p.getY()),
                static_cast<float>(p.getZ()),
                1.0f);
            QVector4D vt = M * v;

            // scale down a bit so big shapes fit the view
            const float s = 0.1f;
            mVertices.push_back(vt.x() * s);
            mVertices.push_back(vt.y() * s);
            mVertices.push_back(vt.z() * s);
        }
    }

    mVertexCount = static_cast<int>(mVertices.size() / 3);

    if (!vbo.isCreated() || !vao.isCreated())
        return;

    makeCurrent();

    vao.bind();
    vbo.bind();

    if (!mVertices.empty())
        vbo.allocate(mVertices.data(), static_cast<int>(mVertices.size() * sizeof(float)));
    else
        vbo.allocate(nullptr, 0);

    shader.bind();
    shader.enableAttributeArray("position");
    shader.setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(float));
    shader.release();

    vbo.release();
    vao.release();

    doneCurrent();
}
