#include "GLWidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>


GLWidget::GLWidget(QWidget* parent): QOpenGLWidget(parent)
    , rotationX(30.0f), rotationY(45.0f), rotationZ(30.0f), zoom(50.0f){}
//Initialize rotation around X, Y, Z axis(30, 45, 30 deg resp.)
//Initial zoom value = 50 (camera is 50 units away along - Z)

GLWidget::~GLWidget(){}

void GLWidget::drawShape(std::vector<Point>& vec) // from pts
{
    vertices.clear();

    std::vector <Point> pts = vec;

    for (const Point& pt : pts)
    {
        float x = pt.getX();
        float y = pt.getY();
        float z = pt.getZ();
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    update();
}

void GLWidget::clearShape()
{
    vertices.clear();
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Set background color (white)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);

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

    // Draw the shape
    if (!vertices.empty()) {
        // Draw points
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(6.0f);

        glBegin(GL_POINTS);
        for (int i = 0; i < vertices.size(); i += 3) {
            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        glEnd();
        
        // Draw connecting lines
        glColor3f(0.5f, 0.8f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        for (size_t i = 0; i < vertices.size(); i += 3) {
            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        glEnd();
    }
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