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
	QComboBox *m_combo_box;
	PaintingWidget *m_painting;


private slots:
	void onComboBoxSelected(const QString &text);
};
