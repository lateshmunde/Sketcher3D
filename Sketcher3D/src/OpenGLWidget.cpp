//#include "OpenGLWidget.h"
//#include <QMouseEvent>
//#include <QWheelEvent>
//#include <QtMath>
//#include <QDebug>
//
//OpenGLWidget::OpenGLWidget(QWidget* parent)
//    : QOpenGLWidget(parent)
//    , mShapeVBO(QOpenGLBuffer::VertexBuffer)
//    , mNormalVBO(QOpenGLBuffer::VertexBuffer)
//    , mRotationX(0.0f) // initial rotation around X
//    , mRotationY(0.0f) // initial rotation around Y
//    , mRotationZ(0.0f) // initial rotation around z
//    , mZoom(500.0f) // camera distance
//    , mLightDir(0.0f, 0.0f, -1.0f) // light  from +Z toward screen
//    , mLightColor(1.0f, 1.0f, 1.0f) // white light
//    , mObjectColor(0.0f, 0.7f, 1.0f) // blue-cyan color
//    , mCameraPos(0.0f, 0.0f, mZoom) 
//{}
//
//OpenGLWidget::~OpenGLWidget()
//{
//    makeCurrent();
//    mShapeVAO.destroy();
//    mShapeVBO.destroy();
//    //mNormalVAO.destroy();
//    mNormalVBO.destroy();
//    mShader.removeAllShaders();
//    doneCurrent();
//}
//
//void OpenGLWidget::drawShape(const std::vector<float>& vec, const std::vector<float>& normal)
//{
//    for (auto it : vec)
//    {
//        mVertices.push_back(it);
//    }
//
//    for (int i = 0; i < normal.size(); i += 3)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            mNormals.push_back(normal[i]);
//            mNormals.push_back(normal[i + 1]);
//            mNormals.push_back(normal[i + 2]);
//        }
//
//    }
//    update();  // Request paintGL() //tells qt to call paint
//}
//
//void OpenGLWidget::clearShape()
//{
//    mVertices.clear();
//    mNormals.clear();
//    update();
//}
//
//void OpenGLWidget::initializeGL()
//{
//    initializeOpenGLFunctions();
//    glEnable(GL_DEPTH_TEST); // 3D depth handling
//    glClearColor(0.1f, 0.1f, 0.1f, 0.1f); // dark background
//
//    //Vertex Shader
//    const char* vs = R"(
//               #version 460 core
//
//        layout(location = 0) in vec3 aPos; //vertex position coming from VBO
//        layout(location = 1) in vec3 aNorm; //aNorm = vertex normal for lighting
//
//        uniform mat4 uModel; //transforms object (translate / rotate)
//        uniform mat4 uView; //camera matrix
//        uniform mat4 uProj; //projection (perspective)
//
//        //Outputs sent to fragment shader
//        out vec3 vFragPos;
//        out vec3 vNormal;
//
//        void main()
//        {
//            //computes the final position of the vertex on screen // final clip-space position 
//            gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
//
//            //Convert the vertex position from object space -> world space // world-space fragment position
//            vFragPos = vec3(uModel * vec4(aPos, 1.0));
//            
//            //If model includes scaling or rotation, normals must transform differently
//            //normal matrix -> inverse-transpose of the 3×3 of the model matrix
//            //Ensures normals stay perpendicular to surfaces.
//            // world-space normal (must normalize later)
//            vNormal = mat3(transpose(inverse(uModel))) * aNorm;
//        }
//
//    )";
//
//    //Fragment Shader 
//    const char* fs = R"(
//                    #version 460 core
//
//            in vec3 vFragPos;
//            in vec3 vNormal;
//
//            out vec4 FragColor; //final color of the pixel
//
//            uniform vec3 uLightDir;   // directional light
//            uniform vec3 uLightColor; // color of the light
//            uniform vec3 uColor;      // base object color
//            uniform vec3 uViewPos;    // camera position
//
//            void main()
//            {
//                //Normals may have become non-unit due to interpolation -> normalize again.
//                // Normalize interpolated normal
//                vec3 N = normalize(vNormal);
//
//                // Light direction (directional light -> use opposite)
//                vec3 L = normalize(-uLightDir);
//
//                // Diffuse shading
//                float diff = max(dot(N, L), 0.0); //Dot product //negative values removed using max
//
//                // Specular shading
//                vec3 V = normalize(uViewPos - vFragPos);  // view direction //Vector from surface to camera
//                vec3 R = reflect(-L, N); // reflection direction //Reflection of light direction around the normal
//                float spec = pow(max(dot(V, R), 0.0), 32.0); // shininess = 32, controls highlight sharpness
//
//                // Combine (ambient + diffuse + specular)
//                vec3 ambient  = 0.2 * uLightColor; //Constant soft lighting, no dark objects
//                vec3 diffuse  = diff * uLightColor; //angle between light and surface
//                vec3 specular = spec * uLightColor * 0.5;  // strength = 0.5
//
//                vec3 finalColor = (ambient + diffuse + specular) * uColor; //Multiply lighting by object color
//
//                FragColor = vec4(finalColor, 1.0); //Output pixel color
//            }
//    )";
//
//
//    mShader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
//    mShader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
//    mShader.link();
//
//    if (!mShader.link()) {
//        qWarning() << "Shader link failed:" << mShader.log();
//    }
//
//
//    // Create VAO + VBO for shape
//    mShapeVAO.create(); //VAO stores vertex attribute configuration
//    mShapeVAO.bind();
//
//
//    mShapeVBO.create(); //Creates a Vertex Buffer Object(storage for vertices)
//    mShapeVBO.bind();
//    mShapeVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
//
//    //mNormalVAO.create();
//    //mNormalVAO.bind();
//
//    mNormalVBO.create(); //Creates a Vertex Buffer Object(storage for vertices)
//    mNormalVBO.bind();
//    mNormalVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
//
//    mShader.bind();
//
//    // Position attribute
//    mShapeVBO.bind();
//    glEnableVertexAttribArray(0); // enable layout(location=0)
//    glVertexAttribPointer(
//        0,  // index , matches layout(location = 0)
//        3,  // vec3
//        GL_FLOAT,
//        GL_FALSE,
//        3 * sizeof(float),
//        nullptr
//    );
//
//    // Normal attribute
//    mNormalVBO.bind();
//    glEnableVertexAttribArray(1); // enable layout(location=1)
//    glVertexAttribPointer(
//        1,  // index , matches layout(location = 0)
//        3,  // vec3
//        GL_FLOAT,
//        GL_FALSE,
//        3 * sizeof(float),
//        nullptr
//    );
//
//    mShader.release();
//    mShapeVBO.release();
//    mShapeVAO.release();
//    mNormalVBO.release();
//    //mNormalVAO.release();
//}
//
////Reference from QT example
//void OpenGLWidget::resizeGL(int w, int h)
//{
//    if (h == 0) h = 1;
//    glViewport(0, 0, w, h);
//
//    //perspective projection - viewbox
//    mProjection.setToIdentity();
//    mProjection.perspective(45.0f, float(w) / float(h), 0.1f, 2000.0f); //vertical angle, aspect ratio, near plane ,far plane
//}
//
//void OpenGLWidget::paintGL()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    if (mVertices.empty())
//        return;
//
//    // Build Model Matrix (rotate shape)
//    QMatrix4x4 model;
//    model.setToIdentity();
//    model.rotate(mRotationX, 1, 0, 0);
//    model.rotate(mRotationY, 0, 1, 0);
//    model.rotate(mRotationZ, 0, 0, 1);
//
//    //  View Matrix (camera)
//    QMatrix4x4 view;
//    view.setToIdentity();
//    view.translate(0, 0, -mZoom);   // move backward on Z
//
//    mCameraPos = QVector3D(0, 0, mZoom);
//
//    // Bind shader
//    mShader.bind();
//    // Send minimal uniforms
//    mShader.setUniformValue("uModel", model);
//    mShader.setUniformValue("uView", view);
//    mShader.setUniformValue("uProj", mProjection);
//
//    mShader.setUniformValue("uLightDir", mLightDir);
//    mShader.setUniformValue("uLightColor", mLightColor);
//    mShader.setUniformValue("uColor", mObjectColor);
//    mShader.setUniformValue("uViewPos", mCameraPos);
//
//
//    // Bind VAO + upload vertex data
//    mShapeVAO.bind();
//    mShapeVBO.bind();
//    mShapeVBO.allocate(mVertices.data(), mVertices.size() * sizeof(float));
//
//    //mNormalVAO.bind();
//    mNormalVBO.bind();
//    mNormalVBO.allocate(mNormals.data(), mNormals.size() * sizeof(float));
//
//    // Draw shape as triangles
//    int vertexCount = mVertices.size() / 3;
//    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//
//    mShapeVBO.release();
//    mShapeVAO.release();
//    mNormalVBO.release();
//    //mNormalVAO.release();
//    mShader.release();
//}
//
//
////Reference from QT example
////void OpenGLWidget::mousePressEvent(QMouseEvent* event)
////{
////    mLastMousePos = event->pos();
////}
//
////void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
////{
////    int dx = event->pos().x() - mLastMousePos.x();
////    int dy = event->pos().y() - mLastMousePos.y();
////
////    if (event->buttons() & Qt::LeftButton)
////    {
////        mRotationX += dy * 0.5f;  // rotate up/down
////        mRotationY += dx * 0.5f;  // rotate left/right
////        update();
////    }
////
////    mLastMousePos = event->pos();
////}
//
//
//void OpenGLWidget::wheelEvent(QWheelEvent* event)
//{
//    float zoomFactor = 1.1f;   // 10% zoom
//
//    if (event->angleDelta().y() > 0)
//        mZoom /= zoomFactor;   // zoom in (move camera closer)
//    else
//        mZoom *= zoomFactor;   // zoom out (move camera back)
//
//    // Clamp range so it never gets stuck
//    if (mZoom < 1.0f)   mZoom = 1.0f;
//    if (mZoom > 2000.0f) mZoom = 2000.0f;
//
//    update();
//}
//
//
//void OpenGLWidget::startCuboidTool()
//{
//    firstPointSelected = false;
//    drawingRectangle = false;
//    extruding = false;
//}
//
//
//
//void OpenGLWidget::mousePressEvent(QMouseEvent * event)
//{
//    mLastMousePos = event->pos();
//
//    if (currentTool != Tool::Cuboid) return;
//
//    // FIRST CLICK
//    if (!firstPointSelected && !drawingRectangle && !extruding)
//    {
//        firstCorner = unprojectToGround(event->pos());
//        firstPointSelected = true;
//        return;
//    }
//
//    // SECOND CLICK
//    if (drawingRectangle && !extruding)
//    {
//        secondCorner = unprojectToGround(event->pos());
//
//        drawingRectangle = false;
//        extruding = true;      // go to height mode
//        startExtrudeY = event->pos().y();  // store screen Y for height
//
//        return;
//    }
//
//    // THIRD CLICK -> finalize extrusion
//    if (extruding)
//    {
//        finalizeCuboid(firstCorner, secondCorner, extrusionHeight);
//
//        // reset states
//        firstPointSelected = false;
//        drawingRectangle = false;
//        extruding = false;
//
//        currentTool = Tool::Select;
//        return;
//    }
//   
//      
//}
//
//
//void OpenGLWidget::mouseMoveEvent(QMouseEvent * event)
//{
//    // camera rotate when shape created
//    int dx = event->pos().x() - mLastMousePos.x();
//    int dy = event->pos().y() - mLastMousePos.y();
//
//    if (event->buttons() & Qt::LeftButton)
//    {
//        mRotationX += dy * 0.5f;  // rotate up/down
//        mRotationY += dx * 0.5f;  // rotate left/right
//        update();
//    }
//
//    mLastMousePos = event->pos();
//
//
//    // Cuboid tool
//    if (currentTool != Tool::Cuboid) return;
//
//    // After first click -> rectangle follows mouse
//    if (firstPointSelected && !drawingRectangle && !extruding)
//    {
//        drawingRectangle = true;
//    }
//
//    // rectangle preview
//    if (drawingRectangle)
//    {
//        secondCorner = unprojectToGround(e->pos());
//        update();
//    }
//
//    // extrude preview
//    if (extruding)
//    {
//        float dy = startExtrudeY - e->pos().y();
//        extrusionHeight = dy * 0.05f;   // scale factor
//        update();
//    }
//}
//



