#include "OpenGLWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QRandomGenerator>
#include <QDebug>
#include <limits>
#include <cmath>
#include <algorithm>

// Random float in [minVal, maxVal)
static inline float frand(float minVal, float maxVal) {
    double t = QRandomGenerator::global()->generateDouble(); // [0,1)
    return float(minVal + t * (maxVal - minVal));
}

// ------- Normal helpers -------
// Compute flat (per-triangle) normals and duplicate per vertex.
std::vector<float> OpenGLWidget::computeFlatNormals(const std::vector<float>& v)
{
    std::vector<float> n(v.size(), 0.0f);
    if (v.size() % 9 != 0) return n;

    for (size_t i = 0; i < v.size(); i += 9) {
        QVector3D p0(v[i + 0], v[i + 1], v[i + 2]);
        QVector3D p1(v[i + 3], v[i + 4], v[i + 5]);
        QVector3D p2(v[i + 6], v[i + 7], v[i + 8]);

        QVector3D u = p1 - p0;
        QVector3D v2 = p2 - p0;
        QVector3D normal = QVector3D::crossProduct(u, v2);
        if (!normal.isNull()) normal.normalize(); else normal = QVector3D(0, 0, 1);

        for (int k = 0; k < 3; ++k) {
            n[i + 3 * k + 0] = normal.x();
            n[i + 3 * k + 1] = normal.y();
            n[i + 3 * k + 2] = normal.z();
        }
    }
    return n;
}

// Expand triangle normals (3 floats per tri) -> (9 floats per tri).
std::vector<float> OpenGLWidget::expandTriangleNormals(const std::vector<float>& triNormals,
    size_t vertexCount)
{
    std::vector<float> expanded;
    if (triNormals.empty()) return expanded;

    const size_t triCount = triNormals.size() / 3;
    if (triCount * 9 != vertexCount) return expanded;

    expanded.resize(triCount * 9);
    size_t w = 0;
    for (size_t t = 0; t < triCount; ++t) {
        float nx = triNormals[3 * t + 0];
        float ny = triNormals[3 * t + 1];
        float nz = triNormals[3 * t + 2];
        for (int v = 0; v < 3; ++v) {
            expanded[w++] = nx; expanded[w++] = ny; expanded[w++] = nz;
        }
    }
    return expanded;
}

// ----------------------------

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , mVBO_Pos(QOpenGLBuffer::VertexBuffer)
    , mVBO_Nrm(QOpenGLBuffer::VertexBuffer)
    , mVBO_Col(QOpenGLBuffer::VertexBuffer)
    , mGridVBO(QOpenGLBuffer::VertexBuffer)
    , mBBoxVBO(QOpenGLBuffer::VertexBuffer)
{
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    mVAO.destroy();
    mVBO_Pos.destroy();
    mVBO_Nrm.destroy();
    mVBO_Col.destroy();
    mGridVAO.destroy();
    mGridVBO.destroy();
    mBBoxVAO.destroy();
    mBBoxVBO.destroy();
    mShader.removeAllShaders();
    mPickShader.removeAllShaders();
    mPickFBO.reset();
    doneCurrent();
}

// --------- Public API ---------
void OpenGLWidget::drawShape(const std::vector<float>& vec,
    const std::vector<float>& normal,
    const std::vector<float>& perVertexColors)
{
    Mesh m;
    m.vertices = vec;

    // Normals
    if (!normal.empty()) {
        if (normal.size() == vec.size()) {
            m.normals = normal;
        }
        else if (normal.size() * 3 == vec.size()) {
            m.normals = expandTriangleNormals(normal, vec.size());
            if (m.normals.empty()) m.normals = computeFlatNormals(vec);
        }
        else {
            m.normals = computeFlatNormals(vec);
        }
    }
    else {
        m.normals = computeFlatNormals(vec);
    }

    // Colors
    if (!perVertexColors.empty() && perVertexColors.size() == vec.size()) {
        m.colors = perVertexColors;
        m.hasVertexColors = true;
    }
    else {
        m.hasVertexColors = false;
        m.solidColor = mDefaultObjectColor;
    }

    m.material = mDefaultMaterial;

    updateMeshBoundsAndBox(m);

    mMeshes.emplace_back(std::move(m));
    mSelected = int(mMeshes.size()) - 1;
    update();
}

