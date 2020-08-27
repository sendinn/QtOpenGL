#include "QtOpenGL.h"
#include <QtWidgets/QApplication>
#include "myglwidget.h"
#include "PaintingWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// 	MyGLWidget w;
// 	w.resize(800, 600);
// 	w.show();

    QtOpenGL w;
	w.resize(800, 600);
    w.show();

    return a.exec();
}
