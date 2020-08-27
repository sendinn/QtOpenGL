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
#include <QWheelEvent>
#include "MyShader.h"
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

	QMatrix4x4 projection;
	QMatrix4x4 projection_save;
	QMatrix4x4 projection_use;

	QPoint m_OldPoint;
	QPoint m_NewPoint;
	float m_xMove, m_yMove;
	float m_zoom;

	//旋转后观察点方向与视线向上方向
	QVector3D	camera;
	QVector3D	camera_Up;
	QVector3D   world;
	//辅助坐标系三根轴
	QVector3D	AuxX;
	QVector3D	AuxY;
	QVector3D	AuxZ;
	QVector3D   m_RotateAsix;
};

