#pragma once
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QTimer>
#include <QWheelEvent>
#include "MyShader.h"
#include "Camera.h"
class PaintingWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	PaintingWidget(QWidget *partent);
	~PaintingWidget();

	//继承QOpenGLWidget必须重写的3个函数
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);


	
public:
	void setColor(GLfloat r, GLfloat g, GLfloat b);
	void fillColorBuffer();

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *keyEvent);

private:
	QOpenGLBuffer *m_vbo;             // 存储点数据
	QOpenGLVertexArrayObject *m_vao;  // VAO对象
	QOpenGLBuffer *m_cbo;

	MyShader	*m_shader;   // 渲染器程序对象

	GLfloat colorBuffer[4 * 3 * 3];
	GLfloat vertexData[4 * 3 * 3];

	float aspectRatio;

	QPoint m_OldPoint;
	QPoint m_NewPoint;

	

	QTimer timer;
	Camera m_Camera;
};

