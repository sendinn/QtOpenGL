#include "QtOpenGL.h"
#include <QtWidgets/QApplication>
#include "PaintingWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtOpenGL w;
	w.resize(800, 600);
    w.show();

    return a.exec();
}
