#include "myglwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>
#include <QKeyEvent>

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	translate = -6.0;
	xRot = zRot = 0.0;
	yRot = -30.0;
}

void MyGLWidget::initializeGL() {
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 6; ++i) {
		textures[i] = new QOpenGLTexture(QImage(
			QString(":/img/side%1.png").arg(i + 1)).mirrored());
	}
	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
		"#version 130\n"
		"in vec4 vPosition;                         \n"
		"in vec2 vTexCoord;                         \n"
		"out vec2 texCoord;                         \n"
		"uniform mat4 matrix;                       \n"
		"void main() {                              \n"
		"    texCoord = vTexCoord;                  \n"
		"    gl_Position = matrix * vPosition;      \n"
		"}\n";
	vshader->compileSourceCode(vsrc);
	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc =
		"#version 130\n"
		"uniform sampler2D tex;                     \n"
		"in vec2 texCoord;                          \n"
		"out vec4 fColor;                           \n"
		"void main() {                              \n"
		"    fColor = texture(tex, texCoord);       \n"
		"}\n";
	fshader->compileSourceCode(fsrc);

	program = new QOpenGLShaderProgram;
	program->addShader(vshader);
	program->addShader(fshader);
	program->link();
	program->bind();
}

void MyGLWidget::paintGL() {
	int w = width();
	int h = height();
	int side = qMin(w, h);
	glViewport((w - side) / 2, (h - side) / 2, side, side);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat vectices[6][4][3] =
	{
		{{1.0f,1.0f,1.0f},{1.0f,1.0f,-1.0f},{1.0f,-1.0f,-1.0f},{1.0f,-1.0f,1.0f}},
		{{-1.0f,1.0f,1.0f},{-1.0f,1.0f,-1.0f},{-1.0f,-1.0f,-1.0f},{-1.0f,-1.0f,1.0f}},
		{{1.0f,1.0f,1.0f},{-1.0f,1.0f,1.0f},{-1.0f,1.0f,-1.0f},{1.0f,1.0f,-1.0f}},
		{{1.0f,-1.0f,1.0f},{-1.0f,-1.0f,1.0f},{-1.0f,-1.0f,-1.0f},{1.0f,-1.0f,-1.0f}},
		{{1.0f,1.0f,1.0f},{1.0f,-1.0f,1.0f},{-1.0f,-1.0f,1.0f},{-1.0f,1.0f,1.0f}},
		{{1.0f,1.0f,-1.0f},{1.0f,-1.0f,-1.0f},{-1.0f,-1.0f,-1.0f},{-1.0f,1.0f,-1.0f}},
	};

	GLfloat coords[6][4][2] =
	{
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} },
	};

	vbo.create();
	vbo.bind();
	vbo.allocate((72 + 48) * sizeof(GLfloat));


	vbo.write(0, vectices, 72 * sizeof(GLfloat));
	GLuint vPosition = program->attributeLocation("vPosition");
	program->setAttributeBuffer(vPosition, GL_FLOAT, 0, 3, 0);
	glEnableVertexAttribArray(vPosition);

	vbo.write(72 * sizeof(GLfloat), coords, 48 * sizeof(GLfloat));
	GLuint vTexCoord = program->attributeLocation("vTexCoord");
	program->setAttributeBuffer(vTexCoord, GL_FLOAT, 72 * sizeof(GLfloat), 2, 0);
	glEnableVertexAttribArray(vTexCoord);
	program->setUniformValue("tex", 0);

	QMatrix4x4 matrix;
	matrix.perspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	matrix.translate(0, 0, translate);
	matrix.rotate(xRot, 1, 0, 0);
	matrix.rotate(yRot, 0, 1, 0);
	matrix.rotate(zRot, 0, 0, 1);
	program->setUniformValue("matrix", matrix);

	for (int i = 0; i < 6; i++) {
		textures[i]->bind();
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}
}

void MyGLWidget::resizeGL(int w, int h) {

}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		xRot += 10;
		break;
	case Qt::Key_Left:
		yRot += 10;
		break;
	case Qt::Key_Right:
		zRot += 10;
		break;
	case Qt::Key_Down:
		translate -= 1;
		break;
	case Qt::Key_Space:
		translate += 1;
		break;
	default:
		break;
	}
	update();
	QOpenGLWidget::keyPressEvent(event);
}