#include "GLWidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>

// OpenGL headers - order matters!
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif
#include <GL/gl.h>
#include <GL/glu.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , rotationX(30.0f)
    , rotationY(45.0f)
    , zoom(5.0f)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::loadDATFile(const QString& filename)
{
    vertices.clear();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file:" << filename;
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        // Split by whitespace
        QStringList coords = line.split(" ", Qt::SkipEmptyParts);

        if (coords.size() >= 3) {
            bool okX, okY, okZ;
            float x = coords[0].toFloat(&okX);
            float y = coords[1].toFloat(&okY);
            float z = coords[2].toFloat(&okZ);

            if (okX && okY && okZ) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
    }

    file.close();

    qDebug() << "Loaded" << vertices.size() / 3 << "points from" << filename;

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

    // Set background color (dark gray)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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

    // Draw coordinate axes
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);

    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);

    // Z-axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();

    // Draw the shape
    if (!vertices.empty()) {
        // Draw points
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(6.0f);

        glBegin(GL_POINTS);
        for (size_t i = 0; i < vertices.size(); i += 3) {
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