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
	format.setAlphaBufferSize(24);  //设置alpha缓冲大小
	format.setVersion(3, 3);         //设置版本号
	format.setSamples(10);          //设置重采样次数，用于反走样

	timer.setInterval(18);
	//connect(&timer, &QTimer::timeout, this, static_cast<void (PaintingWidget::*)()>(&PaintingWidget::update));
	timer.start();

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
	//QOpenGLFunctions *f = this->context()->functions();

	m_shader = new MyShader("VertexSource.txt", "FragmentSource.txt");
	
	glEnable(GL_DEPTH_TEST);   // 三维绘图的关键！

	m_vao = new QOpenGLVertexArrayObject();
	m_vbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_cbo = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	QOpenGLVertexArrayObject::Binder{ m_vao };
	//m_vao->create();
	//m_vao->bind();
	m_vbo->create();//生成VBO对象
	m_vbo->bind();//将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中
	m_vbo->allocate(this->vertexData, 4 * 3 * 3 * sizeof(GLfloat));//将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度

	/*
	第一个参数指定我们要配置的顶点属性。顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)，它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	第二个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	第三个参数表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
	第四个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(GLfloat)。
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
	//m_vao->release(); //解绑VAO，安全使用

}




void PaintingWidget::paintGL()
{
	//类继承QOpenGLExtraFunctions无需该语句
	//QOpenGLFunctions *f = this->context()->functions();
	
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);//设置清屏颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓存


	m_shader->GetShader()->bind();
	{

		/*
		VAO的创建绑定与解绑是不是有点类似于线程锁，c++标准库中提供了lock_guard，它会在其构造函数中加锁，而在析构函数中解锁，而QOpenGLVertexArrayObject也提供了类似的机制，
		QOpenGLVertexArrayObject::Binder创建一个用于管理绑定的对象
		*/
		QOpenGLVertexArrayObject::Binder{ m_vao }; //绑定VAO（不存在时创建），离开作用域自动解绑 ==》代替vao的creat bind，以及之后的release  
		//m_vao->bind();

		QMatrix4x4 model;
		m_shader->GetShader()->setUniformValue("model", model);

		QMatrix4x4 view;
		view.lookAt(m_Camera.m_CameraPos, m_Camera.m_CameraPos + m_Camera.m_CameraFront, m_Camera.m_CameraUp);
		m_shader->GetShader()->setUniformValue("view", view);

		QMatrix4x4 projection;
		//透视投影
		projection.perspective(45.0f, width() / (float)height(), 0.1f, 10.0f);
		m_shader->GetShader()->setUniformValue("projection", projection);

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
	glViewport(0, 0, w, h);                //定义视口区域

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
		if (m_Camera.pitch >= M_PI / 2)                          //将俯视角限制到[-90°,90°]
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