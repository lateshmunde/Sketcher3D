#include "OpenGLWidget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>

// ---------------- Constructor ----------------

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mShapeVBO(QOpenGLBuffer::VertexBuffer)
    , mRotationX(0.0f)                     // initial rotation around X
    , mRotationY(0.0f)                     // initial rotation around Y
    , mZoom(20.0f)                          // camera distance
    , mLightDir(0.0f, 0.0f, 1.0f)           // light coming from +Z toward screen
    , mObjectColor(0.0f, 0.7f, 1.0f)        // blue-cyan color
{
}

// ---------------- Destructor ----------------

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();               // Ensure the GL context is active before deleting
    mShapeVAO.destroy();
    mShapeVBO.destroy();
    mShader.removeAllShaders();
    doneCurrent();
}

// ---------------- API: draw new shape ----------------

void OpenGLWidget::drawShape(std::vector<Point>& vec)
{
    // Convert input Points into flat float list (x,y,z)
    mVertices.clear();
    mVertices.reserve(vec.size() * 3);

    for (const Point& p : vec)
    {
        mVertices.push_back(p.getX());
        mVertices.push_back(p.getY());
        mVertices.push_back(p.getZ());
    }

    update();     // Request paintGL()
}

// ---------------- API: clear shape ----------------

void OpenGLWidget::clearShape()
{
    mVertices.clear();
    update();
}

// ---------------- Create OpenGL resources ----------------

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();      // REQUIRED: enables OpenGL function calls
    glEnable(GL_DEPTH_TEST);          // 3D depth handling
    glClearColor(1, 1, 1, 1);          // white background

    // -------- Minimal Vertex Shader --------
    const char* vs = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;

        uniform vec3 uLightDir;       // Direction light
        uniform vec3 uColor;          // Base object color

        out vec3 vColor;              // Final color to fragment shader

        void main()
        {
            // Transform vertex position to clip space
            gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

            // Compute normal from vertex (works OK for convex poly shapes)
            vec3 normal = normalize(aPos);

            // Lighting = max(dot(N,L), 0)
            float diff = max(dot(normal, normalize(uLightDir)), 0.0);

            // Combine base color with diffuse lighting
            vColor = uColor * (0.2 + diff);  // add little ambient light
        }
    )";

    // -------- Minimal Fragment Shader --------
    const char* fs = R"(
        #version 330 core

        in vec3 vColor;         // Color from vertex shader
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0);   // Output final color
        }
    )";

    // Compile + link shader program
    mShader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    mShader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
    if (!mShader.link())
        qDebug() << "Shader link error:" << mShader.log();

    // -------- Create VAO + VBO for shape --------
    mShapeVAO.create();
    mShapeVAO.bind();

    mShapeVBO.create();
    mShapeVBO.bind();
    mShapeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    mShader.bind();
    glEnableVertexAttribArray(0);                 // enable layout(location=0)
    glVertexAttribPointer(
        0,                                         // index
        3,                                         // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );
    mShader.release();

    mShapeVBO.release();
    mShapeVAO.release();
}


void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    //perspective projection - viewbox
    mProjection.setToIdentity();
    mProjection.perspective(45.0f, float(w) / float(h), 0.1f, 500.0f);
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mVertices.empty())
        return;    // nothing to draw

    // ----- Build Model Matrix (rotate shape) -----
    QMatrix4x4 model;
    model.setToIdentity();
    model.rotate(mRotationX, 1, 0, 0);
    model.rotate(mRotationY, 0, 1, 0);

    // ----- Build View Matrix (fixed camera) -----
    QMatrix4x4 view;
    view.setToIdentity();
    view.translate(0, 0, -mZoom);   // move backward on Z

    // Bind shader
    mShader.bind();

    // Send minimal uniforms
    mShader.setUniformValue("uModel", model);
    mShader.setUniformValue("uView", view);
    mShader.setUniformValue("uProj", mProjection);
    mShader.setUniformValue("uLightDir", mLightDir);
    mShader.setUniformValue("uColor", mObjectColor);

    // Bind VAO + upload vertex data
    mShapeVAO.bind();
    mShapeVBO.bind();
    mShapeVBO.allocate(mVertices.data(), mVertices.size() * sizeof(float));

    // Draw shape as triangles
    int vertexCount = mVertices.size() / 3;
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    mShapeVBO.release();
    mShapeVAO.release();
    mShader.release();
}


void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    mLastMousePos = event->pos();
}



void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->pos().x() - mLastMousePos.x();
    int dy = event->pos().y() - mLastMousePos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        mRotationX += dy * 0.5f;  // rotate up/down
        mRotationY += dx * 0.5f;  // rotate left/right
        update();
    }

    mLastMousePos = event->pos();
}


void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
        mZoom -= 1.0f;
    else
        mZoom += 1.0f;

    if (mZoom < 5.0f)   mZoom = 5.0f;
    if (mZoom > 200.0f) mZoom = 200.0f;

    update();
}
