//#include "OpenGLWidget.h"
//#include <QFile>
//#include <QTextStream>
//#include <QDebug>
//#include <QtMath>
//
//
//OpenGLWidget::OpenGLWidget(QWidget* parent): QOpenGLWidget(parent), vbo(QOpenGLBuffer::VertexBuffer)
//    , rotationX(30.0f), rotationY(45.0f), rotationZ(30.0f), zoom(10.0f), mvpLoc(-1){}
////Initialize rotation around X, Y, Z axis(30, 45, 30 deg resp.)
////Initial zoom value = 50 (camera is 50 units away along - Z)
//
//OpenGLWidget::~OpenGLWidget()
//{
//    makeCurrent();
//    vao.destroy();
//    vbo.destroy();
//    shader.removeAllShaders();
//    doneCurrent();
//}
//
//void OpenGLWidget::drawShape(std::vector<Point>& vec) // from pts
//{
//    // Convert Point to float array [x0,y0,z0, x1,y1,z1, ...]
//    mVertices.clear();
//    mVertices.reserve(vec.size() * 3);
//
//    for (const Point& pt : vec)
//    {
//        mVertices.push_back(static_cast<float>(pt.getX()));
//        mVertices.push_back(static_cast<float>(pt.getY()));
//        mVertices.push_back(static_cast<float>(pt.getZ()));
//    }
//
//
//    update();
//
//  /*  if (isValid())
//    {
//        makeCurrent();
//
//        vao.bind();
//        vbo.bind();
//        vbo.allocate(mVertices.data(), mVertices.size() * sizeof(float));
//        vbo.release();
//        vao.release();
//
//        update();
//    }*/
//}
//
//void OpenGLWidget::clearShape()
//{
//    mVertices.clear();
//    update();
//}
//
//void OpenGLWidget::initializeGL()
//{
//    initializeOpenGLFunctions();
//
//    // Set background color (white)
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//
//    // Enable depth testing for 3D
//    glEnable(GL_DEPTH_TEST);
//
//
//    // SHADERS (modern pipeline) 
//   // Vertex shader: takes vec3 position and multiplies by MVP matrix
//    const char* vertexSrc = R"(
//        attribute vec3 position;
//        uniform mat4 mvp;
//        void main()
//        {
//            gl_Position = mvp * vec4(position, 1.0);
//        }
//    )";
//
//    const char* fragmentSrc = R"(
//        void main()
//        {
//            gl_FragColor = vec4(0.7, 0.7, 0.7, 0.7);
//        }
//    )";
//
//    if (!shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc))
//        qDebug() << "Vertex shader error:" << shader.log();
//
//    if (!shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc))
//        qDebug() << "Fragment shader error:" << shader.log();
//
//    // Bind attribute location 0 to "position"
//    shader.bindAttributeLocation("position", 0);
//
//    if (!shader.link())
//        qDebug() << "Shader link error:" << shader.log();
//
//    mvpLoc = shader.uniformLocation("mvp");
//    if (mvpLoc < 0)
//        qDebug() << "Warning: mvp uniform not found in shader";
//
//
//    // VAO + VBO SETUP
//    vao.create();
//    vao.bind();
//
//    vbo.create();
//    vbo.bind();
//    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
//
//    // Enable vertex attribute 0 (position) and describe layout
//    shader.bind();
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(
//        0,                  // index (must match bindAttributeLocation)
//        3,                  // vec3
//        GL_FLOAT,           // type
//        GL_FALSE,           // normalized?
//        3 * sizeof(float),  // stride (x,y,z tightly packed)
//        nullptr             // offset in VBO
//    );
//    shader.release();
//
//    vbo.release();
//    vao.release();
//
//    // Optional: point size for when draw points
//    glPointSize(5.0f);
//
//    /*// Simple vertex + fragment shaders (hardcoded)
//    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
//        R"(
//        attribute vec3 position;
//        void main()
//        {
//            gl_Position = mvp * vec4(position, 1.0);
//        }
//    )");
//
//    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
//        R"(
//        void main()
//        {
//            gl_FragColor = vec4(0.0, 0.7, 1.0, 1.0);
//        }
//    )");
//    shader.bindAttributeLocation("position", 0);
//
//    shader.link();
//
//    vao.create();
//    vao.bind();
//
//    vbo.create();
//    vbo.bind();
//
//    if (!mVertices.empty()) {
//        vbo.allocate(mVertices.data(), mVertices.size() * sizeof(float));
//    }
//
//    shader.bind();
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(
//        0,
//        3,
//        GL_FLOAT,
//        GL_FALSE,
//        3 * sizeof(float),
//        nullptr
//    );
//
//    vao.release();
//    vbo.release();
//    shader.release();*/
//
//
//
//
//    // Set point size
//    //glPointSize(5.0f);
//
//    // Enable line smoothing
//    /*glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);*/
//}
//
//void OpenGLWidget::resizeGL(int w, int h)
//{
//    if (h == 0) h = 1;
//
//    glViewport(0, 0, w, h);
//
//    // Build projection matrix using Qt (no glMatrixMode / glFrustum)
//    projection.setToIdentity();
//    float aspect = float(w) / float(h);
//    projection.perspective(45.0f, aspect, 0.1f, 100.0f);
//
//    //glMatrixMode(GL_PROJECTION);
//    //glLoadIdentity();
//
//    //// Manual perspective calculation (avoiding gluPerspective)
//    //GLfloat aspect = (GLfloat)w / (GLfloat)h;
//    //GLfloat fH = tan(45.0f / 360.0f * 3.14159f) * 0.1f;
//    //GLfloat fW = fH * aspect;
//
//    //glFrustum(-fW, fW, -fH, fH, 0.1, 100.0);
//
//    //glMatrixMode(GL_MODELVIEW);
//}
//
//void OpenGLWidget::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    if (mVertices.empty()) return;
//
//
//    // --------- BUILD MODEL, VIEW, PROJECTION (MVP) ---------
//    QMatrix4x4 model;
//    model.setToIdentity();
//    model.rotate(rotationX, 1.0f, 0.0f, 0.0f);
//    model.rotate(rotationY, 0.0f, 1.0f, 0.0f);
//    model.rotate(rotationZ, 0.0f, 0.0f, 1.0f);
//
//    QMatrix4x4 view;
//    view.setToIdentity();
//    view.translate(0.0f, 0.0f, -zoom);   // move camera back along -Z
//
//    QMatrix4x4 mvp = projection * view * model;
//
//    // --------- BIND SHADER + SET UNIFORMS ---------
//    shader.bind();
//    shader.setUniformValue(mvpLoc, mvp);
//
//    // --------- BIND VAO + UPLOAD VERTICES + DRAW ---------
//    vao.bind();
//    vbo.bind();
//
//    // Upload CPU data to GPU every frame (simple and safe for now)
//    vbo.allocate(mVertices.data(), static_cast<int>(mVertices.size() * sizeof(float)));
//
//    // Draw as triangles
//    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(mVertices.size() / 3));
//
//    vbo.release();
//    vao.release();
//    shader.release();
//
//
//    /*glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();*/
//
//    //// Camera positioning
//    //glTranslatef(0.0f, 0.0f, -zoom);
//    //glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
//    //glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
//    //glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
//
//    //// Draw coordinate axes
//    //glLineWidth(2.0f);
//    //glBegin(GL_LINES);
//    //// X-axis (Red)
//    //glColor3f(1.0f, 0.0f, 0.0f);
//    //glVertex3f(0.0f, 0.0f, 0.0f);
//    //glVertex3f(10.0f, 0.0f, 0.0f);
//    //
//    //// Y-axis (Green)
//    //glColor3f(0.0f, 1.0f, 0.0f);
//    //glVertex3f(0.0f, 0.0f, 0.0f);
//    //glVertex3f(0.0f, 10.0f, 0.0f);
//    //
//    //// Z-axis (Blue)
//    //glColor3f(0.0f, 0.0f, 1.0f);
//    //glVertex3f(0.0f, 0.0f, 0.0f);
//    //glVertex3f(0.0f, 0.0f, 10.0f);
//    //glEnd();
//
//    
//   /* shader.bind();
//    vao.bind();
//    glDrawArrays(GL_TRIANGLES, 0, (mVertices.size() / 3));
//    vao.release();
//    shader.release();*/
//
//    // Draw the shape
//    //if (!mVertices.empty()) 
//    //{
//    //    // Draw points
//    //    glColor3f(1.0f, 1.0f, 1.0f);
//    //    glPointSize(6.0f);
//
//    //    glBegin(GL_POINTS);
//    //    for (int i = 0; i < mVertices.size(); i += 3) {
//    //        glVertex3f(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
//    //    }
//    //    glEnd();
//    //    
//    //    // Draw connecting lines
//    //    glColor3f(0.5f, 0.8f, 1.0f);
//    //    glLineWidth(2.0f);
//    //    glBegin(GL_LINE_LOOP);
//    //    for (size_t i = 0; i < mVertices.size(); i += 3) {
//    //        glVertex3f(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
//    //    }
//    //    glEnd();
//    //}
//}
//
//void OpenGLWidget::mousePressEvent(QMouseEvent* event)
//{
//    lastMousePos = event->pos();
//}
//
//void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
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
//void OpenGLWidget::wheelEvent(QWheelEvent* event)
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

#include "OpenGLWidget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_shapeVBO(QOpenGLBuffer::VertexBuffer)
    , m_axesVBO(QOpenGLBuffer::VertexBuffer)
    , m_camDistance(20.0f)
    , m_camYaw(45.0f)
    , m_camPitch(25.0f)
    , m_lightPos(10.0f, 10.0f, 10.0f)
    , m_lightColor(1.0f, 1.0f, 1.0f)
    , m_shapeColor(0.0f, 0.7f, 1.0f)    // cyan-ish
{
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    m_shapeVAO.destroy();
    m_shapeVBO.destroy();
    m_axesVAO.destroy();
    m_axesVBO.destroy();
    m_program.removeAllShaders();
    doneCurrent();
}

void OpenGLWidget::drawShape(std::vector<Point>& vec)
{
    m_vertices.clear();
    m_vertices.reserve(vec.size() * 3);

    for (const Point& p : vec)
    {
        m_vertices.push_back(static_cast<float>(p.getX()));
        m_vertices.push_back(static_cast<float>(p.getY()));
        m_vertices.push_back(static_cast<float>(p.getZ()));
    }

    update();   // trigger paintGL()
}

void OpenGLWidget::clearShape()
{
    m_vertices.clear();
    update();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // white background
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);   // white background

    // ------------ SHADERS (modern GLSL 330 core) ------------

    const char* vsSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;

        uniform bool uUseLighting;
        uniform vec3 uBaseColor;

        out vec3 vColor;
        out vec3 vFragPos;
        out vec3 vNormal;

        void main()
        {
            vec4 worldPos = uModel * vec4(aPos, 1.0);
            vFragPos = worldPos.xyz;

            // Fake normal from position (ok for simple shapes/demo)
            vNormal = normalize(mat3(uModel) * aPos);

            vColor = uBaseColor;

            gl_Position = uProj * uView * worldPos;
        }
    )";

    const char* fsSrc = R"(
        #version 330 core

        in vec3 vColor;
        in vec3 vFragPos;
        in vec3 vNormal;

        uniform bool uUseLighting;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform vec3 uViewPos;

        out vec4 FragColor;

        void main()
        {
            if (!uUseLighting)
            {
                FragColor = vec4(vColor, 1.0);
                return;
            }

            vec3 norm = normalize(vNormal);
            vec3 lightDir = normalize(uLightPos - vFragPos);

            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor;

            vec3 ambient = 0.2 * uLightColor;

            vec3 result = (ambient + diffuse) * vColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsSrc))
        qDebug() << "Vertex shader error:" << m_program.log();

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsSrc))
        qDebug() << "Fragment shader error:" << m_program.log();

    if (!m_program.link())
        qDebug() << "Shader link error:" << m_program.log();

    // ------------ SHAPE VAO/VBO ------------

    m_shapeVAO.create();
    m_shapeVAO.bind();

    m_shapeVBO.create();
    m_shapeVBO.bind();
    m_shapeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_program.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                      // location = 0 in shader
        3,                      // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    m_program.release();

    m_shapeVBO.release();
    m_shapeVAO.release();

    // ------------ AXES VAO/VBO ------------

    m_axesVAO.create();
    m_axesVAO.bind();

    m_axesVBO.create();
    m_axesVBO.bind();
    m_axesVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

    // 3 axes, 2 points each => 6 vertices
    float axisVerts[] = {
        // X axis
        0.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f,
        // Y axis
        0.0f, 0.0f, 0.0f,
        0.0f, 10.0f, 0.0f,
        // Z axis
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 10.0f
    };
    m_axesVBO.allocate(axisVerts, sizeof(axisVerts));

    m_program.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    m_program.release();

    m_axesVBO.release();
    m_axesVAO.release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    m_projection.setToIdentity();
    float aspect = float(w) / float(h);
    m_projection.perspective(45.0f, aspect, 0.1f, 500.0f);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --------- Compute camera (orbit) ---------

    float yawRad = qDegreesToRadians(m_camYaw);
    float pitchRad = qDegreesToRadians(m_camPitch);

    QVector3D camPos;
    camPos.setX(m_camDistance * qCos(pitchRad) * qSin(yawRad));
    camPos.setY(m_camDistance * qSin(pitchRad));
    camPos.setZ(m_camDistance * qCos(pitchRad) * qCos(yawRad));

    QVector3D target(0.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);

    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camPos, target, up);

    m_program.bind();

    // Common uniforms
    m_program.setUniformValue("uView", view);
    m_program.setUniformValue("uProj", m_projection);
    m_program.setUniformValue("uLightPos", m_lightPos);
    m_program.setUniformValue("uLightColor", m_lightColor);
    m_program.setUniformValue("uViewPos", camPos);

    // --------- Draw axes (no lighting) ---------
    QMatrix4x4 axesModel;
    axesModel.setToIdentity();

    m_program.setUniformValue("uModel", axesModel);
    m_program.setUniformValue("uUseLighting", false);

    m_axesVAO.bind();

    // X axis - red
    m_program.setUniformValue("uBaseColor", QVector3D(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2);

    // Y axis - green
    m_program.setUniformValue("uBaseColor", QVector3D(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 2, 2);

    // Z axis - blue
    m_program.setUniformValue("uBaseColor", QVector3D(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 4, 2);

    m_axesVAO.release();

    // --------- Draw shape (with lighting) ---------
    if (!m_vertices.empty())
    {
        QMatrix4x4 model;
        model.setToIdentity();  // you can add rotations/scales here later

        m_program.setUniformValue("uModel", model);
        m_program.setUniformValue("uUseLighting", true);
        m_program.setUniformValue("uBaseColor", m_shapeColor);

        m_shapeVAO.bind();
        m_shapeVBO.bind();

        m_shapeVBO.allocate(
            m_vertices.data(),
            static_cast<int>(m_vertices.size() * sizeof(float))
        );

        int vertexCount = static_cast<int>(m_vertices.size() / 3);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        m_shapeVBO.release();
        m_shapeVAO.release();
    }

    m_program.release();
}

// --------- Mouse & wheel: camera control ---------

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->pos().x() - m_lastMousePos.x();
    int dy = event->pos().y() - m_lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_camYaw += dx * 0.5f;
        m_camPitch += dy * 0.5f;

        if (m_camPitch > 89.0f)  m_camPitch = 89.0f;
        if (m_camPitch < -89.0f) m_camPitch = -89.0f;

        update();
    }

    m_lastMousePos = event->pos();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
        m_camDistance -= 1.0f;
    else
        m_camDistance += 1.0f;

    if (m_camDistance < 2.0f)   m_camDistance = 2.0f;
    if (m_camDistance > 200.0f) m_camDistance = 200.0f;

    update();
}
