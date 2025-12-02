//#include "GLWidget.h"
//#include <QFile>
//#include <QTextStream>
//#include <QDebug>
//#include <QtMath>
//
//// OpenGL headers - order matters!
//#ifdef _WIN32
//#include <windows.h>
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")
//#endif
//#include <GL/gl.h>
//#include <GL/glu.h>
//
//GLWidget::GLWidget(QWidget* parent)
//    : QOpenGLWidget(parent)
//    , rotationX(30.0f)
//    , rotationY(45.0f)
//    , zoom(5.0f)
//{
//}
//
//GLWidget::~GLWidget()
//{
//}
//
//void GLWidget::loadDATFile(const QString& filename)
//{
//    vertices.clear();
//
//    QFile file(filename);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "Cannot open file:" << filename;
//        return;
//    }
//
//    QTextStream in(&file);
//
//    while (!in.atEnd()) {
//        QString line = in.readLine().trimmed();
//
//        if (line.isEmpty())
//            continue;
//
//        // Split by whitespace
//        QStringList coords = line.split(" ", Qt::SkipEmptyParts);
//
//        if (coords.size() >= 3) {
//            bool okX, okY, okZ;
//            float x = coords[0].toFloat(&okX);
//            float y = coords[1].toFloat(&okY);
//            float z = coords[2].toFloat(&okZ);
//
//            if (okX && okY && okZ) {
//                vertices.push_back(x);
//                vertices.push_back(y);
//                vertices.push_back(z);
//            }
//        }
//    }
//
//    file.close();
//
//    qDebug() << "Loaded" << vertices.size() / 3 << "points from" << filename;
//
//    update();
//}
//
//void GLWidget::clearShape()
//{
//    vertices.clear();
//    update();
//}
//
//void GLWidget::initializeGL()
//{
//    initializeOpenGLFunctions();
//
//    // Set background color (dark gray)
//    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//
//    // Enable depth testing for 3D
//    glEnable(GL_DEPTH_TEST);
//
//    // Set point size
//    glPointSize(5.0f);
//
//    // Enable line smoothing
//    glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//}
//
//void GLWidget::resizeGL(int w, int h)
//{
//    if (h == 0) h = 1;
//
//    glViewport(0, 0, w, h);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//
//    // Manual perspective calculation (avoiding gluPerspective)
//    GLfloat aspect = (GLfloat)w / (GLfloat)h;
//    GLfloat fH = tan(45.0f / 360.0f * 3.14159f) * 0.1f;
//    GLfloat fW = fH * aspect;
//
//    glFrustum(-fW, fW, -fH, fH, 0.1, 100.0);
//
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void GLWidget::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    // Camera positioning
//    glTranslatef(0.0f, 0.0f, -zoom);
//    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
//    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
//
//    // Draw coordinate axes
//    glLineWidth(2.0f);
//    glBegin(GL_LINES);
//    // X-axis (Red)
//    glColor3f(1.0f, 0.0f, 0.0f);
//    glVertex3f(0.0f, 0.0f, 0.0f);
//    glVertex3f(2.0f, 0.0f, 0.0f);
//
//    // Y-axis (Green)
//    glColor3f(0.0f, 1.0f, 0.0f);
//    glVertex3f(0.0f, 0.0f, 0.0f);
//    glVertex3f(0.0f, 2.0f, 0.0f);
//
//    // Z-axis (Blue)
//    glColor3f(0.0f, 0.0f, 1.0f);
//    glVertex3f(0.0f, 0.0f, 0.0f);
//    glVertex3f(0.0f, 0.0f, 2.0f);
//    glEnd();
//
//    // Draw the shape
//    if (!vertices.empty()) {
//        // Draw points
//        glColor3f(1.0f, 1.0f, 1.0f);
//        glPointSize(6.0f);
//
//        glBegin(GL_POINTS);
//        for (size_t i = 0; i < vertices.size(); i += 3) {
//            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
//        }
//        glEnd();
//
//        // Draw connecting lines
//        glColor3f(0.5f, 0.8f, 1.0f);
//        glLineWidth(2.0f);
//        glBegin(GL_LINE_LOOP);
//        for (size_t i = 0; i < vertices.size(); i += 3) {
//            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
//        }
//        glEnd();
//    }
//}
//
//void GLWidget::mousePressEvent(QMouseEvent* event)
//{
//    lastMousePos = event->pos();
//}
//
//void GLWidget::mouseMoveEvent(QMouseEvent* event)
//{
//    //int dx = event->x() - lastMousePos.x();
//    int dx = static_cast<int>(event->position().x()) - lastMousePos.x();
//    //int dy = event->y() - lastMousePos.y();
//    int dy = static_cast<int>(event->position().y()) - lastMousePos.y();
//
//    if (event->buttons() & Qt::LeftButton) {
//        rotationX += dy * 0.5f;
//        rotationY += dx * 0.5f;
//        update();
//    } 
//
//    lastMousePos = event->pos();
//}
//
//void GLWidget::wheelEvent(QWheelEvent* event)
//{
//    if (event->angleDelta().y() > 0)
//        zoom -= 0.5f;
//    else
//        zoom += 0.5f;
//
//    if (zoom < 1.0f) zoom = 1.0f;
//    if (zoom > 20.0f) zoom = 20.0f;
//
//    update();
//}


