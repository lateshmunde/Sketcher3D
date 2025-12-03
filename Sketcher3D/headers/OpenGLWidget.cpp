#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Simple vertex + fragment shaders (hardcoded)
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        "(\
        attribute vec3 position;\
        void main()\
        {\
            gl_Position = vec4(position, 1.0);\
        }\
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
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

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

    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.release();
    shader.release();
}

void OpenGLWidget::setVertices(const std::vector<Point>& points)
{
    // convert DOUBLE -> FLOAT only for OpenGL
    std::vector<float> data;
    data.reserve(points.size() * 3);

    for (const auto& p : points)
    {
        data.push_back(static_cast<float>(p.getX()));
        data.push_back(static_cast<float>(p.getY()));
        data.push_back(static_cast<float>(p.getZ()));
    }

    vao.bind();
    vbo.bind();
    vbo.allocate(data.data(), data.size() * sizeof(float));
    vbo.release();
    vao.release();

    update();
}
