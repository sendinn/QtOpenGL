#include "QtOpenGL.h"
#include <QComboBox>
#include "MyMatrix.h"
QtOpenGL::QtOpenGL(QWidget *parent)
    : QWidget(parent)
{
    //ui.setupUi(this);


	m_layout = new QVBoxLayout(this);
	setLayout(m_layout);

	m_painting = new PaintingWidget(this);
	m_layout->addWidget(m_painting);

	MyMatrix t;
}
