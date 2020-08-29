#include "PaintingWidget.h"
#include <qmath.h>
#include <QTime>
PaintingWidget::PaintingWidget(QWidget* parent) :
	QOpenGLWidget(parent), 
	m_vbo(nullptr), 
	m_vao(nullptr), 
	m_shader(nullptr)
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
		1.0f, 1.0f, 0.0f,
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
	const GLfloat VERTEX_INIT_DATA2[] = {
		-0.5f, 0, 0,
		0.5f, 0, 0,
		0,0.7,0
	};
	memcpy(this->vertexData, VERTEX_INIT_DATA2, sizeof(this->vertexData));
	memcpy(this->colorBuffer, COLOR_INIT_DATA, sizeof(this->colorBuffer));

	setFocusPolicy(Qt::StrongFocus);



	QSurfaceFormat format;
	format.setAlphaBufferSize(24);  //����alpha�����С
	format.setVersion(3, 3);         //���ð汾��
	format.setSamples(10);          //�����ز������������ڷ�����

	timer.setInterval(18);
	//connect(&timer, &QTimer::timeout, this, static_cast<void (PaintingWidget::*)()>(&PaintingWidget::update));
	timer.start();

}
PaintingWidget::~PaintingWidget() {

}


/*
��ʼ�� Shader Program
��ʼ�����㻺�岢װ�������ε�λ������
*/
void PaintingWidget::initializeGL()
{
	initializeOpenGLFunctions();//��ʼ��opengl����
	//QOpenGLFunctions *f = this->context()->functions();

	m_shader = new MyShader("VertexSource.txt", "FragmentSource.txt");
	
	glEnable(GL_DEPTH_TEST);   // ��ά��ͼ�Ĺؼ���

	m_vao = new QOpenGLVertexArrayObject();
	m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_cbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	QOpenGLVertexArrayObject::Binder{ m_vao };
	//m_vao->create();
	//m_vao->bind();
	m_vbo->create();//����VBO����
	m_vbo->bind();//��VBO�󶨵���ǰ�Ķ��㻺�����QOpenGLBuffer::VertexBuffer����
	m_vbo->allocate(this->vertexData, 4 * 3 * 3 * sizeof(GLfloat));//���������ݷ��䵽VBO�У���һ������Ϊ����ָ�룬�ڶ�������Ϊ���ݵ��ֽڳ���

	/*
	��һ������ָ������Ҫ���õĶ������ԡ�������ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	�ڶ�������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
	������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0��
	���ĸ�����ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(GLfloat)��
	*/
	m_shader->GetShader()->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
	//f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	m_shader->GetShader()->enableAttributeArray(0);
	//f->glEnableVertexAttribArray(0);

	m_vbo->release();
	m_cbo->create();
	m_cbo->bind();
	m_cbo->allocate(this->colorBuffer, 4 * 3 * 3 * sizeof(GLfloat));

	m_shader->GetShader()->setAttributeBuffer(1, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
	//f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	m_shader->GetShader()->enableAttributeArray(1);
	//f->glEnableVertexAttribArray(1);
	m_cbo->release();
	//m_vao->release(); //���VAO����ȫʹ��

}




void PaintingWidget::paintGL()
{
	//��̳�QOpenGLExtraFunctions��������
	//QOpenGLFunctions *f = this->context()->functions();
	
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);//����������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����


	m_shader->GetShader()->bind();
	{

		/*
		VAO�Ĵ����������ǲ����е��������߳�����c++��׼�����ṩ��lock_guard���������乹�캯���м������������������н�������QOpenGLVertexArrayObjectҲ�ṩ�����ƵĻ��ƣ�
		QOpenGLVertexArrayObject::Binder����һ�����ڹ���󶨵Ķ���
		*/
		QOpenGLVertexArrayObject::Binder{ m_vao }; //��VAO��������ʱ���������뿪�������Զ���� ==������vao��creat bind���Լ�֮���release  
		//m_vao->bind();

		QMatrix4x4 model;
		m_shader->GetShader()->setUniformValue("model", model);

		QMatrix4x4 view;
		view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.m_CameraUp);
		m_shader->GetShader()->setUniformValue("view", view);

		QMatrix4x4 projection;
		//͸��ͶӰ
		projection.perspective(45.0f, width() / (float)height(), 0.1f, 10.0f);
		m_shader->GetShader()->setUniformValue("projection", projection);

#define test3

#ifdef test1
		float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
		QMatrix4x4 trans;
		trans.translate(0.0f, 0.5*qAbs(qSin(time)), 0.0f);        //��y��ƽ��0.5*[0,1]
		trans.scale(0.5*qAbs(qSin(time)), 0.5*qAbs(qSin(time))); //x��y��[0,0.5]��������
		trans.rotate(360 * time, 0.0f, 0.0f, -1.0f);                 //��ת360*time
		m_shader->GetShader()->setUniformValue(m_shader->GetShader()->uniformLocation("MVP"), trans);
#endif


#ifdef test2
		float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
		QMatrix4x4 view;
		float radius = 10.0f;
		/*
		m_CameraPos + QVector3D ��ʾ��+����ķ���ʼ�տ�����ǰ��
		view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + QVector3D(0,0,-1), m_Camera.GetCameraUp());
		m_CameraPos + Direction ��ʾ�۲�㣬ʼ�տ���ͬһ����
		view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos - (m_Camera.m_CameraPos - QVector3D(0,0,-1)), m_Camera.GetCameraUp());
		*/

		view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.GetCameraUp());
		//m_shader->GetShader()->setUniformValue("view", view);
#endif

		for (int i = 0; i < 10; ++i)
		{
			model.translate(QVector3D(0, -0.1, 0));
			m_shader->GetShader()->setUniformValue("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}





		//m_vao->release();
	}
	m_shader->GetShader()->release();

	update();
}

void PaintingWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);                //�����ӿ�����

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

	if (event->buttons() & Qt::LeftButton)
	{
		static bool first = true;
		if (first)
		{
			m_OldPoint = event->pos();
			first = false;
		}
		float dx = event->x() - m_OldPoint.x();
		float dy = m_OldPoint.y() - event->y();
		dx *= 0.01;
		dy *= 0.01;
		m_Camera.yaw -= dx;
		m_Camera.pitch -= dy;
		if (m_Camera.pitch >= M_PI / 2)                          //�����ӽ����Ƶ�[-90��,90��]
			m_Camera.pitch = (M_PI) / 2 - 0.1;
		if (m_Camera.pitch <= -M_PI / 2)
			m_Camera.pitch = -(M_PI) / 2 + 0.1;

		m_Camera.m_CameraFront.setY(sin(m_Camera.pitch));
		m_Camera.m_CameraFront.setX(cos(m_Camera.pitch)*cos(m_Camera.yaw));
		m_Camera.m_CameraFront.setZ(cos(m_Camera.pitch)*sin(m_Camera.yaw));

	}
	if (event->buttons() & Qt::RightButton)
	{
		//model.translate(QVector3D(0.01, 0, 0));
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
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() + 0.1f);
	}
	else
	{
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() - 0.1f);
	}
	update();
}

void PaintingWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	switch (keyEvent->key()) {
	case Qt::Key_Right:
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() - 0.1f);
		break;
	case Qt::Key_Left:
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() + 0.1f);
		break;
	case Qt::Key_Up:
		m_Camera.m_CameraPos.setY(m_Camera.m_CameraPos.y() - 0.1f);
		break;
	case Qt::Key_Down:
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.y() + 0.1f);
		break;
	default:
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