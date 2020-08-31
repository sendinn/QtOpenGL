#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QOpenGLExtraFunctions>
class Widget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Widget(QOpenGLWidget *parent = 0);
    ~Widget();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawarrow(GLdouble x0, GLdouble y0, GLdouble z0, GLdouble x1, GLdouble y1, GLdouble z1);
};

