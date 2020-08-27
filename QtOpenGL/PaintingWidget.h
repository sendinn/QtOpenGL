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

	//�̳�QOpenGLWidget������д��3������
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
	QOpenGLBuffer *m_vbo;             // �洢������
	QOpenGLVertexArrayObject *m_vao;  // VAO����
	QOpenGLBuffer *m_cbo;

	MyShader	*m_shader;   // ��Ⱦ���������

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

	//��ת��۲�㷽�����������Ϸ���
	QVector3D	camera;
	QVector3D	camera_Up;
	QVector3D   world;
	//��������ϵ������
	QVector3D	AuxX;
	QVector3D	AuxY;
	QVector3D	AuxZ;
	QVector3D   m_RotateAsix;
};

