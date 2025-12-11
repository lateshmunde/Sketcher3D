#include "OpenGLWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mShapeVBO(QOpenGLBuffer::VertexBuffer)
    , mNormalVBO(QOpenGLBuffer::VertexBuffer)
    , mRotationX(0.0f) // initial rotation around X
    , mRotationY(0.0f) // initial rotation around Y
    , mRotationZ(0.0f) // initial rotation around z
    , mZoom(500.0f) // camera distance
    , mLightDir(0.0f, 0.0f, -1.0f) // light  from +Z toward screen
    , mLightColor(1.0f, 1.0f, 1.0f) // white light
    , mObjectColor(0.0f, 0.7f, 1.0f) // blue-cyan color
    , mCameraPos(0.0f, 0.0f, mZoom) 
{}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    mShapeVAO.destroy();
    mShapeVBO.destroy();
    //mNormalVAO.destroy();
    mNormalVBO.destroy();
    mShader.removeAllShaders();
    doneCurrent();
}

void OpenGLWidget::drawShape(const std::vector<float>& vec, const std::vector<float>& normal)
{
    for (auto it : vec)
    {
        mVertices.push_back(it);
    }

    for (int i = 0; i < normal.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            mNormals.push_back(normal[i]);
            mNormals.push_back(normal[i + 1]);
            mNormals.push_back(normal[i + 2]);
        }

    }
    update();  // Request paintGL() //tells qt to call paint
}

