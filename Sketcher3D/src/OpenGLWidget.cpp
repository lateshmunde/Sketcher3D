#include "OpenGLWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mShapeVBO(QOpenGLBuffer::VertexBuffer)
    , mRotationX(0.0f) // initial rotation around X
    , mRotationY(0.0f) // initial rotation around Y
    , mRotationZ(0.0f) // initial rotation around Y
    , mZoom(20.0f) // camera distance
    , mLightDir(0.0f, 0.0f, 1.0f) // light  from +Z toward screen
    , mObjectColor(0.0f, 0.7f, 1.0f) // blue-cyan color
{
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent(); 
    mShapeVAO.destroy();
    mShapeVBO.destroy();
    mShader.removeAllShaders();
    doneCurrent();
}

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

    update();  // Request paintGL() //tells qt to call paint
}

void OpenGLWidget::drawShapeCube(std::vector<Point>& pts, std::vector<Triangle>& tris)
{
    // Convert input Points into flat float list (x,y,z)
    mVertices.clear();
    //mVertices.reserve(pts.size() * 3);
    mVertices.reserve(36 * 3);

    for (const Triangle& t : tris)
    {
        mVertices.push_back(pts[t.m1].getX());
        mVertices.push_back(pts[t.m1].getY());
        mVertices.push_back(pts[t.m1].getZ());

        mVertices.push_back(pts[t.m2].getX());
        mVertices.push_back(pts[t.m2].getY());
        mVertices.push_back(pts[t.m2].getZ());

        mVertices.push_back(pts[t.m3].getX());
        mVertices.push_back(pts[t.m3].getY());
        mVertices.push_back(pts[t.m3].getZ());
    }

    update();  // Request paintGL() //tells qt to call paint 
}


void OpenGLWidget::clearShape()
{
    mVertices.clear();
    update();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();  
    glEnable(GL_DEPTH_TEST); // 3D depth handling
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f); // dark background

    //Vertex Shade
    const char* vs = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;

        uniform mat4 uModel; //uniform - same value for all vertices.
        uniform mat4 uView; //Moves the camera
        uniform mat4 uProj; //Projection matrix - Perspective

        uniform vec3 uLightDir;  // Direction light
        uniform vec3 uColor;  // Base object color

        out vec3 vColor;  // output color to fragment shader

        void main()
        {
            // Transform vertex position to clip space //Converts local coordinates
            gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

           // Compute normal from vertex
            vec3 normal = normalize(aPos);

            // Lighting = max(dot(N,L), 0) //diffuse lighting.
            float diff = max(dot(normal, normalize(uLightDir)), 0.0);

            // Combine base color with diffuse lighting
            vColor = uColor * (0.2 + diff);  // add little ambient light
        }
    )";

    //Fragment Shader 
    const char* fs = R"(
        #version 330 core

        in vec3 vColor; // Color from vertex shader
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0);  // Output final color //1 (opaque)
        }
    )";

    
    mShader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    mShader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
 

    // Create VAO + VBO for shape
    mShapeVAO.create(); //VAO stores vertex attribute configuration
    mShapeVAO.bind();

    mShapeVBO.create(); //Creates a Vertex Buffer Object(storage for vertices)
    mShapeVBO.bind();
    mShapeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    mShader.bind();
    glEnableVertexAttribArray(0); // enable layout(location=0)
    glVertexAttribPointer(
        0,  // index , matches layout(location = 0)
        3,  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );
    mShader.release();

    mShapeVBO.release();
    mShapeVAO.release();
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mVertices.empty())
        return;    

    // Build Model Matrix (rotate shape)
    QMatrix4x4 model;
    model.setToIdentity();
    model.rotate(mRotationX, 1, 0, 0);
    model.rotate(mRotationY, 0, 1, 0);
    model.rotate(mRotationZ, 0, 0, 1);

    //  View Matrix (fixed camera)
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


//Reference from QT example
void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    //perspective projection - viewbox
    mProjection.setToIdentity();
    mProjection.perspective(45.0f, float(w) / float(h), 0.1f, 500.0f);
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
