#pragma once
#include <QString>
#include <QOpenGLShaderProgram>
class MyShader
{
public:
	MyShader(QString VertexPath,QString FragmentPath);

	QOpenGLShaderProgram* GetShader() {
		return m_Shader;
	}
	 

private:
	QOpenGLShaderProgram* m_Shader;
	QString m_VertexSource;
	QString m_FragmentSource;
};