void OpenGLWidget::clearShape()
{
    mVertices.clear();
    mNormals.clear();
    update();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST); // 3D depth handling
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f); // dark background

    //Vertex Shader
    const char* vs = R"(
               #version 460 core

        layout(location = 0) in vec3 aPos; //vertex position coming from VBO
        layout(location = 1) in vec3 aNorm; //aNorm = vertex normal for lighting

        uniform mat4 uModel; //transforms object (translate / rotate)
        uniform mat4 uView; //camera matrix
        uniform mat4 uProj; //projection (perspective)

        //Outputs sent to fragment shader
        out vec3 vFragPos;
        out vec3 vNormal;

        void main()
        {
            //computes the final position of the vertex on screen // final clip-space position 
            gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

            //Convert the vertex position from object space -> world space // world-space fragment position
            vFragPos = vec3(uModel * vec4(aPos, 1.0));
            
            //If model includes scaling or rotation, normals must transform differently
            //normal matrix -> inverse-transpose of the 3×3 of the model matrix
            //Ensures normals stay perpendicular to surfaces.
            // world-space normal (must normalize later)
            vNormal = mat3(transpose(inverse(uModel))) * aNorm;
        }

    )";

    //Fragment Shader 
    const char* fs = R"(
                    #version 460 core

            in vec3 vFragPos;
            in vec3 vNormal;

            out vec4 FragColor; //final color of the pixel

            uniform vec3 uLightDir;   // directional light
            uniform vec3 uLightColor; // color of the light
            uniform vec3 uColor;      // base object color
            uniform vec3 uViewPos;    // camera position

            void main()
            {
                //Normals may have become non-unit due to interpolation -> normalize again.
                // Normalize interpolated normal
                vec3 N = normalize(vNormal);

                // Light direction (directional light -> use opposite)
                vec3 L = normalize(-uLightDir);

                // Diffuse shading
                float diff = max(dot(N, L), 0.0); //Dot product //negative values removed using max

                // Specular shading
                vec3 V = normalize(uViewPos - vFragPos);  // view direction //Vector from surface to camera
                vec3 R = reflect(-L, N); // reflection direction //Reflection of light direction around the normal
                float spec = pow(max(dot(V, R), 0.0), 32.0); // shininess = 32, controls highlight sharpness

                // Combine (ambient + diffuse + specular)
                vec3 ambient  = 0.2 * uLightColor; //Constant soft lighting, no dark objects
                vec3 diffuse  = diff * uLightColor; //angle between light and surface
                vec3 specular = spec * uLightColor * 0.5;  // strength = 0.5

                vec3 finalColor = (ambient + diffuse + specular) * uColor; //Multiply lighting by object color

                FragColor = vec4(finalColor, 1.0); //Output pixel color
            }
    )";


    mShader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    mShader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
    mShader.link();

    if (!mShader.link()) {
        qWarning() << "Shader link failed:" << mShader.log();
    }


    // Create VAO + VBO for shape
    mShapeVAO.create(); //VAO stores vertex attribute configuration
    mShapeVAO.bind();


    mShapeVBO.create(); //Creates a Vertex Buffer Object(storage for vertices)
    mShapeVBO.bind();
    mShapeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    //mNormalVAO.create();
    //mNormalVAO.bind();

    mNormalVBO.create(); //Creates a Vertex Buffer Object(storage for vertices)
    mNormalVBO.bind();
    mNormalVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    mShader.bind();

    // Position attribute
    mShapeVBO.bind();
    glEnableVertexAttribArray(0); // enable layout(location=0)
    glVertexAttribPointer(
        0,  // index , matches layout(location = 0)
        3,  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    // Normal attribute
    mNormalVBO.bind();
    glEnableVertexAttribArray(1); // enable layout(location=1)
    glVertexAttribPointer(
        1,  // index , matches layout(location = 0)
        3,  // vec3
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    mShader.release();
    mShapeVBO.release();
    mShapeVAO.release();
    mNormalVBO.release();
    //mNormalVAO.release();
}

//Reference from QT example
void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    //perspective projection - viewbox
    mProjection.setToIdentity();
    mProjection.perspective(45.0f, float(w) / float(h), 0.1f, 2000.0f); //vertical angle, aspect ratio, near plane ,far plane
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

    //  View Matrix (camera)
    QMatrix4x4 view;
    view.setToIdentity();
    view.translate(0, 0, -mZoom);   // move backward on Z

    mCameraPos = QVector3D(0, 0, mZoom);

    // Bind shader
    mShader.bind();
    // Send minimal uniforms
    mShader.setUniformValue("uModel", model);
    mShader.setUniformValue("uView", view);
    mShader.setUniformValue("uProj", mProjection);

    mShader.setUniformValue("uLightDir", mLightDir);
    mShader.setUniformValue("uLightColor", mLightColor);
    mShader.setUniformValue("uColor", mObjectColor);
    mShader.setUniformValue("uViewPos", mCameraPos);


    // Bind VAO + upload vertex data
    mShapeVAO.bind();
    mShapeVBO.bind();
    mShapeVBO.allocate(mVertices.data(), mVertices.size() * sizeof(float));

    //mNormalVAO.bind();
    mNormalVBO.bind();
    mNormalVBO.allocate(mNormals.data(), mNormals.size() * sizeof(float));

    // Draw shape as triangles
    int vertexCount = mVertices.size() / 3;
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    mShapeVBO.release();
    mShapeVAO.release();
    mNormalVBO.release();
    //mNormalVAO.release();
    mShader.release();
}


//Reference from QT example
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
    float zoomFactor = 1.1f;   // 10% zoom

    if (event->angleDelta().y() > 0)
        mZoom /= zoomFactor;   // zoom in (move camera closer)
    else
        mZoom *= zoomFactor;   // zoom out (move camera back)

    // Clamp range so it never gets stuck
    if (mZoom < 1.0f)   mZoom = 1.0f;
    if (mZoom > 2000.0f) mZoom = 2000.0f;

    update();
}

void OpenGLWidget::setPreDetails()
{
    this->mPreVertices = this->mVertices;
    this->mPreNormals = this->mNormals;
}

void OpenGLWidget::delCurShapeForTrans()
{
    this->mVertices = this->mPreVertices;
    this->mNormals = this->mPreNormals;
}
