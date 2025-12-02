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
        R"(
            layout(location = 0) in vec3 position;
            void main()
            {
                gl_Position = vec4(position, 1.0);
            }
        )");

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(
            out vec4 color;
            void main()
            {
                color = vec4(0.0, 0.7, 1.0, 1.0);
            }
        )");
   /* shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_src);
    shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_src);*/

    shader.link();

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    shader.bind();
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));

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