void OpenGLWidget::clearShape()
{
    mMeshes.clear();
    mSelected = -1;
    update();
}

void OpenGLWidget::setGridVisible(bool enabled) { mGridEnabled = enabled; update(); }
void OpenGLWidget::setWireframe(bool enabled) { mWireframe = enabled;   update(); }
void OpenGLWidget::setLightingEnabled(bool e) { mLightingEnabled = e;   update(); }
void OpenGLWidget::setShowBoundingBox(bool e) { mShowBoundingBox = e;   update(); }

void OpenGLWidget::setSceneBrightness(float exposure, float gamma, float emissiveBoost)
{
    mExposure = std::max(0.1f, exposure);
    mGamma = std::max(0.1f, gamma);
    mEmissive = std::max(0.0f, emissiveBoost);
    update();
}

void OpenGLWidget::setColorOfSelected(const QColor& color)
{
    if (mMeshes.empty()) return;
    int idx = (mSelected >= 0) ? mSelected : int(mMeshes.size()) - 1;
    mMeshes[idx].solidColor = toVec3(color);
    update();
}

void OpenGLWidget::setRandomVertexColorsForSelected()
{
    if (mMeshes.empty()) return;
    int idx = (mSelected >= 0) ? mSelected : int(mMeshes.size()) - 1;

    auto& mesh = mMeshes[idx];
    mesh.colors.resize(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
        float r = frand(0.45f, 1.0f);
        float g = frand(0.45f, 1.0f);
        float b = frand(0.45f, 1.0f);
        mesh.colors[i] = r;
        mesh.colors[i + 1] = g;
        mesh.colors[i + 2] = b;
    }
    mesh.hasVertexColors = true;
    update();
}

void OpenGLWidget::clearVertexColorsForSelected()
{
    if (mMeshes.empty()) return;
    int idx = (mSelected >= 0) ? mSelected : int(mMeshes.size()) - 1;

    auto& mesh = mMeshes[idx];
    mesh.colors.clear();
    mesh.hasVertexColors = false;
    update();
}

void OpenGLWidget::setMaterialForSelected(const QColor& ka,
    const QColor& kd,
    const QColor& ks,
    float shininess)
{
    if (mMeshes.empty()) return;
    int idx = (mSelected >= 0) ? mSelected : int(mMeshes.size()) - 1;
    auto& mat = mMeshes[idx].material;
    mat.Ka = toVec3(ka);
    mat.Kd = toVec3(kd);
    mat.Ks = toVec3(ks);
    mat.shininess = std::max(1.0f, shininess);
    update();
}

