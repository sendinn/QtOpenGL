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
	把前面定义的 Vertex Shader 和 Fragment Shader 都使用 addShaderFromSourceCode 方法添加到 m_shader 中，
	然后执行 link() 方法编译链接这个 GLSL 程序
	*/
	if (!m_Shader->addShaderFromSourceCode(QOpenGLShader::Vertex, m_VertexSource))
	{
		qDebug() << "ERROR:" << m_Shader->log();    //如果编译出错,打印报错信息
	}
	if(!m_Shader->addShaderFromSourceCode(QOpenGLShader::Fragment, m_FragmentSource))
	{
		qDebug() << "ERROR:" << m_Shader->log();    //如果编译出错,打印报错信息
	}
	if (m_Shader->link()) {
		qDebug("Shaders link success.");
	}
	else {
		qDebug("Shaders link failed!");
	}

}
