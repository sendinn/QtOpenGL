#include "QtOpenGL.h"
#include <QtWidgets/QApplication>
#include "PaintingWidget.h"
//#include "TrackBall/widget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


// 	Widget t;
// 	t.resize(800, 600);
// 	t.show();


    QtOpenGL w;
	w.resize(800, 600);
    w.show();

    return a.exec();
}
