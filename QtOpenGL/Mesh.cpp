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
	initializeOpenGLFunctions();//��ʼ��opengl����

	m_Shader = new MyShader("VertexSource.txt", "FragmentSource.txt");

	m_VAO = new QOpenGLVertexArrayObject();
	m_VBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_CBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
	m_EBO = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

	QOpenGLVertexArrayObject::Binder{ m_VAO };
	/*
	�ȼ���
	m_vao->create();
	m_vao->bind();
	...
	m_vao->release();
	�������������Զ�release
	*/


	//������λ��
	{
		m_VBO->create();//����VBO����
		m_VBO->bind();//��VBO�󶨵���ǰ�Ķ��㻺�����QOpenGLBuffer::VertexBuffer����
		//���������ݷ��䵽VBO�У���һ������Ϊ����ָ�룬�ڶ�������Ϊ���ݵ��ֽڳ���
		m_VBO->allocate(&m_Vertices[0], m_Vertices.size() * sizeof(ModelVertex));// һ��Ҫд��&m_Vertices[0]���У�����û��ͼƬ
		//glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(ModelVertex), &m_Vertices, GL_STATIC_DRAW); ���		
		m_Shader->GetShader()->enableAttributeArray(0);
		/*
		��һ������ָ������Ҫ���õĶ������ԡ�������ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
		�ڶ�������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
		������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0��
		���ĸ�����ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
		�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(GLfloat)��
		*/
		m_Shader->GetShader()->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(ModelVertex));
		/*
		QOpenGLFunctions *f = this->context()->functions();
		f->glEnableVertexAttribArray(0); 
		f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0); 
		���
		*/
		m_VBO->release(); //releaseһ��Ҫ�ڸøö���ʹ�������ͷţ���ǰ�ͷŻ�ʲô����������
	}



// 	m_EBO->create();//����VBO����
// 	m_EBO->bind();//��VBO�󶨵���ǰ�Ķ��㻺�����QOpenGLBuffer::VertexBuffer����
// 	m_EBO->allocate(&m_Indices, m_Indices.size() * sizeof(unsigned int));//���������ݷ��䵽VBO�У���һ������Ϊ����ָ�룬�ڶ�������Ϊ���ݵ��ֽڳ���
// 	m_EBO->release();




	//��������ɫ
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
 