#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class QOpenGLTexture;
class QOpenGLShaderProgram;

class MyGLWidget :public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	MyGLWidget(QWidget *parent = 0);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void keyPressEvent(QKeyEvent *evnet);

private:
	QOpenGLShaderProgram *program;
	QOpenGLBuffer vbo;
	QOpenGLTexture *textures[6];
	GLfloat translate, xRot, yRot, zRot;
};