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
	format.setAlphaBufferSize(24);  //设置alpha缓冲大小
	format.setVersion(3, 3);         //设置版本号
	format.setSamples(10);          //设置重采样次数，用于反走样

	timer.setInterval(18);
	//connect(&timer, &QTimer::timeout, this, static_cast<void (PaintingWidget::*)()>(&PaintingWidget::update));
	timer.start();




	setMouseTracking(true);         //开启鼠标追踪：Qt默认不会实时监控鼠标移动

	//QCursor::setPos(geometry().center());   //设置鼠标位置为窗口矩形区域的中心
}
PaintingWidget::~PaintingWidget() {

}


/*
初始化 Shader Program
初始化顶点缓冲并装入三角形点位置坐标
*/
void PaintingWidget::initializeGL()
{
	initializeOpenGLFunctions();//初始化opengl函数	
	glEnable(GL_DEPTH_TEST);   // 三维绘图的关键！

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
	//类继承QOpenGLExtraFunctions无需该语句
	//QOpenGLFunctions *f = this->context()->functions();
	
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);//设置清屏颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓存

	m_Shader->GetShader()->bind();
	for (int i = 0; i < m_Meshes.size(); ++i)
	{
		{

			/*
			VAO的创建绑定与解绑是不是有点类似于线程锁，c++标准库中提供了lock_guard，它会在其构造函数中加锁，而在析构函数中解锁，而QOpenGLVertexArrayObject也提供了类似的机制，
			QOpenGLVertexArrayObject::Binder创建一个用于管理绑定的对象
			*/
			//QOpenGLVertexArrayObject::Binder{ m_vao }; //绑定VAO（不存在时创建），离开作用域自动解绑 ==》代替vao的creat bind，以及之后的release  
			//m_vao->bind();

			m_Model.setToIdentity();
			m_Model.rotate(m_Rotate);
			m_Shader->GetShader()->setUniformValue("model", m_Model);


			m_View.setToIdentity();
			m_View.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.m_CameraUp);
			m_Shader->GetShader()->setUniformValue("view", m_View);

			//透视投影
			m_Projection.setToIdentity();
			m_Projection.perspective(45.0f, width() / (float)height(), 0.1f, 10.0f);
			m_Shader->GetShader()->setUniformValue("projection", m_Projection);

#define test3

#ifdef test1
			float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
			QMatrix4x4 trans;
			trans.translate(0.0f, 0.5*qAbs(qSin(time)), 0.0f);        //向y轴平移0.5*[0,1]
			trans.scale(0.5*qAbs(qSin(time)), 0.5*qAbs(qSin(time))); //x，y在[0,0.5]进行缩放
			trans.rotate(360 * time, 0.0f, 0.0f, -1.0f);                 //旋转360*time
			m_shader->GetShader()->setUniformValue(m_shader->GetShader()->uniformLocation("MVP"), trans);
#endif


#ifdef test2
			float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
			QMatrix4x4 view;
			float radius = 10.0f;
			/*
			m_CameraPos + QVector3D 表示点+看向的方向，始终看向正前方
			view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + QVector3D(0,0,-1), m_Camera.GetCameraUp());
			m_CameraPos + Direction 表示观察点，始终看向同一个点
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
	glViewport(0, 0, w, h);                //定义视口区域

	aspectRatio = (float)w / h;
}

#define 正x轴 QVector3D(1.0f,0.0f,0.0f)
#define 正y轴 QVector3D(0.0f,1.0f,0.0f)
#define 四元数旋转
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


#ifdef 四元数旋转
		float dx = event->x() - m_OldPoint.x();
		float dy = event->y() - m_OldPoint.y();
		dx *= 0.3;
		dy *= 0.3;
		QQuaternion xRotate, yRotate;

		yRotate = QQuaternion::fromAxisAndAngle(正y轴, dx);
		xRotate = QQuaternion::fromAxisAndAngle(正x轴, dy);


		m_Rotate = yRotate * xRotate * m_Rotate;

#endif

#ifdef 欧拉角旋转
		float dx = event->x() - m_OldPoint.x();
		float dy = m_OldPoint.y() - event->y();
		dx *= 0.01;
		dy *= 0.01;
		m_Camera.yaw -= dx;
		m_Camera.pitch -= dy;
		if (m_Camera.pitch >= M_PI / 2)                          //将俯视角限制到[-90°,90°]
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

	//QCursor::setPos(geometry().center());       //将鼠标复原到窗口中央
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