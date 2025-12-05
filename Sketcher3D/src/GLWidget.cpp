#include "GLWidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>


GLWidget::GLWidget(QWidget* parent): QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer)
    , rotationX(30.0f), rotationY(45.0f), rotationZ(30.0f), zoom(50.0f){}
//Initialize rotation around X, Y, Z axis(30, 45, 30 deg resp.)
//Initial zoom value = 50 (camera is 50 units away along - Z)

GLWidget::~GLWidget()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    doneCurrent();
}

void GLWidget::drawShape(std::vector<Point>& vec) // from pts
{
    mVertices.clear();
    mVertices.reserve(vec.size() * 3);

    std::vector <Point> pts = vec;

    for (const Point& pt : pts)
    {
        mVertices.push_back(static_cast<float>(pt.getX()));
        mVertices.push_back(static_cast<float>(pt.getY()));
        mVertices.push_back(static_cast<float>(pt.getZ()));
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

void GLWidget::clearShape()
{
    mVertices.clear();
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Set background color (white)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);

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




    // Set point size
    glPointSize(5.0f);

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void GLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Manual perspective calculation (avoiding gluPerspective)
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    GLfloat fH = tan(45.0f / 360.0f * 3.14159f) * 0.1f;
    GLfloat fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera positioning
    glTranslatef(0.0f, 0.0f, -zoom);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

    // Draw coordinate axes
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    
    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    
    // Z-axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();

    if (mVertices.empty()) return;
    shader.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, (mVertices.size() / 3));
    vao.release();
    shader.release();

    // Draw the shape
    //if (!mVertices.empty()) 
    //{
    //    // Draw points
    //    glColor3f(1.0f, 1.0f, 1.0f);
    //    glPointSize(6.0f);

    //    glBegin(GL_POINTS);
    //    for (int i = 0; i < mVertices.size(); i += 3) {
    //        glVertex3f(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
    //    }
    //    glEnd();
    //    
    //    // Draw connecting lines
    //    glColor3f(0.5f, 0.8f, 1.0f);
    //    glLineWidth(2.0f);
    //    glBegin(GL_LINE_LOOP);
    //    for (size_t i = 0; i < mVertices.size(); i += 3) {
    //        glVertex3f(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
    //    }
    //    glEnd();
    //}
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    //int dx = event->x() - lastMousePos.x();
    int dx = static_cast<int>(event->position().x()) - lastMousePos.x();
    //int dy = event->y() - lastMousePos.y();
    int dy = static_cast<int>(event->position().y()) - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;
        update();
    } 

    lastMousePos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
        zoom -= 0.5f;
    else
        zoom += 0.5f;

    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 20.0f) zoom = 20.0f;

    update();
}