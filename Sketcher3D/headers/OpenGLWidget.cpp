#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer),
    mTransform(Transformation::identity4())
{
}

OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Vertex + fragment shaders using a mat4 transform uniform
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(
            attribute vec3 position;
            uniform mat4 uTransform;
            void main()
            {
                gl_Position = uTransform * vec4(position, 1.0);
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

    shader.bind();
    uTransformLoc = shader.uniformLocation("uTransform");
    shader.release();

    // Triangle vertices in XY plane
    const float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, static_cast<int>(sizeof(vertices)));

    shader.bind();
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));
    shader.release();

    vao.release();
    vbo.release();

    resetTransform(); // upload identity once
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.bind();
    vao.bind();
    uploadTransform();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.release();
    shader.release();
}

void OpenGLWidget::uploadTransform() 
{
    float arr[16];
    Transformation::toArrayRowMajor(mTransform, arr);
    glUniformMatrix4fv(uTransformLoc, 1, GL_TRUE, arr);
}

void OpenGLWidget::resetTransform()
{
    mTransform = Transformation::identity4();
    update();
}

void OpenGLWidget::applyTranslation(float dx, float dy)
{
    mTransform = Transformation::multiply(mTransform, Transformation::translate3D(dx, dy, 0.f));
    update();
}

void OpenGLWidget::applyRotation(float degrees)
{
    mTransform = Transformation::multiply(mTransform, Transformation::rotateZ(degrees));
    update();
}

void OpenGLWidget::applyScale(float sx, float sy)
{
    mTransform = Transformation::multiply(mTransform, Transformation::scale3D(sx, sy, 1.f));
    update();
}

void OpenGLWidget::setTransform(const Transformation::Mat& m)
{
    mTransform = m;
    update();
}


void OpenGLWidget::applyRotateArbZ(float angleDeg, float px, float py, float pz)
{
    // 1. Build the transforms in DEGREES (your rotateZ accepts degrees)
    Transformation::Mat T1 = Transformation::translate3D(-px, -py, -pz);
    Transformation::Mat Rz = Transformation::rotateZ(angleDeg);
    Transformation::Mat T2 = Transformation::translate3D(px, py, pz);

    // 2. Combined local rotation around pivot
    Transformation::Mat local = Transformation::multiply(T2, Transformation::multiply(Rz, T1));

    // 3. Apply it AFTER the existing transform
    mTransform = Transformation::multiply(mTransform, local);

    update();
}