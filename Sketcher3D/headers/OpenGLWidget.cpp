#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Simple vertex + fragment shaders (hardcoded)
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(
        attribute vec3 position;
        void main()
        {
            gl_Position = vec4(position, 1.0);
        }
    )");

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(
        void main()
        {
            gl_FragColor = vec4(0.0, 0.7, 1.0, 1.0);
        }
    )");
    shader.bindAttributeLocation("position", 0);

    shader.link();

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();

    if (!mVertices.empty()) {
        vbo.allocate(mVertices.data(), mVertices.size() * sizeof(float));
    }

    shader.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    vao.release();
    vbo.release();
    shader.release();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mVertices.empty()) return;
    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.release();
    shader.release();
}

void OpenGLWidget::setVertices(const std::vector<Point>& points)
{
    mVertices.clear();
    mVertices.reserve(points.size() * 3);

    for (const auto& p : points)
    {
        mVertices.push_back(static_cast<float>(p.getX()));
        mVertices.push_back(static_cast<float>(p.getY()));
        mVertices.push_back(static_cast<float>(p.getZ()));
    }

    if (isValid())
    {
        makeCurrent();

        vao.bind();
        vbo.bind();
        vbo.allocate(mVertices.data(), mVertices.size() * sizeof(float));
        vbo.release();
        vao.release();

        update();
    }
}

std::vector<Point> OpenGLWidget::getVertices() const
{
    std::vector<Point> points;
    for (int i = 0; i < mVertices.size(); i += 3)
    {
        points.emplace_back(
            static_cast<double>(mVertices[i]),
            static_cast<double>(mVertices[i + 1]),
            static_cast<double>(mVertices[i + 2])
        );
    }
    return points;
}
