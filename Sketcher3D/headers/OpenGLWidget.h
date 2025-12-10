#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QMatrix4x4>
#include <QPoint>
#include <QColor>
#include <vector>
#include <memory>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    // Shapes input
    void drawShape(const std::vector<float>& vec,
        const std::vector<float>& normal = {},
        const std::vector<float>& perVertexColors = {});
    void clearShape();

    // Scene properties
    void setGridVisible(bool enabled);
    void setWireframe(bool enabled);
    void setLightingEnabled(bool enabled);
    void setShowBoundingBox(bool enabled);

    // Brightness controls (exposure, gamma, emissive)
    void setSceneBrightness(float exposure, float gamma, float emissiveBoost);

    // Selection / editing
    void setColorOfSelected(const QColor& color);
    void setRandomVertexColorsForSelected();
    void clearVertexColorsForSelected();

    // Materials for selected mesh
    void setMaterialForSelected(const QColor& ka,
        const QColor& kd,
        const QColor& ks,
        float shininess);

    // Sun light direction (world space)
    void setSunDirection(const QVector3D& dir) {
        QVector3D d = dir;
        if (!d.isNull()) d.normalize();
        mLightDir = d;
        update();
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    struct Material {
        QVector3D Ka{ 0.35f, 0.35f, 0.35f };  // ambient (brighter)
        QVector3D Kd{ 1.15f, 1.15f, 1.15f }; // diffuse (bright)
        QVector3D Ks{ 0.45f, 0.45f, 0.45f }; // specular
        float     shininess{ 32.0f };
    };

    class Mesh {
    public:
        std::vector<float> vertices;   // xyz...
        std::vector<float> normals;    // nx ny nz... (same count as vertices)
        std::vector<float> colors;     // optional r g b...
        bool hasVertexColors{ false };

        QVector3D solidColor{ 0.15f, 0.75f, 1.0f };
        Material  material;

        QVector3D aabbMin, aabbMax;
        std::vector<float> bboxLines;  // GL_LINES positions only
    };

    std::vector<Mesh> mMeshes;
    int mSelected{ -1 };

    // GL
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer            mVBO_Pos;
    QOpenGLBuffer            mVBO_Nrm;
    QOpenGLBuffer            mVBO_Col;
    QOpenGLShaderProgram     mShader;

    // Grid
    bool                     mGridEnabled{ false };
    QOpenGLVertexArrayObject mGridVAO;
    QOpenGLBuffer            mGridVBO;
    int                      mGridVertexCount{ 0 };

    // BBox
    bool                     mShowBoundingBox{ true };
    QOpenGLVertexArrayObject mBBoxVAO;
    QOpenGLBuffer            mBBoxVBO;

    // Picking
    std::unique_ptr<QOpenGLFramebufferObject> mPickFBO;
    QOpenGLShaderProgram     mPickShader;

    // Camera
    QMatrix4x4 mProjection;
    QPoint     mLastMousePos;
    float      mRotationX{ 0.0f };
    float      mRotationY{ 0.0f };
    float      mRotationZ{ 0.0f };
    float      mZoom{ 500.0f };

    // Lighting + wireframe
    QVector3D  mLightDir{ 0.0f, -0.7f, 1.0f }; // world-space "sun" direction
    bool       mLightingEnabled{ true };
    bool       mWireframe{ false };

    // Brightness controls
    float mExposure{ 1.4f };
    float mGamma{ 2.0f };
    float mEmissive{ 0.08f };

    // Defaults for new meshes
    QVector3D mDefaultObjectColor{ 0.15f, 0.75f, 1.0f };
    Material  mDefaultMaterial{};

private:
    static QVector3D toVec3(const QColor& c) {
        return QVector3D(c.redF(), c.greenF(), c.blueF());
    }

    // Data prep helpers
    static std::vector<float> computeFlatNormals(const std::vector<float>& vertices);
    static std::vector<float> expandTriangleNormals(const std::vector<float>& triNormals,
        size_t vertexCount);

    void buildGrid(float halfExtent = 200.0f, float step = 10.0f);
    void updateMeshBoundsAndBox(Mesh& m);
    void ensurePickFBO();
    int  pickMeshAt(const QPoint& pos); // returns mesh index or -1

    // Drawing
    void drawAllMeshes();
    void drawGrid();
    void drawSelectedBBox();
};
