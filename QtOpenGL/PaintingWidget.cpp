#include "PaintingWidget.h"
#include <qmath.h>
#include <QTime>


GLfloat VERTEX_INIT_DATA2[] = {
	-0.5f, 0, 0,
	0.5f, 0, 0,
	0,0.7,0
};

GLfloat VERTEX_INIT_DATA[] = {
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

GLfloat COLOR_INIT_DATA[] = {
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

PaintingWidget::PaintingWidget(QWidget* parent) :
	QOpenGLWidget(parent), 
	m_vbo(nullptr), 
	m_vao(nullptr)
{

	setFocusPolicy(Qt::StrongFocus);



	QSurfaceFormat format;
	format.setAlphaBufferSize(24);  //����alpha�����С
	format.setVersion(3, 3);         //���ð汾��
	format.setSamples(10);          //�����ز������������ڷ�����

	timer.setInterval(18);
	//connect(&timer, &QTimer::timeout, this, static_cast<void (PaintingWidget::*)()>(&PaintingWidget::update));
	timer.start();




	setMouseTracking(true);         //�������׷�٣�QtĬ�ϲ���ʵʱ�������ƶ�

	//QCursor::setPos(geometry().center());   //�������λ��Ϊ���ھ������������
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
	glEnable(GL_DEPTH_TEST);   // ��ά��ͼ�Ĺؼ���

// 	m_Mesh = new Mesh();
// 	m_Mesh->SetVertex(VERTEX_INIT_DATA2, 3);
// 	m_Mesh->SetColor(COLOR_INIT_DATA, 12);
// 	m_Mesh->SetupMesh();
	m_Shader = new MyShader("VertexSource.txt", "FragmentSource.txt");

	Model model("D:\\code\\QtOpenGL\\QtOpenGL\\nanosuit\\nanosuit.obj",m_Shader->GetShader());
	m_Meshes = model.GetMeshes();
}




void PaintingWidget::paintGL()
{
	//��̳�QOpenGLExtraFunctions��������
	//QOpenGLFunctions *f = this->context()->functions();
	
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);//����������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����

	m_Shader->GetShader()->bind();
	for (int i = 0; i < m_Meshes.size(); ++i)
	{
		{

			/*
			VAO�Ĵ����������ǲ����е��������߳�����c++��׼�����ṩ��lock_guard���������乹�캯���м������������������н�������QOpenGLVertexArrayObjectҲ�ṩ�����ƵĻ��ƣ�
			QOpenGLVertexArrayObject::Binder����һ�����ڹ���󶨵Ķ���
			*/
			//QOpenGLVertexArrayObject::Binder{ m_vao }; //��VAO��������ʱ���������뿪�������Զ���� ==������vao��creat bind���Լ�֮���release  
			//m_vao->bind();

			m_Model.setToIdentity();
			m_Model.rotate(m_Rotate);
			m_Shader->GetShader()->setUniformValue("model", m_Model);


			m_View.setToIdentity();
			m_View.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.m_CameraUp);
			m_Shader->GetShader()->setUniformValue("view", m_View);

			//͸��ͶӰ
			m_Projection.setToIdentity();
			m_Projection.perspective(45.0f, width() / (float)height(), 0.1f, 10.0f);
			m_Shader->GetShader()->setUniformValue("projection", m_Projection);

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

#define USE_MESH
#ifdef USE_MESH
			m_Meshes[i]->Draw();
#else
			glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

			//m_vao->release();
		}
	}
	m_Shader->GetShader()->release();

	update();
}

void PaintingWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);                //�����ӿ�����

	aspectRatio = (float)w / h;
}

#define ��x�� QVector3D(1.0f,0.0f,0.0f)
#define ��y�� QVector3D(0.0f,1.0f,0.0f)
#define ��Ԫ����ת
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


#ifdef ��Ԫ����ת
		float dx = event->x() - m_OldPoint.x();
		float dy = event->y() - m_OldPoint.y();
		dx *= 0.3;
		dy *= 0.3;
		QQuaternion xRotate, yRotate;

		yRotate = QQuaternion::fromAxisAndAngle(��y��, dx);
		xRotate = QQuaternion::fromAxisAndAngle(��x��, dy);


		m_Rotate = yRotate * xRotate * m_Rotate;

#endif

#ifdef ŷ������ת
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
#endif

	}
	if (event->buttons() & Qt::RightButton)
	{
		//model.translate(QVector3D(0.01, 0, 0));
	}
	m_OldPoint = event->pos();

	//QCursor::setPos(geometry().center());       //����긴ԭ����������
}

void PaintingWidget::mousePressEvent(QMouseEvent *event)
{
	m_OldPoint = event->pos();
}

void PaintingWidget::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)
	{
		m_Camera.m_CameraPos.setZ(m_Camera.m_CameraPos.z() - 0.1f);
	}
	else
	{
		m_Camera.m_CameraPos.setZ(m_Camera.m_CameraPos.z() + 0.1f);
	}
	update();
}

void PaintingWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	QQuaternion dr(QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 1));
	switch (keyEvent->key()) {
	case Qt::Key_Right:
		//m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() - 0.1f);	
		m_Rotate = dr * m_Rotate;
		m_View.rotate(10 ,QVector3D(0, 0, 1));
		break;
	case Qt::Key_Left:
		m_Camera.m_CameraPos.setX(m_Camera.m_CameraPos.x() + 0.1f);
		break;
	case Qt::Key_Up:
		m_Camera.m_CameraPos.setY(m_Camera.m_CameraPos.y() - 0.1f);
		break;
	case Qt::Key_Down:
		m_Camera.m_CameraPos.setY(m_Camera.m_CameraPos.y() + 0.1f);
		break;
	default:
		break;
	}
	update();
}


void PaintingWidget::fillColorBuffer()
{
	// 	GLfloat colorData[3 * 3];
	// 	for (int i = 0; i < 3; ++i) {
	// 		memcpy(&colorData[i * 3], this->colorBuffer, 3 * sizeof(GLfloat));
	// 	}
	// 	m_cbo->write(0, colorData, 3 * 3 * sizeof(GLfloat));
}

void PaintingWidget::setColor(GLfloat r, GLfloat g, GLfloat b)
{
// 	colorBuffer[0] = r;
// 	colorBuffer[1] = g;
// 	colorBuffer[2] = b;
// 	m_cbo->bind();
// 	fillColorBuffer();
// 	m_cbo->release();
// 	update();
}