#include "OpenGLWidget.h"
#include <QOpenGLShader>
#include <QDebug>
#include <cmath>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer)
{
    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    m_vao.destroy();
    if (m_vbo.isCreated()) m_vbo.destroy();
    delete m_prog;
    doneCurrent();
}

void OpenGLWidget::drawShape(const std::vector<float>& vec, const std::vector<float>& normal)
{
    for (auto it : vec)
    {
        mVertices.push_back(it);
    }

    for (size_t i = 0; i < normal.size(); i += 3)
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



void OpenGLWidget::startCuboidTool()
{
    firstPointSelected = false;
    drawingRectangle = false;
    extruding = false;
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);
    glClearColor(0.1f, 0.12f, 0.14f, 1.0f);

    // simple shader (uniform mat4 mvp; attribute vec3 pos;)
    m_prog = new QOpenGLShaderProgram(this);
    const char* vsrc =
        "#version 330\n"
        "layout(location = 0) in vec3 pos;\n"
        "uniform mat4 mvp;\n"
        "void main() {\n"
        "  gl_Position = mvp * vec4(pos, 1.0);\n"
        "}\n";
    const char* fsrc =
        "#version 330\n"
        "out vec4 fragColor;\n"
        "void main() { fragColor = vec4(0.9, 0.9, 0.9, 1.0); }\n";

    m_prog->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    m_prog->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_prog->link();

    // VBO + VAO
    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    // attribute 0 = vec3 pos
    m_prog->enableAttributeArray(0);
    m_prog->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    m_vbo.release();
    m_vao.release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_viewportW = w;
    m_viewportH = h;
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Build matrices: projection * view
    QMatrix4x4 proj;
    proj.perspective(45.0f, float(m_viewportW) / float(std::max(1, m_viewportH)), 0.1f, 5000.0f);

    QMatrix4x4 view;
    // Place camera at (0, camY, zoom) looking at origin after rotations
    view.translate(m_panX, m_panY, -m_zoom);
    view.rotate(m_rotX, 1.0f, 0.0f, 0.0f);
    view.rotate(m_rotY, 0.0f, 1.0f, 0.0f);

    QMatrix4x4 mvp = proj * view;

    // assemble line vertices for scene + preview
    updatePreviewVertices();

    // upload vertices
    m_vao.bind();
    m_vbo.bind();
    if (!m_lineVertices.empty()) {
        m_vbo.allocate(m_lineVertices.data(), int(m_lineVertices.size() * sizeof(float)));
    }
    else {
        // minimal empty buffer to avoid issues
        static float dummy[3] = { 0.0f,0.0f,0.0f };
        m_vbo.allocate(dummy, sizeof(dummy));
    }

    m_prog->bind();
    m_prog->setUniformValue("mvp", mvp);

    // draw lines
    if (!m_lineVertices.empty()) {
        glDrawArrays(GL_LINES, 0, int(m_lineVertices.size() / 3));
    }

    m_prog->release();
    m_vbo.release();
    m_vao.release();
}

