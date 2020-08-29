#include "MyShader.h"
#include <QFile>
#include <QDebug>
#include <iostream>
using namespace std;

MyShader::MyShader(QString VertexPath, QString FragmentPath)
{
	QFile f1(VertexPath), f2(FragmentPath);
	if (f1.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_VertexSource = f1.readAll();
		f1.close();
	}
	else
	{
		qDebug() << "VertexPath error!";
	}

	if (f2.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_FragmentSource = f2.readAll();
		f2.close();
	}
	else
	{
		cout<< "FragmentPath error!";
	}

	m_Shader = new QOpenGLShaderProgram();
	/*
	��ǰ�涨��� Vertex Shader �� Fragment Shader ��ʹ�� addShaderFromSourceCode ������ӵ� m_shader �У�
	Ȼ��ִ�� link() ��������������� GLSL ����
	*/
	if (!m_Shader->addShaderFromSourceCode(QOpenGLShader::Vertex, m_VertexSource))
	{
		qDebug() << "ERROR:" << m_Shader->log();    //����������,��ӡ������Ϣ
	}
	if(!m_Shader->addShaderFromSourceCode(QOpenGLShader::Fragment, m_FragmentSource))
	{
		qDebug() << "ERROR:" << m_Shader->log();    //����������,��ӡ������Ϣ
	}
	if (m_Shader->link()) {
		qDebug("Shaders link success.");
	}
	else {
		qDebug("Shaders link failed!");
	}

}