// --------- GL lifecycle ---------
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.15f, 0.15f, 0.18f, 1.0f); // slightly brighter bg

    // === Vertex shader: world-space normal, pass color ===
    const char* vs = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNorm;
        layout(location = 2) in vec3 aCol;  // optional

        uniform mat4 uModel, uView, uProj;
        uniform bool uUseVtxColor;

        out vec3 vPosW;
        out vec3 vNormW;
        out vec3 vColor;

        void main(){
            mat3 normalMat = mat3(transpose(inverse(uModel)));

            vec4 pw = uModel * vec4(aPos, 1.0);
            gl_Position = uProj * uView * pw;
            vPosW = pw.xyz;

            vNormW = normalize(normalMat * aNorm);

            vColor = uUseVtxColor ? aCol : vec3(1.0);
        }
    )";

    // === Fragment shader: sun-like light (world space), camera-based Blinn-Phong ===
    const char* fs = R"(
        #version 330 core
        in vec3 vPosW;
        in vec3 vNormW;
        in vec3 vColor;

        out vec4 FragColor;

        uniform vec3  uLightDir;        // world-space sun direction (points FROM light to scene)
        uniform bool  uLightingEnabled;

        uniform vec3  uKa;
        uniform vec3  uKd;
        uniform vec3  uKs;
        uniform float uShininess;

        uniform vec3  uSolidColor;

        uniform float uExposure;
        uniform float uGamma;
        uniform float uEmissive;

        uniform vec3  uCameraPos;       // world-space camera position

        void main(){
            vec3 N = normalize(vNormW);
            vec3 L = normalize(-uLightDir);
            vec3 baseColor = vColor * uSolidColor;

            vec3 lit;
            if(!uLightingEnabled){
                lit = baseColor;
            } else {
                float diff = max(dot(N, L), 0.0);

                // Blinn-Phong: use true camera vector so highlight moves with rotation
                vec3 V = normalize(uCameraPos - vPosW);
                vec3 H = normalize(L + V);
                float spec = pow(max(dot(N, H), 0.0), uShininess);

                lit = uKa * baseColor
                    + uKd * baseColor * diff
                    + uKs * spec;
            }

            // emissive lift
            lit += vec3(uEmissive);

            // exposure + gamma
            lit = vec3(1.0) - exp(-lit * uExposure);
            lit = pow(lit, vec3(1.0 / uGamma));

            FragColor = vec4(lit, 1.0);
        }
    )";

    mShader.addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    mShader.addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
    if (!mShader.link())
        qWarning() << "Shader link failed:" << mShader.log();

    // Mesh VAO/VBO
    mVAO.create(); mVAO.bind();
    mVBO_Pos.create(); mVBO_Pos.bind(); mVBO_Pos.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    mVBO_Nrm.create(); mVBO_Nrm.bind(); mVBO_Nrm.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    mVBO_Col.create(); mVBO_Col.bind(); mVBO_Col.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    mVBO_Col.release();
    mVBO_Nrm.release();
    mVBO_Pos.release();
    mVAO.release();

    // Grid
    mGridVAO.create(); mGridVAO.bind();
    mGridVBO.create(); mGridVBO.bind(); mGridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    buildGrid();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    mGridVBO.release(); mGridVAO.release();

    // BBox
    mBBoxVAO.create(); mBBoxVAO.bind();
    mBBoxVBO.create(); mBBoxVBO.bind(); mBBoxVBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    mBBoxVBO.release(); mBBoxVAO.release();

    // Picking shader (flat color)
    const char* pvs = R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        uniform mat4 uModel, uView, uProj;
        void main(){ gl_Position = uProj*uView*uModel*vec4(aPos,1.0); }
    )";
    const char* pfs = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uIDColor;
        void main(){ FragColor = vec4(uIDColor, 1.0); }
    )";
    mPickShader.addShaderFromSourceCode(QOpenGLShader::Vertex, pvs);
    mPickShader.addShaderFromSourceCode(QOpenGLShader::Fragment, pfs);
    if (!mPickShader.link())
        qWarning() << "Pick shader link failed:" << mPickShader.log();
}

void OpenGLWidget::buildGrid(float halfExtent, float step)
{
    std::vector<float> grid;
    for (float v = -halfExtent; v <= halfExtent + 0.001f; v += step) {
        grid.insert(grid.end(), { -halfExtent, v, 0,  halfExtent, v, 0 });
        grid.insert(grid.end(), { v, -halfExtent, 0,  v, halfExtent, 0 });
    }
    mGridVertexCount = int(grid.size() / 3);

    mGridVBO.bind();
    mGridVBO.allocate(grid.data(), int(grid.size() * sizeof(float)));
    mGridVBO.release();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    mProjection.setToIdentity();
    mProjection.perspective(45.0f, float(w) / float(h), 0.1f, 2000.0f);
    ensurePickFBO();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, mWireframe ? GL_LINE : GL_FILL);

    if (mGridEnabled) drawGrid();
    drawAllMeshes();
    if (mShowBoundingBox && mSelected >= 0) drawSelectedBBox();
}

