#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtOpenGL.h"
#include <QVBoxLayout>
#include "PaintingWidget.h"
class QtOpenGL : public QWidget
{
    Q_OBJECT

public:
    QtOpenGL(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtOpenGLClass ui;


private:
	QVBoxLayout *m_layout;
	PaintingWidget *m_painting;

};
