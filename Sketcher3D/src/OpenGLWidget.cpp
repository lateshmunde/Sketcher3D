#include "OpenGLWidget.h"
#include <QOpenGLShaderProgram>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget()
{
    vao.destroy();
    vbo.destroy();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // --- Vertex Data for a Triangle ---
    GLfloat vertices[] = {
        //  X      Y      Z
        0.0f,  0.6f, 0.0f,   // top
       -0.6f, -0.4f, 0.0f,   // left
        0.6f, -0.4f, 0.0f    // right
    };

    // --- Create VAO ---
    vao.create();
    vao.bind();

    // --- Create VBO ---
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    // --- Create Shader Program ---
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(#version 330 core
           layout(location = 0) in vec3 position;
           void main() {
               gl_Position = vec4(position, 1.0);
           })");

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(#version 330 core
           out vec4 fragColor;
           void main() {
               fragColor = vec4(1.0, 0.3, 0.2, 1.0);
           })");

    shader.link();
    shader.bind();

    // --- Set up vertex attribute pointers ---
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,              // attribute index
        3,              // number of components (x, y, z)
        GL_FLOAT,       // data type
        GL_FALSE,       // normalized
        0,              // stride
        nullptr         // offset
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