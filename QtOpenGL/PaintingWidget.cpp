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
	int a = 1;
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



	QSurfaceFormat format;
	format.setAlphaBufferSize(24);  //设置alpha缓冲大小
	format.setVersion(3, 3);         //设置版本号
	format.setSamples(10);          //设置重采样次数，用于反走样
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

	/*
	VAO的创建绑定与解绑是不是有点类似于线程锁，c++标准库中提供了lock_guard，它会在其构造函数中加锁，而在析构函数中解锁，而QOpenGLVertexArrayObject也提供了类似的机制，
	QOpenGLVertexArrayObject::Binder创建一个用于管理绑定的对象
	*/
	QOpenGLVertexArrayObject::Binder{ m_vao }; //绑定VAO（不存在时创建），离开作用域自动解绑 ==》代替vao的creat bind，以及之后的release  
	//m_vao->bind();
	m_shader->GetShader()->bind();
	QMatrix4x4 mvp;
	mvp.perspective(45.0f, this->aspectRatio, 0.1f, 100.0f);
	mvp.lookAt(camera, world, camera_Up);

	mvp.rotate(10.0f, m_RotateAsix);
	m_shader->GetShader()->setUniformValue(m_shader->GetShader()->uniformLocation("MVP"), mvp);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
	m_shader->GetShader()->release();
	//m_vao->release();

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