#include "GLWidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>
#include <cmath>

// OpenGL headers - order matters!
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , rotationX(30.0f)
    , rotationY(45.0f)
    , zoom(8.0f)
    , wireframeMode(false)
    , showAxes(true)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::calculateNormal(const float* v1, const float* v2, const float* v3, float* normal)
{
    // Calculate two edge vectors
    float edge1[3] = { v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
    float edge2[3] = { v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2] };

    // Cross product to get normal
    normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
    normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
    normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];

    // Normalize
    float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length > 0.0f) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}

void GLWidget::generateTrianglesFromPoints()
{
    triangles.clear();

    if (vertices.size() < 9) return; // Need at least 3 vertices (9 floats) for 1 triangle

    int numVertices = vertices.size() / 3;

    // Create triangles using triangle fan approach
    // This works well for convex shapes
    // First vertex is the center, then we connect consecutive vertices

    for (int i = 1; i < numVertices - 1; i++) {
        Triangle tri;

        // Vertex 1 (center point)
        tri.v1[0] = vertices[0];
        tri.v1[1] = vertices[1];
        tri.v1[2] = vertices[2];

        // Vertex 2
        tri.v2[0] = vertices[i * 3];
        tri.v2[1] = vertices[i * 3 + 1];
        tri.v2[2] = vertices[i * 3 + 2];

        // Vertex 3
        tri.v3[0] = vertices[(i + 1) * 3];
        tri.v3[1] = vertices[(i + 1) * 3 + 1];
        tri.v3[2] = vertices[(i + 1) * 3 + 2];

        // Calculate normal
        calculateNormal(tri.v1, tri.v2, tri.v3, tri.normal);

        triangles.push_back(tri);
    }

    qDebug() << "Generated" << triangles.size() << "triangles from" << numVertices << "vertices";
}

void GLWidget::loadDATFile(const QString& filename)
{
    vertices.clear();
    triangles.clear();

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

    // Generate triangles from loaded vertices
    generateTrianglesFromPoints();

    update();
}

void GLWidget::clearShape()
{
    vertices.clear();
    triangles.clear();
    update();
}

void GLWidget::setWireframeMode(bool wireframe)
{
    wireframeMode = wireframe;
    update();
}

void GLWidget::setShowAxes(bool show)
{
    showAxes = show;
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Set background color (dark gray)
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set up light properties
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // Material properties
    GLfloat matAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void GLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Manual perspective calculation
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
    if (showAxes) {
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        // X-axis (Red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(3.0f, 0.0f, 0.0f);

        // Y-axis (Green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 3.0f, 0.0f);

        // Z-axis (Blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 3.0f);
        glEnd();
        glEnable(GL_LIGHTING);
    }

    // Draw the shape using triangles
    if (!triangles.empty()) {

        if (wireframeMode) {
            // Wireframe mode
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(0.0f, 1.0f, 1.0f); // Cyan color
            glLineWidth(1.5f);
        }
        else {
            // Solid mode with lighting
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(0.3f, 0.7f, 1.0f); // Light blue color
        }

        // Render all triangles
        glBegin(GL_TRIANGLES);
        for (const auto& tri : triangles) {
            glNormal3fv(tri.normal);
            glVertex3fv(tri.v1);
            glVertex3fv(tri.v2);
            glVertex3fv(tri.v3);
        }
        glEnd();

        // Reset polygon mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Draw vertices as points (optional, for debugging)
    if (wireframeMode && !vertices.empty()) {
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glPointSize(5.0f);

        glBegin(GL_POINTS);
        for (size_t i = 0; i < vertices.size(); i += 3) {
            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        glEnd();
    }
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->position().toPoint();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint currentPos = event->position().toPoint();
    int dx = currentPos.x() - lastMousePos.x();
    int dy = currentPos.y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;
        update();
    }

    lastMousePos = currentPos;
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
        zoom -= 0.5f;
    else
        zoom += 0.5f;

    if (zoom < 2.0f) zoom = 2.0f;
    if (zoom > 30.0f) zoom = 30.0f;

    update();
}