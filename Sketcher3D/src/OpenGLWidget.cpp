#include "OpenGLWidget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , m_cameraPos(0.0f, 0.0f, 5.0f)
    , m_cameraTarget(0.0f, 0.0f, 0.0f)
    , m_lightPos(5.0f, 5.0f, 5.0f)
    , m_objectColor(0.2f, 0.7f, 1.0f)   // cyan-ish
    , m_lightColor(1.0f, 1.0f, 1.0f)    // white light
    , m_rotation()
    , m_zoom(1.0f)
    , m_pan(0.0f, 0.0f, 0.0f)
    , m_autoRotate(true)
    , m_autoRotateTimer(new QTimer(this))
{
    // Identity rotation
    m_rotation = QQuaternion();

    // Auto-rotation timer (slow spin around Y axis)
    connect(m_autoRotateTimer, &QTimer::timeout, this, [this]() {
        if (!m_autoRotate)
            return;
        QQuaternion dq = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f); // 0.5° per tick
        m_rotation = dq * m_rotation;
        updateModelMatrix();
        update();
        });
    m_autoRotateTimer->start(16); // ~60 FPS

    updateModelMatrix();
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // ---- Shaders (GLSL 330 core for Qt 6) ----
    const char* vertSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;

        uniform mat4 u_model;
        uniform mat4 u_view;
        uniform mat4 u_proj;

        out vec3 FragPos;
        out vec3 Normal;

        void main()
        {
            vec4 worldPos = u_model * vec4(aPos, 1.0);
            FragPos = worldPos.xyz;
            Normal = mat3(transpose(inverse(u_model))) * aNormal;
            gl_Position = u_proj * u_view * worldPos;
        }
    )";

    const char* fragSrc = R"(
        #version 330 core

        in vec3 FragPos;
        in vec3 Normal;

        uniform vec3 u_lightPos;
        uniform vec3 u_viewPos;
        uniform vec3 u_lightColor;
        uniform vec3 u_objectColor;

        out vec4 FragColor;

        void main()
        {
            // Ambient
            float ambientStrength = 0.15;
            vec3 ambient = ambientStrength * u_lightColor;

            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(u_lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * u_lightColor;

            // Specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(u_viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = specularStrength * spec * u_lightColor;

            vec3 result = (ambient + diffuse + specular) * u_objectColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    if (!shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vertSrc))
        qWarning() << "Vertex shader error:" << shader.log();
    if (!shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fragSrc))
        qWarning() << "Fragment shader error:" << shader.log();
    if (!shader.link())
        qWarning() << "Shader link error:" << shader.log();

    vao.create();
    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo.bind();

    if (!mVertices.empty()) {
        vbo.allocate(mVertices.data(), static_cast<int>(mVertices.size() * sizeof(float)));
    }
    else {
        vbo.allocate(nullptr, 0);
    }

    // Enable vertex attributes (locations 0 and 1 from layout qualifiers)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<void*>(0)
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float))
    );

    vbo.release();
    vao.release();

    // Initial matrices
    m_model.setToIdentity();
    updateViewMatrix();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    float aspect = static_cast<float>(w) / static_cast<float>(h);

    m_proj.setToIdentity();
    m_proj.perspective(45.0f, aspect, 0.1f, 100.0f);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mVertices.empty())
        return;

    shader.bind();
    vao.bind();

    // Set transforms
    shader.setUniformValue("u_model", m_model);
    shader.setUniformValue("u_view", m_view);
    shader.setUniformValue("u_proj", m_proj);

    // Camera / light uniforms
    shader.setUniformValue("u_lightPos", m_lightPos);
    shader.setUniformValue("u_viewPos", m_cameraPos);
    shader.setUniformValue("u_lightColor", m_lightColor);
    shader.setUniformValue("u_objectColor", m_objectColor);

    int vertexCount = static_cast<int>(mVertices.size() / 6); // 6 floats per vertex
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    vao.release();
    shader.release();
}

void OpenGLWidget::setVertices(const std::vector<Point>& points)
{
    // We expect GL_TRIANGLES: each 3 points = one triangle
    if (points.size() < 3) {
        mVertices.clear();
        update();
        return;
    }

    if (points.size() % 3 != 0) {
        qWarning() << "setVertices: points.size() is not a multiple of 3. Extra vertices will be ignored.";
    }

    mVertices.clear();
    mVertices.reserve(points.size() * 6); // 3 pos + 3 normal

    for (size_t i = 0; i + 2 < points.size(); i += 3)
    {
        QVector3D p0(
            static_cast<float>(points[i].getX()),
            static_cast<float>(points[i].getY()),
            static_cast<float>(points[i].getZ())
        );
        QVector3D p1(
            static_cast<float>(points[i + 1].getX()),
            static_cast<float>(points[i + 1].getY()),
            static_cast<float>(points[i + 1].getZ())
        );
        QVector3D p2(
            static_cast<float>(points[i + 2].getX()),
            static_cast<float>(points[i + 2].getY()),
            static_cast<float>(points[i + 2].getZ())
        );

        QVector3D e1 = p1 - p0;
        QVector3D e2 = p2 - p0;
        QVector3D n = QVector3D::crossProduct(e1, e2);

        if (!n.isNull())
            n.normalize();
        else
            n = QVector3D(0.0f, 0.0f, 1.0f);

        auto pushVertex = [&](const QVector3D& p) {
            // position
            mVertices.push_back(p.x());
            mVertices.push_back(p.y());
            mVertices.push_back(p.z());
            // normal
            mVertices.push_back(n.x());
            mVertices.push_back(n.y());
            mVertices.push_back(n.z());
            };

        pushVertex(p0);
        pushVertex(p1);
        pushVertex(p2);
    }

    if (isValid()) {
        makeCurrent();

        vao.bind();
        vbo.bind();
        vbo.allocate(mVertices.data(), static_cast<int>(mVertices.size() * sizeof(float)));

        vbo.release();
        vao.release();

        update();
    }
}

