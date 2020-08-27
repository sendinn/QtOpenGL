#include "PaintingWidget.h"
#include <qmath.h>
PaintingWidget::PaintingWidget(QWidget* parent) :
	QOpenGLWidget(parent), 
	m_vbo(nullptr), 
	m_vao(nullptr), 
	m_shader(nullptr), 
	camera(0.0f, 3.0f, 0.0f),
	world(0.0f,0.0f,0.0f),
	camera_Up(0.0f,0.0f,1.0f),
	AuxY(0.0f,1.0f,0.0f),
	m_RotateAsix(0.0f, 0.0f, 1.0f)
{

	const GLfloat VERTEX_INIT_DATA[] = {
		//face 1
		-0.5f, 0.0f, -0.2887f,
		0.5f, 0.0f, -0.2887f,
		0.0f, 0.0f, 0.5774f,
		//face 2
		-0.5f, 0.0f, -0.2887f,
		0.5f, 0.0f, -0.2887f,
		0.0f, 0.8165f, 0.0f,
		//face 3
		-0.5f, 0.0f, -0.2887f,
		0.0f, 0.0f, 0.5774f,
		0.0f, 0.8165f, 0.0f,
		//face 4
		0.5f, 0.0f, -0.2887f,
		0.0f, 0.0f, 0.5774f,
		0.0f, 0.8165f, 0.0f,
	};
	const GLfloat COLOR_INIT_DATA[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};
	memcpy(this->vertexData, VERTEX_INIT_DATA, sizeof(this->vertexData));
	memcpy(this->colorBuffer, COLOR_INIT_DATA, sizeof(this->colorBuffer));

	setFocusPolicy(Qt::StrongFocus);

	m_zoom = 1;

	AuxZ = camera - world;
	AuxX = QVector3D::crossProduct(AuxY, AuxZ);
	AuxX.normalize();
}
PaintingWidget::~PaintingWidget() {

}


/*
初始化 Shader Program
初始化顶点缓冲并装入三角形点位置坐标
*/
void PaintingWidget::initializeGL()
{
	m_shader = new MyShader("VertexSource", "FragmentSource");

	QOpenGLFunctions *f = this->context()->functions();
	f->glEnable(GL_DEPTH_TEST);   // 三维绘图的关键！

	m_vao = new QOpenGLVertexArrayObject();
	m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_cbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_vao->create();
	m_vao->bind();
	m_vbo->create();
	m_vbo->bind();
	m_vbo->allocate(this->vertexData, 4 * 3 * 3 * sizeof(GLfloat));
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	m_vbo->release();
	m_cbo->create();
	m_cbo->bind();
	m_cbo->allocate(this->colorBuffer, 4 * 3 * 3 * sizeof(GLfloat));
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	m_cbo->release();
	m_vao->release();

}




void PaintingWidget::paintGL()
{
	QOpenGLFunctions *f = this->context()->functions();
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	f->glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
	m_vao->bind();
	m_shader->GetShader()->bind();
	QMatrix4x4 mvp;
	mvp.perspective(45.0f, this->aspectRatio, 0.1f, 100.0f);
	mvp.lookAt(camera, world, camera_Up);

	mvp.rotate(10.0f, m_RotateAsix);
	m_shader->GetShader()->setUniformValue(m_shader->GetShader()->uniformLocation("MVP"), mvp);
	f->glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
	m_shader->GetShader()->release();
	m_vao->release();

	update();
}

void PaintingWidget::resizeGL(int w, int h)
{
	aspectRatio = (float)w / h;
}


void PaintingWidget::fillColorBuffer()
{
	GLfloat colorData[3 * 3];
	for (int i = 0; i < 3; ++i) {
		memcpy(&colorData[i * 3], this->colorBuffer, 3 * sizeof(GLfloat));
	}
	m_cbo->write(0, colorData, 3 * 3 * sizeof(GLfloat));
}

void PaintingWidget::mouseMoveEvent(QMouseEvent *event)
{
	float dx = event->x() - m_OldPoint.x();
	float dy = event->y() - m_OldPoint.y();

	if (event->buttons() & Qt::LeftButton)
	{
		QVector3D MouseTrace = AuxY*dy + AuxX* dx;
		m_RotateAsix = QVector3D::crossProduct(MouseTrace, AuxZ);
		m_RotateAsix.normalize();

		float angle = MouseTrace.length();
		QMatrix4x4 m;
		m.rotate(angle, m_RotateAsix);

		camera = m * camera;
		camera_Up = m * camera_Up;
		camera_Up.normalize();
		AuxY = camera_Up;
		AuxZ = camera - world;
		AuxX = QVector3D::crossProduct(AuxY, AuxZ);
		AuxX.normalize();
	}
	m_OldPoint = event->pos();
}

void PaintingWidget::mousePressEvent(QMouseEvent *event)
{
	m_OldPoint = event->pos();
}

void PaintingWidget::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)
	{
		m_zoom = 1.1;
	}
	else
	{
		m_zoom = 0.9;
	}

	update();
}

void PaintingWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	switch (keyEvent->key()) {
	case Qt::Key_Right:
		camera.setZ(camera.z() + 0.1f);
		break;
	case Qt::Key_Left:
		camera.setZ(camera.z() - 0.1f);
		break;
	case Qt::Key_Up:
		camera.setX(camera.x() + 0.1f);
		break;
	case Qt::Key_Down:
		camera.setX(camera.x() - 0.1f);
		break;
	case Qt::Key_Plus:
		camera.setY(camera.y() + 0.1f);
		break;
	case Qt::Key_Minus:
		camera.setY(camera.y() - 0.1f);
		break;
	}
	update();
}

void PaintingWidget::setColor(GLfloat r, GLfloat g, GLfloat b)
{
	colorBuffer[0] = r;
	colorBuffer[1] = g;
	colorBuffer[2] = b;
	m_cbo->bind();
	fillColorBuffer();
	m_cbo->release();
	update();
}