void OpenGLWidget::drawGrid()
{
    QMatrix4x4 model; model.setToIdentity();
    QMatrix4x4 view;  view.setToIdentity(); view.translate(0, 0, -mZoom);

    mShader.bind();
    mShader.setUniformValue("uModel", model);
    mShader.setUniformValue("uView", view);
    mShader.setUniformValue("uProj", mProjection);
    mShader.setUniformValue("uLightDir", mLightDir);
    mShader.setUniformValue("uLightingEnabled", false);
    mShader.setUniformValue("uSolidColor", QVector3D(0.5f, 0.5f, 0.5f));
    mShader.setUniformValue("uKa", QVector3D(1, 1, 1));
    mShader.setUniformValue("uKd", QVector3D(1, 1, 1));
    mShader.setUniformValue("uKs", QVector3D(0, 0, 0));
    mShader.setUniformValue("uShininess", 1.0f);
    mShader.setUniformValue("uUseVtxColor", false);
    mShader.setUniformValue("uExposure", mExposure);
    mShader.setUniformValue("uGamma", mGamma);
    mShader.setUniformValue("uEmissive", 0.0f);
    // Camera position in world space
    mShader.setUniformValue("uCameraPos", QVector3D(0.0f, 0.0f, mZoom));

    mGridVAO.bind();
    glDrawArrays(GL_LINES, 0, mGridVertexCount);
    mGridVAO.release();
    mShader.release();
}

void OpenGLWidget::drawAllMeshes()
{
    if (mMeshes.empty()) return;

    QMatrix4x4 model; model.setToIdentity();
    model.rotate(mRotationX, 1, 0, 0);
    model.rotate(mRotationY, 0, 1, 0);
    model.rotate(mRotationZ, 0, 0, 1);

    QMatrix4x4 view; view.setToIdentity(); view.translate(0, 0, -mZoom);

    mShader.bind();
    mShader.setUniformValue("uModel", model);
    mShader.setUniformValue("uView", view);
    mShader.setUniformValue("uProj", mProjection);
    mShader.setUniformValue("uLightDir", mLightDir);
    mShader.setUniformValue("uLightingEnabled", mLightingEnabled);
    mShader.setUniformValue("uExposure", mExposure);
    mShader.setUniformValue("uGamma", mGamma);
    // Camera position in world space
    mShader.setUniformValue("uCameraPos", QVector3D(0.0f, 0.0f, mZoom));

    mVAO.bind();
    for (const auto& mesh : mMeshes)
    {
        // Positions
        mVBO_Pos.bind();
        mVBO_Pos.allocate(mesh.vertices.data(), int(mesh.vertices.size() * sizeof(float)));

        // Normals (regen if mismatch)
        const std::vector<float>* normals = &mesh.normals;
        std::vector<float> generated;
        if (mesh.normals.size() != mesh.vertices.size()) {
            generated = computeFlatNormals(mesh.vertices);
            normals = &generated;
        }
        mVBO_Nrm.bind();
        mVBO_Nrm.allocate(normals->data(), int(normals->size() * sizeof(float)));

        // Colors
        const bool useVtx = mesh.hasVertexColors && mesh.colors.size() == mesh.vertices.size();
        if (useVtx) {
            glEnableVertexAttribArray(2);
            mVBO_Col.bind();
            mVBO_Col.allocate(mesh.colors.data(), int(mesh.colors.size() * sizeof(float)));
        }
        else {
            glDisableVertexAttribArray(2);
        }

        // Material + color + emissive
        mShader.setUniformValue("uUseVtxColor", useVtx);
        mShader.setUniformValue("uSolidColor", mesh.solidColor);
        mShader.setUniformValue("uKa", mesh.material.Ka);
        mShader.setUniformValue("uKd", mesh.material.Kd);
        mShader.setUniformValue("uKs", mesh.material.Ks);
        mShader.setUniformValue("uShininess", mesh.material.shininess);
        mShader.setUniformValue("uEmissive", mEmissive);

        glDrawArrays(GL_TRIANGLES, 0, int(mesh.vertices.size() / 3));
    }
    glEnableVertexAttribArray(2);
    mVAO.release();
    mShader.release();
}

