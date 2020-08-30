#pragma once
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include "MyShader.h"

struct vec2
{
	GLfloat x;
	GLfloat y;
};

struct vec3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct ModelVertex
{
	vec3 Position;
// 	vec3 Normal;
// 	vec2 TexCoords;
};


struct ModelTexture
{
	unsigned int id;
	QString type;
	QString path;
};


class Mesh : public QOpenGLWidget, public QOpenGLExtraFunctions
{
public:
	Mesh();
	Mesh(QVector<ModelVertex>	vertices,QVector<ModelTexture>	textures,QVector<unsigned int>	indices);
	~Mesh();


	void SetupMesh();
	void Draw();

	void SetVertex(GLfloat vertices[],int size) {
		m_Vertices.resize(size);
		int a = sizeof(m_Vertices);
		int b = sizeof(ModelVertex);
		memcpy(&(m_Vertices[0]), vertices, size * sizeof(ModelVertex)); //直接写sizeof(m_Vertices)会出错
	}

	void SetColor(GLfloat colors[], int size) {
		m_Colors.resize(size);
		memcpy(&(m_Colors[0]), colors, size * sizeof(vec3));
	}

	QOpenGLShaderProgram* GetShader() {
		return m_Shader->GetShader();
	}

	QVector<ModelVertex>	m_Vertices;
	QVector<ModelTexture>	m_Textures;
	QVector<unsigned int>	m_Indices;
	QVector<vec3>			m_Colors;
private:
	MyShader* m_Shader;

	QOpenGLVertexArrayObject* m_VAO;
	QOpenGLBuffer* m_VBO;//顶点
	QOpenGLBuffer* m_CBO;//颜色
	QOpenGLBuffer* m_EBO;

};