std::vector<Point> OpenGLWidget::getVertices() const
{
    std::vector<Point> points;
    points.reserve(mVertices.size() / 6);

    for (size_t i = 0; i + 5 < mVertices.size(); i += 6) {
        points.emplace_back(
            static_cast<double>(mVertices[i + 0]), // x
            static_cast<double>(mVertices[i + 1]), // y
            static_cast<double>(mVertices[i + 2])  // z
        );
    }

    return points;
}

// ---- Camera / light helpers ----

void OpenGLWidget::updateViewMatrix()
{
    m_view.setToIdentity();
    m_view.lookAt(m_cameraPos, m_cameraTarget, QVector3D(0.0f, 1.0f, 0.0f));
    update();
}

void OpenGLWidget::updateModelMatrix()
{
    m_model.setToIdentity();
    m_model.translate(m_pan);
    m_model.scale(m_zoom);
    m_model.rotate(m_rotation);
}

void OpenGLWidget::setCameraPosition(const QVector3D& pos)
{
    m_cameraPos = pos;
    updateViewMatrix();
}

void OpenGLWidget::setCameraTarget(const QVector3D& target)
{
    m_cameraTarget = target;
    updateViewMatrix();
}

void OpenGLWidget::setLightPosition(const QVector3D& pos)
{
    m_lightPos = pos;
    update();
}

void OpenGLWidget::setObjectColor(const QVector3D& color)
{
    m_objectColor = color;
    update();
}

void OpenGLWidget::setLightColor(const QVector3D& color)
{
    m_lightColor = color;
    update();
}

void OpenGLWidget::setAutoRotate(bool enabled)
{
    m_autoRotate = enabled;
}

// ---- Trackball mapping ----

QVector3D OpenGLWidget::mapToTrackball(const QPointF& p) const
{
    // Convert to [-1, 1] range
    float x = (2.0f * static_cast<float>(p.x()) - width()) / width();
    float y = (height() - 2.0f * static_cast<float>(p.y())) / height();

    float length2 = x * x + y * y;
    float z;
    if (length2 <= 1.0f)
        z = qSqrt(1.0f - length2);
    else {
        float length = qSqrt(length2);
        x /= length;
        y /= length;
        z = 0.0f;
    }
    return QVector3D(x, y, z).normalized();
}

// ---- Mouse interaction ----

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->position();

    if (event->button() == Qt::LeftButton)
        m_leftButtonPressed = true;
    else if (event->button() == Qt::MiddleButton)
        m_middleButtonPressed = true;
    else if (event->button() == Qt::RightButton)
        m_rightButtonPressed = true;

    event->accept();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPointF pos = event->position();
    float dx = static_cast<float>(pos.x() - m_lastMousePos.x());
    float dy = static_cast<float>(pos.y() - m_lastMousePos.y());

    if (m_leftButtonPressed) {
        // Trackball rotation
        QVector3D va = mapToTrackball(m_lastMousePos);
        QVector3D vb = mapToTrackball(pos);

        QVector3D axis = QVector3D::crossProduct(va, vb);
        float dot = QVector3D::dotProduct(va, vb);
        if (dot > 1.0f) dot = 1.0f;
        if (dot < -1.0f) dot = -1.0f;

        float angle = qAcos(dot); // in radians
        if (!axis.isNull() && angle > 1e-4f) {
            axis.normalize();
            float angleDeg = angle * 180.0f / float(M_PI);
            QQuaternion dq = QQuaternion::fromAxisAndAngle(axis, angleDeg);
            m_rotation = dq * m_rotation;
            updateModelMatrix();
        }
    }
    else if (m_middleButtonPressed) {
        // Pan: move x/y in view space
        float panSpeed = 0.01f;
        m_pan += QVector3D(dx * panSpeed, -dy * panSpeed, 0.0f);
        updateModelMatrix();
    }
    else if (m_rightButtonPressed) {
        // Zoom: vertical drag
        float zoomSpeed = 0.01f;
        float factor = 1.0f - dy * zoomSpeed;
        if (factor < 0.1f) factor = 0.1f;
        m_zoom *= factor;
        if (m_zoom < 0.1f) m_zoom = 0.1f;
        if (m_zoom > 20.0f) m_zoom = 20.0f;
        updateModelMatrix();
    }

    m_lastMousePos = pos;
    update();
    event->accept();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        m_leftButtonPressed = false;
    else if (event->button() == Qt::MiddleButton)
        m_middleButtonPressed = false;
    else if (event->button() == Qt::RightButton)
        m_rightButtonPressed = false;

    event->accept();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        float steps = static_cast<float>(numDegrees.y()) / 15.0f; // 15° per step
        float factor = 1.0f + steps * 0.1f;
        if (factor < 0.1f) factor = 0.1f;
        m_zoom *= factor;
        if (m_zoom < 0.1f) m_zoom = 0.1f;
        if (m_zoom > 20.0f) m_zoom = 20.0f;
        updateModelMatrix();
        update();
    }
    event->accept();
}