void OpenGLWidget::drawSelectedBBox()
{
    if (mSelected < 0 || mSelected >= int(mMeshes.size())) return;
    const auto& mesh = mMeshes[mSelected];
    if (mesh.bboxLines.empty()) return;

    QMatrix4x4 model; model.setToIdentity();
    model.rotate(mRotationX, 1, 0, 0);
    model.rotate(mRotationY, 0, 1, 0);
    model.rotate(mRotationZ, 0, 0, 1);
    QMatrix4x4 view; view.setToIdentity(); view.translate(0, 0, -mZoom);

    mShader.bind();
    mShader.setUniformValue("uModel", model);
    mShader.setUniformValue("uView", view);
    mShader.setUniformValue("uProj", mProjection);
    mShader.setUniformValue("uLightDir", mLightDir);
    mShader.setUniformValue("uLightingEnabled", false);
    mShader.setUniformValue("uUseVtxColor", false);
    mShader.setUniformValue("uSolidColor", QVector3D(1.0f, 0.92f, 0.2f));
    mShader.setUniformValue("uKa", QVector3D(1, 1, 1));
    mShader.setUniformValue("uKd", QVector3D(1, 1, 1));
    mShader.setUniformValue("uKs", QVector3D(0, 0, 0));
    mShader.setUniformValue("uShininess", 1.0f);
    mShader.setUniformValue("uExposure", mExposure);
    mShader.setUniformValue("uGamma", mGamma);
    mShader.setUniformValue("uEmissive", 0.0f);
    // Camera position in world space
    mShader.setUniformValue("uCameraPos", QVector3D(0.0f, 0.0f, mZoom));

    mBBoxVAO.bind();
    mBBoxVBO.bind();
    mBBoxVBO.allocate(mesh.bboxLines.data(), int(mesh.bboxLines.size() * sizeof(float)));
    glDrawArrays(GL_LINES, 0, int(mesh.bboxLines.size() / 3));
    mBBoxVBO.release();
    mBBoxVAO.release();
    mShader.release();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    mLastMousePos = event->pos();
    if (event->button() == Qt::RightButton) {
        int idx = pickMeshAt(event->pos());
        if (idx >= 0) { mSelected = idx; update(); }
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->pos().x() - mLastMousePos.x();
    int dy = event->pos().y() - mLastMousePos.y();
    if (event->buttons() & Qt::LeftButton) {
        mRotationX += dy * 0.5f;
        mRotationY += dx * 0.5f;
        update();
    }
    mLastMousePos = event->pos();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    float zoomFactor = 1.1f;
    mZoom = (event->angleDelta().y() > 0) ? (mZoom / zoomFactor) : (mZoom * zoomFactor);
    if (mZoom < 1.0f)   mZoom = 1.0f;
    if (mZoom > 2000.0f) mZoom = 2000.0f;
    update();
}

// --------- Picking ----------
static QVector3D indexToColor(int idx)
{
    int r = (idx & 0x000000FF);
    int g = (idx & 0x0000FF00) >> 8;
    int b = (idx & 0x00FF0000) >> 16;
    return QVector3D(r / 255.0f, g / 255.0f, b / 255.0f);
}
static int colorToIndex(const unsigned char* rgb)
{
    int r = rgb[0], g = rgb[1], b = rgb[2];
    return (r) | (g << 8) | (b << 16);
}

void OpenGLWidget::ensurePickFBO()
{
    QSize sz = size() * devicePixelRatioF();
    if (sz.isEmpty()) return;
    if (mPickFBO && mPickFBO->size() == sz) return;

    QOpenGLFramebufferObjectFormat fmt;
    fmt.setAttachment(QOpenGLFramebufferObject::Depth);
    fmt.setTextureTarget(GL_TEXTURE_2D);
    fmt.setInternalTextureFormat(GL_RGBA8);
    mPickFBO.reset(new QOpenGLFramebufferObject(sz, fmt));
}

int OpenGLWidget::pickMeshAt(const QPoint& pos)
{
    if (mMeshes.empty()) return -1;
    ensurePickFBO();
    if (!mPickFBO) return -1;

    mPickFBO->bind();
    glViewport(0, 0, mPickFBO->size().width(), mPickFBO->size().height());
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model; model.setToIdentity();
    model.rotate(mRotationX, 1, 0, 0);
    model.rotate(mRotationY, 0, 1, 0);
    model.rotate(mRotationZ, 0, 0, 1);
    QMatrix4x4 view; view.setToIdentity(); view.translate(0, 0, -mZoom);

    mPickShader.bind();
    mPickShader.setUniformValue("uModel", model);
    mPickShader.setUniformValue("uView", view);
    mPickShader.setUniformValue("uProj", mProjection);

    mVAO.bind();
    for (int i = 0;i<int(mMeshes.size());++i) {
        const auto& mesh = mMeshes[i];
        mVBO_Pos.bind();
        mVBO_Pos.allocate(mesh.vertices.data(), int(mesh.vertices.size() * sizeof(float)));
        QVector3D idc = indexToColor(i);
        mPickShader.setUniformValue("uIDColor", idc);
        glDrawArrays(GL_TRIANGLES, 0, int(mesh.vertices.size() / 3));
    }
    mVAO.release();
    mPickShader.release();

    QPointF dpr = QPointF(devicePixelRatioF(), devicePixelRatioF());
    QPoint ip = QPoint(int(pos.x() * dpr.x()), int((height() - pos.y()) * dpr.y())); // flip Y
    unsigned char rgb[4] = { 0,0,0,0 };
    glReadPixels(ip.x(), ip.y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgb);

    mPickFBO->release();

    int idx = colorToIndex(rgb);
    if (idx >= 0 && idx < int(mMeshes.size())) return idx;
    return -1;
}

// --------- Utilities ----------
void OpenGLWidget::updateMeshBoundsAndBox(Mesh& m)
{
    if (m.vertices.empty()) return;
    QVector3D mn(std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max());
    QVector3D mx(-std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max());
    for (size_t i = 0;i < m.vertices.size(); i += 3) {
        mn.setX(std::min(mn.x(), m.vertices[i + 0]));
        mn.setY(std::min(mn.y(), m.vertices[i + 1]));
        mn.setZ(std::min(mn.z(), m.vertices[i + 2]));
        mx.setX(std::max(mx.x(), m.vertices[i + 0]));
        mx.setY(std::max(mx.y(), m.vertices[i + 1]));
        mx.setZ(std::max(mx.z(), m.vertices[i + 2]));
    }
    m.aabbMin = mn; m.aabbMax = mx;

    float x0 = mn.x(), y0 = mn.y(), z0 = mn.z();
    float x1 = mx.x(), y1 = mx.y(), z1 = mx.z();
    std::vector<float> b = {
        // bottom
        x0,y0,z0, x1,y0,z0,  x1,y0,z0, x1,y1,z0,
        x1,y1,z0, x0,y1,z0,  x0,y1,z0, x0,y0,z0,
        // top
        x0,y0,z1, x1,y0,z1,  x1,y0,z1, x1,y1,z1,
        x1,y1,z1, x0,y1,z1,  x0,y1,z1, x0,y0,z1,
        // verticals
        x0,y0,z0, x0,y0,z1,  x1,y0,z0, x1,y0,z1,
        x1,y1,z0, x1,y1,z1,  x0,y1,z0, x0,y1,z1
    };
    m.bboxLines.swap(b);
}
