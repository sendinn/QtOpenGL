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
#include "Mesh.h"
#include "Model.h"
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

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *keyEvent);

private:
	QOpenGLBuffer *m_vbo;             // 存储点数据
	QOpenGLVertexArrayObject *m_vao;  // VAO对象
	QOpenGLBuffer *m_cbo;

	float aspectRatio;

	QPoint m_OldPoint;
	QPoint m_NewPoint;

	

	QTimer timer;
	Camera m_Camera;

	QMatrix4x4 m_Projection;
	QMatrix4x4 m_Model;
	QMatrix4x4 m_View;
	QQuaternion m_Rotate;

	QVector3D m_Offset;
	QVector3D m_Center;

	MyShader* m_Shader;
	//Mesh* m_Mesh;
	QVector<Mesh*> m_Meshes;

};

