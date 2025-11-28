#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include <QString>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = nullptr);
    ~GLWidget();

    void loadDATFile(const QString& filename);
    void clearShape();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::vector<float> vertices;

    float rotationX;
    float rotationY;
    float zoom;
    QPoint lastMousePos;
};

#endif // GLWIDGET_H