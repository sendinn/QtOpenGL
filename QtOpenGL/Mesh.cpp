#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(QVector<ModelVertex>	vertices, QVector<ModelTexture>	textures, QVector<unsigned int>	indices)
{
	m_Vertices = vertices;
	m_Textures = textures;
	m_Indices = indices;	
	
}



Mesh::~Mesh()
{

}

void Mesh::SetupMesh()
{
	initializeOpenGLFunctions();//初始化opengl函数

	m_Shader = new MyShader("VertexSource.txt", "FragmentSource.txt");

	m_VAO = new QOpenGLVertexArrayObject();
	m_VBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_CBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_EBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

	QOpenGLVertexArrayObject::Binder{ m_VAO };
	/*
	等价于
	m_vao->create();
	m_vao->bind();
	...
	m_vao->release();
	超过生命周期自动release
	*/


	//处理顶点位置
	{
		m_VBO->create();//生成VBO对象
		m_VBO->bind();//将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中
		//将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
		m_VBO->allocate(&m_Vertices[0], m_Vertices.size() * sizeof(ModelVertex));// 一定要写成&m_Vertices[0]才行，否则没有图片
		//glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(ModelVertex), &m_Vertices, GL_STATIC_DRAW); 亦可		
		m_Shader->GetShader()->enableAttributeArray(0);
		/*
		第一个参数指定我们要配置的顶点属性。顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)，它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
		第二个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
		第三个参数表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
		第四个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
		第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(GLfloat)。
		*/
		m_Shader->GetShader()->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(ModelVertex));
		/*
		QOpenGLFunctions *f = this->context()->functions();
		f->glEnableVertexAttribArray(0); 
		f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0); 
		亦可
		*/
		m_VBO->release(); //release一定要在该该对象使用完再释放，提前释放会什么都画不出来
	}



// 	m_EBO->create();//生成VBO对象
// 	m_EBO->bind();//将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中
// 	m_EBO->allocate(&m_Indices, m_Indices.size() * sizeof(unsigned int));//将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
// 	m_EBO->release();




	//处理顶点颜色
	if(0)
	{
		m_CBO->create();
		m_CBO->bind();
		m_CBO->allocate(&m_Colors[0], m_Colors.size() * sizeof(vec3));
		m_Shader->GetShader()->setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(vec3));
		m_Shader->GetShader()->enableAttributeArray(1);
		m_CBO->release();
	}


}

void Mesh::Draw()
{
	//QOpenGLVertexArrayObject::Binder{ m_VAO };

	for (unsigned int i = 0; i < m_Textures.size(); ++i)
	{
		if (m_Textures[i].type == "texture_diffuse")
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}
		else if (m_Textures[i].type == "texture_specular")
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}
	}
	glDrawArrays(GL_POINTS, 0, 3);

}
 