// ----------------- Input handling -----------------

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    // left button only for creation
    if (event->button() != Qt::LeftButton) return;

    // FIRST CLICK -> pick first corner on ground plane
    if (!firstPointSelected && !drawingRectangle && !extruding) {
        firstCornerWorld = unprojectScreenToGround(event->pos());
        firstPointSelected = true;
        // we don't set drawingRectangle here; it will be set on mouseMove to start following pointer
        update();
        return;
    }

    // SECOND CLICK -> if rectangle was being drawn, finalize base and go to extrude
    if (drawingRectangle && !extruding) {
        secondCornerWorld = unprojectScreenToGround(event->pos());
        drawingRectangle = false;
        extruding = true;
        startExtrudeScreenY = float(event->pos().y());
        currentExtrudeHeight = 0.0f;
        update();
        return;
    }

    // THIRD CLICK -> finalize extrusion and add cuboid
    if (extruding) {
        finalizeCuboid();
        // reset booleans
        firstPointSelected = false;
        drawingRectangle = false;
        extruding = false;
        update();
        return;
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // camera rotate/pan if right button? (optional)
    if (event->buttons() & Qt::RightButton) {
        static QPoint last;
        if (last.isNull()) last = event->pos();
        int dx = event->pos().x() - last.x();
        int dy = event->pos().y() - last.y();
        m_rotY += dx * 0.4f;
        m_rotX += dy * 0.4f;
        last = event->pos();
        update();
        return;
    }

    // After first click, we begin drawing rectangle following mouse
    if (firstPointSelected && !drawingRectangle && !extruding) {
        drawingRectangle = true; // start previewing
    }

    if (drawingRectangle) {
        secondCornerWorld = unprojectScreenToGround(event->pos());
        update();
    }

    if (extruding) {
        float dy = startExtrudeScreenY - float(event->pos().y());
        currentExtrudeHeight = dy * 0.2f; // scale factor - tweak as desired
        if (currentExtrudeHeight < 0.0f) currentExtrudeHeight = 0.0f;
        update();
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    // zoom
    m_zoom -= event->angleDelta().y() * 0.05f;
    if (m_zoom < 10.0f) m_zoom = 10.0f;
    if (m_zoom > 4000.0f) m_zoom = 4000.0f;
    update();
}

// ----------------- Helpers -----------------

QVector3D OpenGLWidget::unprojectScreenToGround(const QPoint& screenPos)
{
    // Convert screen to NDC
    float x = (2.0f * screenPos.x()) / float(std::max(1, m_viewportW)) - 1.0f;
    float y = 1.0f - (2.0f * screenPos.y()) / float(std::max(1, m_viewportH));
    // we will unproject to near and far in NDC space
    QVector4D ndcNear(x, y, -1.0f, 1.0f);
    QVector4D ndcFar(x, y, 1.0f, 1.0f);

    // reconstruct projection and view (same as paintGL)
    QMatrix4x4 proj;
    proj.perspective(45.0f, float(m_viewportW) / float(std::max(1, m_viewportH)), 0.1f, 5000.0f);

    QMatrix4x4 view;
    view.translate(m_panX, m_panY, -m_zoom);
    view.rotate(m_rotX, 1.0f, 0.0f, 0.0f);
    view.rotate(m_rotY, 0.0f, 1.0f, 0.0f);

    QMatrix4x4 inv = (proj * view).inverted();

    QVector4D worldNear = inv * ndcNear;
    QVector4D worldFar = inv * ndcFar;
    if (worldNear.w() != 0.0f) worldNear /= worldNear.w();
    if (worldFar.w() != 0.0f) worldFar /= worldFar.w();

    QVector3D pNear(worldNear.x(), worldNear.y(), worldNear.z());
    QVector3D pFar(worldFar.x(), worldFar.y(), worldFar.z());

    // Ray from pNear -> pFar
    QVector3D dir = (pFar - pNear).normalized();

    // Intersect with plane y = 0 (ground). Solve pNear.y + t*dir.y = 0
    if (std::abs(dir.y()) < 1e-6f) {
        // ray parallel to plane; fallback to pNear projected to ground x/z
        return QVector3D(pNear.x(), 0.0f, pNear.z());
    }
    float t = -pNear.y() / dir.y();
    QVector3D hit = pNear + dir * t;
    return QVector3D(hit.x(), 0.0f, hit.z());
}

void OpenGLWidget::updatePreviewVertices()
{
    m_lineVertices.clear();

    // 1) add existing cuboids' wireframe edges
    for (const Cuboid1& c : m_cuboids) {
        // compute 8 corners
        float x1 = c.minCorner.x(), x2 = c.maxCorner.x();
        float z1 = c.minCorner.z(), z2 = c.maxCorner.z();
        float y0 = 0.0f;
        float y1 = c.height;

        // 12 edges -> 24 vertices (pairs)
        auto pushLine = [&](QVector3D a, QVector3D b) {
            m_lineVertices.push_back(a.x()); m_lineVertices.push_back(a.y()); m_lineVertices.push_back(a.z());
            m_lineVertices.push_back(b.x()); m_lineVertices.push_back(b.y()); m_lineVertices.push_back(b.z());
            };

        // bottom rectangle
        pushLine({ x1,y0,z1 }, { x2,y0,z1 });
        pushLine({ x2,y0,z1 }, { x2,y0,z2 });
        pushLine({ x2,y0,z2 }, { x1,y0,z2 });
        pushLine({ x1,y0,z2 }, { x1,y0,z1 });

        // top rectangle
        pushLine({ x1,y1,z1 }, { x2,y1,z1 });
        pushLine({ x2,y1,z1 }, { x2,y1,z2 });
        pushLine({ x2,y1,z2 }, { x1,y1,z2 });
        pushLine({ x1,y1,z2 }, { x1,y1,z1 });

        // vertical edges
        pushLine({ x1,y0,z1 }, { x1,y1,z1 });
        pushLine({ x2,y0,z1 }, { x2,y1,z1 });
        pushLine({ x2,y0,z2 }, { x2,y1,z2 });
        pushLine({ x1,y0,z2 }, { x1,y1,z2 });
    }

    // 2) rectangle preview (while drawingRectangle or when first point selected and following)
    if (firstPointSelected && drawingRectangle && !extruding) {
        float x1 = firstCornerWorld.x();
        float z1 = firstCornerWorld.z();
        float x2 = secondCornerWorld.x();
        float z2 = secondCornerWorld.z();
        float y = 0.0f;

        auto push = [&](QVector3D a, QVector3D b) {
            m_lineVertices.push_back(a.x()); m_lineVertices.push_back(a.y()); m_lineVertices.push_back(a.z());
            m_lineVertices.push_back(b.x()); m_lineVertices.push_back(b.y()); m_lineVertices.push_back(b.z());
            };

        push({ x1,y,z1 }, { x2,y,z1 });
        push({ x2,y,z1 }, { x2,y,z2 });
        push({ x2,y,z2 }, { x1,y,z2 });
        push({ x1,y,z2 }, { x1,y,z1 });
    }

    // 3) extrude preview (top rectangle + verticals)
    if (firstPointSelected && extruding) {
        float x1 = firstCornerWorld.x();
        float z1 = firstCornerWorld.z();
        float x2 = secondCornerWorld.x();
        float z2 = secondCornerWorld.z();
        float y0 = 0.0f;
        float y1 = currentExtrudeHeight;

        auto push = [&](QVector3D a, QVector3D b) {
            m_lineVertices.push_back(a.x()); m_lineVertices.push_back(a.y()); m_lineVertices.push_back(a.z());
            m_lineVertices.push_back(b.x()); m_lineVertices.push_back(b.y()); m_lineVertices.push_back(b.z());
            };

        // bottom
        push({ x1,y0,z1 }, { x2,y0,z1 });
        push({ x2,y0,z1 }, { x2,y0,z2 });
        push({ x2,y0,z2 }, { x1,y0,z2 });
        push({ x1,y0,z2 }, { x1,y0,z1 });

        // top
        push({ x1,y1,z1 }, { x2,y1,z1 });
        push({ x2,y1,z1 }, { x2,y1,z2 });
        push({ x2,y1,z2 }, { x1,y1,z2 });
        push({ x1,y1,z2 }, { x1,y1,z1 });

        // verticals
        push({ x1,y0,z1 }, { x1,y1,z1 });
        push({ x2,y0,z1 }, { x2,y1,z1 });
        push({ x2,y0,z2 }, { x2,y1,z2 });
        push({ x1,y0,z2 }, { x1,y1,z2 });
    }
}

void OpenGLWidget::finalizeCuboid()
{
    // ensure base corners are min/max
    float x1 = firstCornerWorld.x();
    float z1 = firstCornerWorld.z();
    float x2 = secondCornerWorld.x();
    float z2 = secondCornerWorld.z();

    Cuboid1 c;
    c.minCorner.setX(std::min(x1, x2));
    c.maxCorner.setX(std::max(x1, x2));
    c.minCorner.setZ(std::min(z1, z2));
    c.maxCorner.setZ(std::max(z1, z2));

    c.minCorner.setY(0.0f);
    c.maxCorner.setY(0.0f);
    c.height = currentExtrudeHeight;

    // avoid zero-size bases
    const float eps = 1e-3f;
    if (std::abs(c.maxCorner.x() - c.minCorner.x()) < eps) c.maxCorner.setX(c.minCorner.x() + 1.0f);
    if (std::abs(c.maxCorner.z() - c.minCorner.z()) < eps) c.maxCorner.setZ(c.minCorner.z() + 1.0f);
    if (c.height < eps) c.height = 1.0f;

    m_cuboids.push_back(c);
}
